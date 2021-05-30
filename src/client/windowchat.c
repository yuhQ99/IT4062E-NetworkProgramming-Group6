#include "common.h"
#include "callback.h"
#include "account.h"
#include "room.h"
#include "windowchat.h"

char mess[LENGTH];

void add_to_list(const gchar *str) {
    GtkTreeIter iter;
    gtk_tree_store_append(chat_store, &iter, NULL);
    gtk_tree_store_set(chat_store, &iter, 0, str, -1);
}

void recv_msg() {
    char message[LENGTH] = {};
  	while (1) {
		int receive = recv(sockfd, message, LENGTH, 0);
        if (receive > 0) {
            add_to_list(message);
        } else if (receive == 0) {
		    break;
        } else {
		// -1
	    }
	memset(message, 0, sizeof(message));
  }
}

void window_chat_init(char* roomid) {
    pthread_t recv_msg_thread;
    
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "../../glade/homescreen.glade", NULL);

    window_chat = GTK_WIDGET(gtk_builder_get_object(builder, "window_chat"));
    gtk_builder_connect_signals(builder, NULL);

    scroll_chat = GTK_WIDGET(gtk_builder_get_object(builder, "scroll_chat"));
    entry_message = GTK_WIDGET(gtk_builder_get_object(builder, "entry_message"));
    button_send_message = GTK_WIDGET(gtk_builder_get_object(builder, "entry_message"));
    label_roomID = GTK_WIDGET(gtk_builder_get_object(builder, "label_roomID"));
    view_chat = GTK_WIDGET(gtk_builder_get_object(builder, "view_chat"));
    chat_store = GTK_TREE_STORE(gtk_builder_get_object(builder, "chat_store"));
    tree_view_chat_room = GTK_TREE_VIEW(gtk_builder_get_object(builder, "tree_view_chat_room"));
    column_user = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "column_user"));
    chat_content = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "chat_content"));
    selection = GTK_TREE_SELECTION(gtk_builder_get_object(builder, "selection"));

    gtk_label_set_text(GTK_LABEL(label_roomID), roomid);

    gtk_tree_view_column_add_attribute(column_user, chat_content, "text", 0);

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view_chat_room));
    gtk_widget_show(window_chat);
    

    pthread_create(&recv_msg_thread, NULL, (void *) recv_msg, NULL);
}

void on_entry_message_changed(GtkEntry *e) {
    sprintf(mess, "%s", gtk_entry_get_text(e));
}

void on_button_send_message_clicked() {
    add_to_list(mess);
    char buffer[LENGTH + 32] = {};
    if(strcmp(mess, "exit") == 0) {
        on_window_chat_destroy();
    } else {
        sprintf(buffer, "%s: %s\n", username, mess);
        send(sockfd, buffer, strlen(buffer), 0);
    }
    
    bzero(mess, LENGTH);
    bzero(buffer, LENGTH + 32);
    gtk_entry_set_text(GTK_ENTRY(entry_message),"");
    
}

