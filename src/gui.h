//
// Created by reece on 13/12/2019.
//

#ifndef LINUX_SYSTEM_MONITOR_GUI_H
#define LINUX_SYSTEM_MONITOR_GUI_H

#include <gtk/gtk.h>

// window and grid layout
struct window{
    GtkWidget* window;
    GtkWidget* top_level_fixed;
};

// program labels
struct labels{
    GtkWidget* cpu_model_label;
};

#endif //LINUX_SYSTEM_MONITOR_GUI_H
