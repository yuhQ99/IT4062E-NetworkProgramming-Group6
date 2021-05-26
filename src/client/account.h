#ifndef ACCOUNT_H
#define ACCOUNT_H
#include "common.h"

void str_trim_lf (char* arr, int length);
void login_init(char* choice);

void on_entry_username_changed(GtkEntry *e);

void on_entry_password_changed(GtkEntry *e);

void on_button_account_confirm_clicked();

#endif // !ACCOUNT_H