#include <stdio.h>
#include <stdlib.h>
#include <gtk-3.0/gtk/gtk.h>

#include "cp.h"

struct window_attr
{
    GtkFileChooser *gtk_file_chooser;
    GtkWidget *window;
    GtkEntry *entry_src;
    GtkEntry *entry_dst;
};

static void check_inputs(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
    struct window_attr *w_att = data;
    if (gtk_entry_get_text(GTK_ENTRY(w_att->entry_src)) != '\0')
        g_print("hey");
    

}
static void open_dialog_src(GtkWidget *buton, gpointer data)
{
    struct window_attr *w_att = data;
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new("Choose a source file", GTK_WINDOW(w_att->window), GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
    gtk_widget_show_all(dialog);
    gint resp=gtk_dialog_run(GTK_DIALOG(dialog));
    if (resp == GTK_RESPONSE_OK)
        g_print("%s\n", gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
    else
        g_print("You pressed the cancel button !\n");
    
    gtk_entry_set_text(w_att->entry_src, gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
    gtk_widget_destroy(dialog);
 
}
static void *open_dialog_dst(GtkWidget *buton, gpointer data)
{
    struct window_attr *w_att = data;
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new("Choose a destination file", GTK_WINDOW(w_att->window), GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
    gtk_widget_show_all(dialog);
    gint resp=gtk_dialog_run(GTK_DIALOG(dialog));
    if (resp == GTK_RESPONSE_OK)
        g_print("%s\n", gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
    else
        g_print("You pressed the cancel button !\n");
    
    gtk_entry_set_text(w_att->entry_dst ,gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
    gtk_widget_destroy(dialog);
 
}

static void set_window_attr(GtkWidget **window)
{
    struct window_attr *win_attr = (struct window_attr*)malloc(sizeof(struct window_attr));

    GtkWidget *button_src, *button_dst, *fixed, *button_go;
    GtkEntry *entry_src, *entry_dst;

    *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(*window), "COPY FILE GUI");
    gtk_window_set_default_size(GTK_WINDOW(*window), 800, 250);
    gtk_window_set_position(GTK_WINDOW(*window), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(*window), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(*window), 10);
    win_attr->window = *window;
    fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(*window), fixed);


    button_src = gtk_button_new_with_label("Source");
    gtk_widget_set_size_request(button_src, 120, 30);
    gtk_fixed_put(GTK_FIXED(fixed), button_src, 15, 15);    
    
    button_dst = gtk_button_new_with_label("Destination");
    gtk_widget_set_size_request(button_dst, 120, 30);
    gtk_fixed_put(GTK_FIXED(fixed), button_dst, 15, 100);

    button_go = gtk_button_new_with_label("Copy");
    gtk_widget_set_size_request(button_go, 120, 30);
    gtk_fixed_put(GTK_FIXED(fixed), button_go, 630, 200);

    g_signal_connect(button_dst, "clicked", G_CALLBACK(open_dialog_dst), win_attr);
    g_signal_connect(button_src, "clicked", G_CALLBACK(open_dialog_src), win_attr);


    entry_src = gtk_entry_new ();
    gtk_widget_set_size_request(entry_src, 600, 30);
    gtk_fixed_put(GTK_FIXED(fixed), entry_src, 160, 15);

    entry_dst = gtk_entry_new ();
    gtk_widget_set_size_request(entry_dst, 600, 30);
    gtk_fixed_put(GTK_FIXED(fixed), entry_dst, 160, 100);
    
    win_attr->entry_dst = entry_dst;
    win_attr->entry_src = entry_src;
    gtk_widget_add_events(*window, GDK_KEY_PRESS_MASK);

    g_signal_connect (G_OBJECT (*window), "key_press_event",
        G_CALLBACK (check_inputs), win_attr);
    gtk_widget_show_all(*window);



}


int main(int argc, char *argv[])
{
    GtkWidget *window;

    gtk_init(&argc, &argv);

    set_window_attr(&window);

    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_main();

}