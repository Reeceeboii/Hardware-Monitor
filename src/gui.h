//
// Created by reece on 13/12/2019.
//

#ifndef LINUX_SYSTEM_MONITOR_GUI_H
#define LINUX_SYSTEM_MONITOR_GUI_H

#include "utils.h"
#include <gtk/gtk.h>

// window and grid layout
struct window{
    GtkWidget* window;
    GtkWidget* top_level_grid;
    GtkWidget* menu_bar;
    GtkWidget* top_level_box;
};

// program labels
struct labels{
    GtkWidget *cpu_model_label, *core_count_label, *thread_count_label, *cache_size_label;
    GtkWidget *total_mem_label, *mem_available_label, *total_swap_label, *swap_available_label;
};

// other GUI elements
struct misc{
    GtkWidget *mem_used_bar, *swap_used_bar;
};

void load_gui(struct callback_bundle* cbb, GtkBuilder* builder);
void set_labels(struct callback_bundle* cbb);

#endif //LINUX_SYSTEM_MONITOR_GUI_H
