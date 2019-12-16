/*
 * Created by Reece Mercer
 */

#include "procParser.h"
#include "gui.h"
#include "utils.h"
#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>


/**
 * Callback used by the refresh button in the 'file' menu bar option
 * @param invoker The widget that invoked the callback function
 * @param callback_bundle_ptr A pointer to the program's callback bundle struct
 */
void refresh_callback(GtkWidget* invoker, gpointer callback_bundle_ptr){
    struct callback_bundle* cbb = callback_bundle_ptr;
    *cbb->cpuParsed = parse_cpu();
    *cbb->memParsed = parse_mem(cbb);
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(cbb->misc->mem_used_bar), calc_mem_used_percentage(cbb));
    set_labels(cbb);
}

/**
 * Callback function that is fired when one of the data unit radio buttons is changed
 * @param invoker Pointer to the radio button widget that invoked this callback
 * @param callback_bundle_ptr A pointer to the program's callback bundle
 */
void data_unit_change_callback(GtkWidget* invoker, gpointer callback_bundle_ptr){
   struct callback_bundle* cbb = callback_bundle_ptr;
   struct data_unit_radios* radios = cbb->radios;
   if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radios->kib_but))){

   }
   printf("%s\n", "Hello this callback works");

}
/**
 * Periodically update the program's data using fresh calls to proc
 * @param callback_bundle A pointer to the program's callback bundle
 * @return gboolean TRUE to stop this callback being unregistered
 */
gboolean timed_refresh(void* callback_bundle){
    struct callback_bundle* cbb = callback_bundle;
    *cbb->memParsed = parse_mem(cbb); // recalculate memory data
    struct mem_parsed* mem = cbb->memParsed;
    gdouble mem_used_percentage = calc_mem_used_percentage(cbb);
    char mem_used_str[100];
    sprintf(mem_used_str, "%s remaining of %s (%.1f%%) used",
            mem->mem_available_gib, mem->total_mem_gib, mem_used_percentage * 100);
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(cbb->misc->mem_used_bar), mem_used_percentage);
    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(cbb->misc->mem_used_bar), mem_used_str);
    set_labels(cbb);
    return TRUE;
}

int main(int argc, char* argv[]){
    GtkBuilder* builder;
    struct window win;
    struct labels lab;
    struct data_unit_radios radios;
    struct misc misc;
    struct callback_bundle callbackBundle = {
            .periodic_refresh_rate = 250, .mem_data_type = KB, .win = &win, .lab = &lab,
            .radios = &radios, .misc = &misc };
    struct CPU_parsed cpuParsed = parse_cpu();
    struct mem_parsed memParsed = parse_mem(&callbackBundle);
    callbackBundle.cpuParsed = &cpuParsed;
    callbackBundle.memParsed = &memParsed;

    // initialise GTK
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("gui.glade");
    win.window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    g_signal_connect(win.window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    // construct signal callback table
    gtk_builder_connect_signals(builder, &callbackBundle);

    load_gui(&callbackBundle, builder);
    // set up a periodic refresh of the data retrieved from proc
    g_timeout_add(callbackBundle.periodic_refresh_rate, (GSourceFunc)timed_refresh, &callbackBundle);
    gtk_widget_show(win.window);
    // kick off the GTK main loop
    gtk_main();
    return EXIT_SUCCESS;
}