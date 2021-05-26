#include "common.h"
#include "callback.h"

void on_window_main_destroy()
{
    gtk_main_quit();
}

void on_window_account_destroy() {
    gtk_main_quit();
}

void on_window_room_choice_destroy() {
    gtk_main_quit();
}

void on_window_chat_destroy() {
    gtk_main_quit();
}