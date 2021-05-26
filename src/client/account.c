#include "common.h"
#include "account.h"
#include "callback.h"
#include "room.h"


char password[20];
char c[3];
char noti[LENGTH];

void str_trim_lf (char* arr, int length) {
  	int i;
  	for (i = 0; i < length; i++) { // trim \n
    	if (arr[i] == '\n') {
      		arr[i] = '\0';
      		break;
    	}
  	}
}

void login_init(char* choice) {
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "../../glade/homescreen.glade", NULL);

    window_account = GTK_WIDGET(gtk_builder_get_object(builder, "window_account"));
    gtk_builder_connect_signals(builder, NULL);

    label_account_mode = GTK_WIDGET(gtk_builder_get_object(builder, "label_account_mode"));
    entry_username = GTK_WIDGET(gtk_builder_get_object(builder, "entry_username"));
    button_account_confirm = GTK_WIDGET(gtk_builder_get_object(builder, "button_account_confirm"));
    label_account_noti = GTK_WIDGET(gtk_builder_get_object(builder, "label_account_noti"));
	entry_password = GTK_WIDGET(gtk_builder_get_object(builder, "entry_password"));
	gtk_label_set_text(GTK_LABEL(label_account_mode), choice);
    if(strcmp(choice, "Sign in") == 0) {
        strcpy(c, "1");
    }
    else {
        strcpy(c, "2");
    }
    gtk_widget_show(window_account);
}

void connect_server(char* user, char*pwd) {
    char *ip = "127.0.0.1";
	int port = 5000;

    struct sockaddr_in server_addr;

	/* Socket settings */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
  	server_addr.sin_family = AF_INET;
  	server_addr.sin_addr.s_addr = inet_addr(ip);
  	server_addr.sin_port = htons(port);


  	// Connect to Server
  	int err = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  	if (err == -1) {
		printf("ERROR: connect\n");
		gtk_main_quit();
	}

    send(sockfd, c, 3, 0);

	// Send name
	send(sockfd, user, 32, 0);
	
	// send password
	send(sockfd, pwd, 20, 0);

    int receive = recv(sockfd, noti, 100, 0);
    if (receive > 0) {
		noti[receive] = 0;
        str_trim_lf(noti, strlen(noti));
	}
	
	if(strstr(noti, "Account are active in this app.") != NULL || strstr(noti, "Password does not correct.") != NULL || strstr(noti, "User does not exist, please sign up.") != NULL || atoi(c) == 2) {
		gtk_label_set_text(GTK_LABEL(label_account_noti), noti);
		gtk_entry_set_text(GTK_ENTRY(entry_username),"");
		gtk_entry_set_text(GTK_ENTRY(entry_password),"");
		
		if(strstr(noti, "Sign up successfully! Please submit username and password again to login.") != NULL) {
			gtk_label_set_text(GTK_LABEL(label_account_mode), "Sign in");
			strcpy(c, "1");
		}

		if(strstr(noti, "User does not exist, please sign up.") != NULL) {
			gtk_label_set_text(GTK_LABEL(label_account_mode), "Sign up");
			strcpy(c, "2");
		}
	} else {
		gtk_widget_hide(window_account);
    	room_init();
	}
    
}

void on_entry_username_changed(GtkEntry *e) {
    sprintf(username, "%s", gtk_entry_get_text(e));
}

void on_entry_password_changed(GtkEntry *e) {
    sprintf(password, "%s", gtk_entry_get_text(e));
}

void on_button_account_confirm_clicked() {
    connect_server(username, password);
}