#include <gtk/gtk.h>
#include <string.h>

GtkWidget* window;
GtkWidget* fixed1;
GtkWidget* button1;
GtkWidget* label1;
GtkBuilder* builder;
int x = 10;

int main(int argc, char* argv[]){
    // initialise GTK
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("gui.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    // construct signal callback table
    gtk_builder_connect_signals(builder, NULL);

    // construct GUI from Glade XML
    fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
    button1 = GTK_WIDGET(gtk_builder_get_object(builder, "button1"));
    label1 = GTK_WIDGET(gtk_builder_get_object(builder, "label1"));

    gtk_widget_show(window);
    gtk_main();

    return EXIT_SUCCESS;
}

void on_button1_clicked(GtkButton* b){
    ++x;
    char str[15];
    sprintf(str, "%d", x);
    gtk_label_set_text(GTK_LABEL(label1), str);
}