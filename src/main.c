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
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(cbb->misc->swap_used_bar), calc_swap_used_percentage(cbb));
    set_labels(cbb);
}

/**
 * Periodically update the program's data using fresh calls to proc
 * @param callback_bundle A pointer to the program's callback bundle
 * @return gboolean TRUE to stop this callback being unregistered
 */
gboolean timed_refresh(void* callback_bundle){
    struct callback_bundle* cbb = callback_bundle;
    *cbb->memParsed = parse_mem(cbb); // recalculate memory data
    *cbb->cpuParsed = parse_cpu(cbb);
    struct mem_parsed* mem = cbb->memParsed;
    gdouble mem_used_percentage = calc_mem_used_percentage(cbb);
    gdouble swap_used_percentage = calc_swap_used_percentage(cbb);
    char mem_used_str[100];
    char swap_used_str[100];
    sprintf(mem_used_str, "%s remaining of %s (%.1f%%) used",
            mem->mem_available_gib, mem->total_mem_gib, mem_used_percentage * 100);
    sprintf(swap_used_str, "%s remaining of %s (%.1f%%) used",
            mem->swap_available_gib, mem->total_swap_gib, swap_used_percentage * 100);
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(cbb->misc->mem_used_bar), mem_used_percentage);
    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(cbb->misc->mem_used_bar), mem_used_str);
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(cbb->misc->swap_used_bar), swap_used_percentage);
    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(cbb->misc->swap_used_bar), swap_used_str);

    set_labels(cbb);
    return TRUE;
}

int main(int argc, char* argv[]){
    // initialise GTK
    gtk_init(&argc, &argv);
    GtkBuilder* builder;
    struct window win;
    builder = gtk_builder_new_from_file("gui.glade");
    win.window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    g_signal_connect(win.window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    struct labels lab;
    struct misc misc;
    struct callback_bundle callbackBundle = {
            .periodic_refresh_rate = 150, .mem_data_type = KB, .freq_unit = GHz, .win = &win, .lab = &lab,
            .misc = &misc };
    struct CPU_parsed cpuParsed = parse_cpu();
    // make sure use has < MAX_SUPPORTED_THREADS (procparse.h)
    if(cpuParsed.thread_count > MAX_SUPPORTED_THREADS){
        printf("No more than %d threads are supported, you have %d",
                MAX_SUPPORTED_THREADS, cpuParsed.thread_count);
        exit(EXIT_SUCCESS);
    }

    struct mem_parsed memParsed = parse_mem(&callbackBundle);
    callbackBundle.cpuParsed = &cpuParsed;
    callbackBundle.memParsed = &memParsed;

    // construct signal callback table
    gtk_builder_connect_signals(builder, &callbackBundle);

    load_gui(&callbackBundle, builder);

    for(int i = 0; i < callbackBundle.cpuParsed->thread_count; ++i){
        char label[25];
        sprintf(label, "%.4f", callbackBundle.cpuParsed->thread_freq[i]);
        GtkWidget* thread_label = gtk_label_new(label);
        misc.thread_labels[i] = thread_label;
        gtk_grid_attach(GTK_GRID(win.top_level_grid), misc.thread_labels[i], 2, i, 1, 1);
    }

    // set up a periodic refresh of the data retrieved from proc
    g_timeout_add(callbackBundle.periodic_refresh_rate, (GSourceFunc)timed_refresh, &callbackBundle);

    gtk_widget_show_all(win.window);
    gtk_widget_show(win.window);
    // kick off the GTK main loop
    gtk_main();
    return EXIT_SUCCESS;
}