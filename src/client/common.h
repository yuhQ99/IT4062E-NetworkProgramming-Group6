#ifndef _COMMON_H
#define _COMMON_H

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define LENGTH 1000

GtkWidget *button_sign_in;
GtkWidget *button_sign_up;
GtkBuilder  *builder; 
GtkWidget   *window_main;
GtkWidget   *window_account;
GtkWidget   *label_account_mode;
GtkWidget   *entry_username;
GtkWidget   *entry_password;
GtkWidget   *button_account_confirm;
GtkWidget   *window_room_choice;
GtkWidget   *button_create_room;
GtkWidget   *entry_roomID_join;
GtkWidget   *window_room_create;
GtkWidget   *label_account_noti;
GtkWidget   *alert_dialog;
GtkWidget   *button_close;
GtkWidget   *label_alert;
GtkWidget   *window_chat;
GtkWidget   *label_roomID;
GtkWidget   *view_chat;
GtkWidget   *entry_message;
GtkWidget   *button_send_message;
GtkWidget   *scroll_chat;
GtkTreeStore    *chat_store;
GtkTreeView     *tree_view_chat_room;
GtkTreeViewColumn    *column_user;
GtkCellRenderer    *chat_content;
GtkTreeSelection   *selection;

int sockfd;
char username[30];

#endif
