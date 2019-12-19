//
// Created by reece on 13/12/2019.
//

#ifndef LINUX_SYSTEM_MONITOR_GUI_H
#define LINUX_SYSTEM_MONITOR_GUI_H

#include "utils.h"
#include "procParser.h"
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
    GtkWidget *mem_available_label, *swap_available_label;
};

// other GUI elements
struct misc{
    GtkWidget *mem_used_bar, *swap_used_bar;
    GtkWidget * thread_labels[MAX_SUPPORTED_THREADS];
};

void load_gui(struct callback_bundle* cbb, GtkBuilder* builder);
void set_labels(struct callback_bundle* cbb);

#endif //LINUX_SYSTEM_MONITOR_GUI_H
