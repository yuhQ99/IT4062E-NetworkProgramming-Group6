#include "common.h"
#include "callback.h"
#include "account.h"
#include "WindowMain.h"

void main_window_init() {
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "../../glade/homescreen.glade", NULL);

    window_main = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);

    button_sign_in = GTK_WIDGET(gtk_builder_get_object(builder, "button_sign_in"));
    gtk_builder_connect_signals(builder, NULL);
    button_sign_up = GTK_WIDGET(gtk_builder_get_object(builder, "button_sign_up"));
}

void on_button_sign_in_clicked() {
    char tmp[30];
    strcpy(tmp, "Sign in");
    
    gtk_widget_hide(window_main);
    login_init(tmp);
}

void on_button_sign_up_clicked() {
    char tmp[30];
    strcpy(tmp, "Sign up");
    
    gtk_widget_hide(window_main);
    login_init(tmp);
}