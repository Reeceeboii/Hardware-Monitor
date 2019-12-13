#include "procParser.h"
#include "cpuinfo.h"
#include "gui.h"
#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>


int main(int argc, char* argv[]){
    GtkBuilder* builder;
    struct window win;
    struct labels lab;
    struct cpu_details cpuDetails;
    cpuDetails.name = get_cpu_model_name();

    // initialise GTK
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("gui.glade");
    win.window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    g_signal_connect(win.window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    // construct signal callback table
    gtk_builder_connect_signals(builder, NULL);

    // construct GUI from Glade XML
    win.top_level_fixed = GTK_WIDGET(gtk_builder_get_object(builder, "top_level_fixed"));
    lab.cpu_model_label = GTK_WIDGET(gtk_builder_get_object(builder, "cpu_model_label"));

    //g_timeout_add_seconds(1, (GSourceFunc)callback, &val);

    gtk_label_set_text(GTK_LABEL(lab.cpu_model_label), cpuDetails.name);

    gtk_widget_show(win.window);
    gtk_main();

    return EXIT_SUCCESS;
}