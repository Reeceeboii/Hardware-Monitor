#include "procParser.h"
#include "gui.h"
#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>

// construct all GUI objects from Glade XML
void load_gui(struct window* win, struct labels* lab, GtkBuilder* builder, struct CPU_parsed* cpu){
    win->top_level_box = GTK_WIDGET(gtk_builder_get_object(builder, "top_level_box"));
    win->menu_bar = GTK_WIDGET(gtk_builder_get_object(builder, "menu_bar"));
    win->top_level_grid = GTK_WIDGET(gtk_builder_get_object(builder, "top_level_grid"));
    lab->cpu_model_label = GTK_WIDGET(gtk_builder_get_object(builder, "cpu_model_label"));
    // populate with data
    gtk_label_set_text(GTK_LABEL(lab->cpu_model_label), cpu->model_name);
}

int main(int argc, char* argv[]){
    GtkBuilder* builder;
    struct window win;
    struct labels lab;
    struct CPU_parsed cpuParsed = parse_cpu();

    // initialise GTK
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("gui.glade");
    win.window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    g_signal_connect(win.window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    // construct signal callback table
    gtk_builder_connect_signals(builder, NULL);

    load_gui(&win, &lab, builder, &cpuParsed);

    //g_timeout_add_seconds(1, (GSourceFunc)callback, &val);
    gtk_widget_show(win.window);
    gtk_main();

    return EXIT_SUCCESS;
}