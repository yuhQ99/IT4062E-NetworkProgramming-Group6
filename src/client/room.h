#ifndef ROON_H
#define ROOM_H

#include "common.h"

void room_init();
void on_entry_roomID_join_changed(GtkEntry *e);
void on_button_create_room_clicked();
void on_button_join_room_clicked();
void on_button_chatbox_clicked();
void on_button_chatroom_clicked();
void on_button_room_create_confirm_clicked();
void on_button_close_clicked();
#endif
