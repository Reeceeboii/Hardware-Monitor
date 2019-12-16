//
// Created by reece on 16/12/2019.
//

#include "gui.h"
#include "procParser.h"

/**
 * Update GUI labels with up to date system information
 * @param cbb A pointer to the callback bundle
 */
void set_labels(struct callback_bundle* cbb){
    struct labels* lab = cbb->lab;
    struct CPU_parsed* cpuParsed = cbb->cpuParsed;
    struct mem_parsed* memParsed = cbb->memParsed;
    // cpu
    gtk_label_set_text(GTK_LABEL(lab->cpu_model_label), cpuParsed->model_name);
    gtk_label_set_text(GTK_LABEL(lab->core_count_label), cpuParsed->core_count);
    gtk_label_set_text(GTK_LABEL(lab->cache_size_label), cpuParsed->cache_size);
    // memory
    gtk_label_set_text(GTK_LABEL(lab->total_mem_label), memParsed->total_mem);
    gtk_label_set_text(GTK_LABEL(lab->mem_available_label), memParsed->mem_available);
    gtk_label_set_text(GTK_LABEL(lab->total_swap_label), memParsed->total_swap);
    gtk_label_set_text(GTK_LABEL(lab->swap_available_label), memParsed->swap_free);
}

/**
 * Construct GTK GUI from Glade XML
 * @param cbb A pointer to the program's callback bundle
 * @param builder A pointer to the program's GtkBuilder
 */
void load_gui(struct callback_bundle* cbb, GtkBuilder* builder){
    struct window* win = cbb->win;
    struct labels* lab = cbb->lab;
    struct misc* misc = cbb->misc;
    struct data_unit_radios* radios = cbb->radios;
    // load window
    win->top_level_box = GTK_WIDGET(gtk_builder_get_object(builder, "top_level_box"));
    win->menu_bar = GTK_WIDGET(gtk_builder_get_object(builder, "menu_bar"));
    win->top_level_grid = GTK_WIDGET(gtk_builder_get_object(builder, "top_level_grid"));
    // load labels
    lab->cpu_model_label = GTK_WIDGET(gtk_builder_get_object(builder, "cpu_model_label"));
    lab->core_count_label = GTK_WIDGET(gtk_builder_get_object(builder, "core_count_label"));
    lab->cache_size_label = GTK_WIDGET(gtk_builder_get_object(builder, "cache_size_label"));
    lab->total_mem_label = GTK_WIDGET(gtk_builder_get_object(builder, "total_mem_label"));
    lab->mem_available_label = GTK_WIDGET(gtk_builder_get_object(builder, "mem_available_label"));
    lab->total_swap_label = GTK_WIDGET(gtk_builder_get_object(builder, "total_swap_label"));
    lab->swap_available_label = GTK_WIDGET(gtk_builder_get_object(builder, "swap_available_label"));
    // load other GUI elements
    misc->mem_used_bar = GTK_WIDGET(gtk_builder_get_object(builder, "mem_used_bar"));
    // load radio buttons
    radios->kib_but = GTK_WIDGET(gtk_builder_get_object(builder, "kib_radio_button"));
    radios->mib_but = GTK_WIDGET(gtk_builder_get_object(builder, "mib_radio_button"));
    radios->gib_but = GTK_WIDGET(gtk_builder_get_object(builder, "gib_radio_button"));

    // populate labels with data
    set_labels(cbb);
}