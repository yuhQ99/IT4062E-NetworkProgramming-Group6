#ifndef WINDOWCHAT_H
#define WINDOWCHAT_H

void window_chat_init(char* roomid);
void recv_msg();
void add_to_list(const gchar *str);
#endif // !WINDOWCHAT_H