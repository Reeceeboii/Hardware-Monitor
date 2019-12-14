/*
 * Created by Reece Mercer
 */

#include "procParser.h"
#include "gui.h"
#include "main.h"
#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>

/**
 * Update GUI labels with up to date system information
 * @param cbb A pointer to the callback bundle
 */
void set_labels(struct callback_bundle* cbb) {
    struct labels* lab = cbb->lab;
    struct CPU_parsed* cpuParsed = cbb->cpuParsed;
    struct mem_parsed* memParsed = cbb->memParsed;
    // cpu
    gtk_label_set_text(GTK_LABEL(lab->cpu_model_label), cpuParsed->model_name);
    gtk_label_set_text(GTK_LABEL(lab->core_count_label), cpuParsed->core_count);
    gtk_label_set_text(GTK_LABEL(lab->cache_size_label), cpuParsed->cache_size);
    // memory
    gtk_label_set_text(GTK_LABEL(lab->total_mem_label), memParsed->total_mem);
    gtk_label_set_text(GTK_LABEL(lab->mem_free_label), memParsed->mem_free);
    gtk_label_set_text(GTK_LABEL(lab->total_swap_label), memParsed->total_swap);
    gtk_label_set_text(GTK_LABEL(lab->swap_free_label), memParsed->swap_free);
}

/**
 * Construct GTK GUI from Glade XML
 * @param cbb A pointer to the callback bundle
 * @param builder A pointer to a GtkBuilder
 */
void load_gui(struct callback_bundle* cbb, GtkBuilder* builder){
    struct window* win = cbb->win;
    struct labels* lab = cbb->lab;
    // load window
    win->top_level_box = GTK_WIDGET(gtk_builder_get_object(builder, "top_level_box"));
    win->menu_bar = GTK_WIDGET(gtk_builder_get_object(builder, "menu_bar"));
    win->top_level_grid = GTK_WIDGET(gtk_builder_get_object(builder, "top_level_grid"));
    // load labels
    lab->cpu_model_label = GTK_WIDGET(gtk_builder_get_object(builder, "cpu_model_label"));
    lab->core_count_label = GTK_WIDGET(gtk_builder_get_object(builder, "core_count_label"));
    lab->cache_size_label = GTK_WIDGET(gtk_builder_get_object(builder, "cache_size_label"));
    lab->total_mem_label = GTK_WIDGET(gtk_builder_get_object(builder, "total_mem_label"));
    lab->mem_free_label = GTK_WIDGET(gtk_builder_get_object(builder, "mem_free_label"));
    lab->total_swap_label = GTK_WIDGET(gtk_builder_get_object(builder, "total_swap_label"));
    lab->swap_free_label = GTK_WIDGET(gtk_builder_get_object(builder, "swap_free_label"));
    // populate with data
    set_labels(cbb);
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
    set_labels(callback_bundle_ptr);
}

int main(int argc, char* argv[]){
    GtkBuilder* builder;
    struct window win;
    struct labels lab;
    struct CPU_parsed cpuParsed = parse_cpu();
    struct mem_parsed memParsed = parse_mem();
    struct callback_bundle callbackBundle = {
            .mem_data_type = KB, .win = &win, .lab = &lab,
            .cpuParsed = &cpuParsed, .memParsed = &memParsed };

    // initialise GTK
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("gui.glade");
    win.window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    g_signal_connect(win.window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    // construct signal callback table
    gtk_builder_connect_signals(builder, &callbackBundle);

    load_gui(&callbackBundle, builder);

    //g_timeout_add_seconds(1, (GSourceFunc)callback, &val);
    gtk_widget_show(win.window);
    gtk_main();

    return EXIT_SUCCESS;
}