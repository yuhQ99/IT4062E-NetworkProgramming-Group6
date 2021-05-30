#include "common.h"
#include "callback.h"
#include "account.h"
#include "room.h"
#include "windowchat.h"

char roomID[10];
char room_choice[3];
char noti[100];

void room_init() {
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "../../glade/homescreen.glade", NULL);

    window_room_choice = GTK_WIDGET(gtk_builder_get_object(builder, "window_room_choice"));
    gtk_builder_connect_signals(builder, NULL);

    button_create_room = GTK_WIDGET(gtk_builder_get_object(builder, "button_create_room"));
    entry_roomID_join = GTK_WIDGET(gtk_builder_get_object(builder, "entry_roomID_join"));
    window_room_create = GTK_WIDGET(gtk_builder_get_object(builder, "window_room_create"));
    alert_dialog = GTK_WIDGET(gtk_builder_get_object(builder, "alert_dialog"));
    button_close = GTK_WIDGET(gtk_builder_get_object(builder, "button_close"));
    label_alert = GTK_WIDGET(gtk_builder_get_object(builder, "label_alert"));
    
    gtk_widget_show(window_room_choice);
}

void on_entry_roomID_join_changed(GtkEntry *e) {
    sprintf(roomID, "%s", gtk_entry_get_text(e));
}

void on_button_create_room_clicked() {
    gtk_widget_show(window_room_create);
}

void on_button_join_room_clicked() {
    strcpy(room_choice, "3");
    send(sockfd, room_choice, 3, 0);
    send(sockfd, roomID, 10, 0);

    int receive = recv(sockfd, noti, 100, 0);
    if (receive > 0) {
		noti[receive] = 0;
        str_trim_lf(noti, strlen(noti));
	}
	

	if(strstr(noti, "Max Client in this room!") != NULL || strstr(noti, "not found") != NULL) {
		gtk_entry_set_text(GTK_ENTRY(entry_roomID_join),"");
        gtk_label_set_text(GTK_LABEL(label_alert), noti);
        gtk_widget_show(alert_dialog);
	}
    else {
        // gtk_label_set_text(GTK_LABEL(label_roomID), roomID);
        gtk_widget_hide(window_room_choice);
        // gtk_widget_show(window_chat);
        window_chat_init(roomID);
    }
	
}

void on_button_chatbox_clicked() {
    strcpy(room_choice, "1");
}

void on_button_chatroom_clicked() {
    strcpy(room_choice, "2");
}

void on_button_room_create_confirm_clicked() {
    send(sockfd, room_choice, 3, 0);
    
    int receive = recv(sockfd, noti, 100, 0);
    if (receive > 0) {
		noti[receive] = 0;
        str_trim_lf(noti, strlen(noti));
      	printf("%s\n", noti);
	}
    // gtk_label_set_text(GTK_LABEL(label_roomID), noti);
    gtk_widget_hide(window_room_create);
    gtk_widget_hide(window_room_choice);
    // gtk_widget_show(window_chat);
    window_chat_init(noti);

}

void on_button_close_clicked() {
    gtk_widget_hide(alert_dialog);
}