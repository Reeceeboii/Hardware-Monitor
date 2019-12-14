/*
 * Created by Reece Mercer
 */

#include "procParser.h"
#include "gui.h"
#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>

/**
 * Callback struct used to pass supposedly global params around without them having global scope.
 * Callbacks can annoyingly only have one parameter so I have to bundle multiple up to use more than 1
 */
struct callback_bundle{
    struct labels* lab;
    struct CPU_parsed* cpuParsed;
    struct mem_parsed* memParsed;
};

/**
 * Update GUI labels with up to date system information
 * @param lab Pointer to a labels struct
 * @param cpu Pointer to a CPU_parsed struct
 */
void set_labels(struct labels* lab, const struct CPU_parsed* cpu) {
    gtk_label_set_text(GTK_LABEL(lab->cpu_model_label), cpu->model_name);
    gtk_label_set_text(GTK_LABEL(lab->core_count_label), cpu->core_count);
    gtk_label_set_text(GTK_LABEL(lab->cache_size_label), cpu->cache_size);

}

/**
 * Construct GTK GUI from Glade XML
 * @param win A pointer to a window struct
 * @param lab A pointer to a labels struct
 * @param builder A pointer to a GtkBuilder
 * @param cpu A pointer to a CPU_parsed struct
 */
void load_gui(struct window* win, struct labels* lab, GtkBuilder* builder, const struct CPU_parsed* cpu){
    win->top_level_box = GTK_WIDGET(gtk_builder_get_object(builder, "top_level_box"));
    win->menu_bar = GTK_WIDGET(gtk_builder_get_object(builder, "menu_bar"));
    win->top_level_grid = GTK_WIDGET(gtk_builder_get_object(builder, "top_level_grid"));
    win->thread_tree_view = GTK_WIDGET(gtk_builder_get_object(builder, "thread_tree_view"));
    lab->cpu_model_label = GTK_WIDGET(gtk_builder_get_object(builder, "cpu_model_label"));
    lab->core_count_label = GTK_WIDGET(gtk_builder_get_object(builder, "core_count_label"));
    lab->cache_size_label = GTK_WIDGET(gtk_builder_get_object(builder, "cache_size_label"));
    // populate with data
    set_labels(lab, cpu);
}

/**
 * Callback used by the refresh button in the 'file' menu bar option
 * @param invoker The widget that invoked the callback function
 * @param callback_bundle_ptr A pointer to the program's callback bundle struct
 */
void refresh_callback(GtkWidget* invoker, gpointer callback_bundle_ptr){
    struct callback_bundle* cbb = callback_bundle_ptr;
    *cbb->cpuParsed = parse_cpu();
    *cbb->memParsed = parse_mem();
    set_labels(cbb->lab, cbb->cpuParsed);
}

int main(int argc, char* argv[]){
    GtkBuilder* builder;
    struct window win;
    struct labels lab;
    struct CPU_parsed cpuParsed = parse_cpu();
    struct mem_parsed memParsed = parse_mem();
    struct callback_bundle callbackBundle = {
            .lab = &lab, .cpuParsed = &cpuParsed, .memParsed = &memParsed };

    // initialise GTK
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("gui.glade");
    win.window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    g_signal_connect(win.window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    // construct signal callback table
    gtk_builder_connect_signals(builder, &callbackBundle);

    load_gui(&win, &lab, builder, &cpuParsed);

    //g_timeout_add_seconds(1, (GSourceFunc)callback, &val);
    gtk_widget_show(win.window);
    gtk_main();

    return EXIT_SUCCESS;
}