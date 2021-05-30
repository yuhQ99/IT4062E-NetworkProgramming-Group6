#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include "linkedlist.h"
#include "controller.h"

#define MAX_CLIENTS 100
#define BUFFER_SZ 1000
char* roomFilePath = "../../data/room.txt";
char* accountFilePath = "../../data/account.txt";

static _Atomic unsigned int cli_count = 0;
static int uid = 10;
roomNode_t* roomHead;
accNode_t* accHead;

int rid;

typedef struct{
	struct sockaddr_in address;
	int sockfd;
	int uid;
	char name[32];
	int room;
} client_t;

client_t *clients[MAX_CLIENTS];

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void str_overwrite_stdout() {
    printf("\r%s", "> ");
    fflush(stdout);
}

void str_trim_lf (char* arr, int length) {
  int i;
  for (i = 0; i < length; i++) { // trim \n
    if (arr[i] == '\n') {
      arr[i] = '\0';
      break;
    }
  }
}

void print_client_addr(struct sockaddr_in addr){
    printf("%d.%d.%d.%d",
        addr.sin_addr.s_addr & 0xff,
        (addr.sin_addr.s_addr & 0xff00) >> 8,
        (addr.sin_addr.s_addr & 0xff0000) >> 16,
        (addr.sin_addr.s_addr & 0xff000000) >> 24);
}

void queue_add(client_t *cl){
	pthread_mutex_lock(&clients_mutex);

	for(int i=0; i < MAX_CLIENTS; ++i){
		if(!clients[i]){ 
			clients[i] = cl;
			break;
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}


void queue_remove(int uid){
	pthread_mutex_lock(&clients_mutex);

	for(int i=0; i < MAX_CLIENTS; ++i){
		if(clients[i]){
			if(clients[i]->uid == uid){
				clients[i] = NULL;
				break;
			}
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}

/* Send message to all clients except sender */
void send_message(char *s, int uid, int room){
	pthread_mutex_lock(&clients_mutex);

	for(int i=0; i<MAX_CLIENTS; ++i){
		if(clients[i]){
			if(clients[i]->uid != uid && clients[i]->room == room){
				if(write(clients[i]->sockfd, s, strlen(s)) < 0){
					perror("ERROR: write to descriptor failed");
					break;
				}
			}
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}

/* Handle all communication with the client */
void *handle_client(void *arg){
	char buff_out[BUFFER_SZ];
	char acc_choice[3];
	char name[32];
    char password[20];
	char room[10];
    char room_choice[3];
	char noti[100];
	int leave_flag = 0; // check a client connect or error
	int receive; // length of string received from server
	int verify_status = 0;
	cli_count++;
	client_t *cli = (client_t *)arg;
	
	if(recv(cli->sockfd, acc_choice, 3, 0) <= 0) {
		// printf("Didn't enter the choice.\n");
		leave_flag = 1;
	}

	if(!leave_flag) {
		// Name
		if(recv(cli->sockfd, name, 32, 0) <= 0 || strlen(name) <  2 || strlen(name) >= 32-1){
			printf("Didn't enter the name.\n");
			leave_flag = 1;
		}

    	if(recv(cli->sockfd, password, 20, 0) <= 0 || strlen(password) <  2 || strlen(password) >= 20-1){
			printf("Didn't enter the password.\n");
			leave_flag = 1;
		}
	}

	if (!leave_flag) {
		if (atoi(acc_choice) == 1) {
			accNode_t* node = searchAccount(accHead, name);
			if (node != NULL) {
				if (!strcmp(password, node->acc->pwd)) {
					if(node->acc->status == 0) {
						node->acc->status = 1;
						sprintf(noti, "Login successfully!");
						send(cli->sockfd, noti, 100, 0);
					} else {
						sprintf(noti, "Account are active in this app. You can't sign in by this account");
						send(cli->sockfd, noti, 100, 0);
						leave_flag = 1;
					}
				} else {
					sprintf(noti, "Password does not correct.");
					send(cli->sockfd, noti, 100, 0);
					leave_flag = 1;
				}
			} else {
				sprintf(noti, "User does not exist, please sign up.");
				send(cli->sockfd, noti, 100, 0);
				leave_flag = 1;
			}
		} else if (atoi(acc_choice) == 2) {
			accNode_t* node = searchAccount(accHead, name);
			if (node == NULL) {
				account_t* user = (account_t*) malloc(sizeof(account_t));
				strcpy(user->name, name);
				strcpy(user->pwd, password);
				user->status = 0;
				insertAccNode(accHead, createAccNode(user));
				saveAcc(accHead, accountFilePath);
				sprintf(noti, "Sign up successfully! Please submit username and password again to login.");
				send(cli->sockfd, noti, 100, 0);
				leave_flag = 1;
			} else {
				sprintf(noti, "User does exist, please try other username.");
				send(cli->sockfd, noti, 100, 0);
				leave_flag = 1;
			}
		}
	}

	if(!leave_flag) {
		while (1) {
			if(verify_status) {
				break;
			}

			if(recv(cli->sockfd, room_choice, 3, 0) <= 0){
			}

			if(atoi(room_choice) == 3) {
				if(recv(cli->sockfd, room, 10, 0) <= 0){
					sprintf(noti, "Please insert the room ID");
					send(cli->sockfd, noti, 100, 0);
					continue;
				}
			}

			if(atoi(room_choice) == 1) {
				rid = rid + 1;
				room_t* chatbox = (room_t*) malloc(sizeof(room_t));
				chatbox->roomID = rid;
				strcpy(chatbox->hostname, name);
				chatbox->roomMaxClient = 2;
				chatbox->memberCount = 1;
				insertRoomNode(roomHead, createRoomNode (chatbox));
				saveRoom(roomHead, roomFilePath);
				cli->room = chatbox->roomID;
				sprintf(noti, "%d", chatbox->roomID);
				send(cli->sockfd, noti, 100, 0);
				verify_status = 1;
        	}
        	else if(atoi(room_choice) == 2) {
				rid = rid + 1;
				room_t* chatroom = (room_t*) malloc(sizeof(room_t));
				chatroom->roomID = rid;
				strcpy(chatroom->hostname, name);
				chatroom->roomMaxClient = 10;
				chatroom->memberCount = 1;
				insertRoomNode(roomHead, createRoomNode (chatroom));
				saveRoom(roomHead, roomFilePath);
				cli->room = chatroom->roomID;
				sprintf(noti, "%d", cli->room);
				send(cli->sockfd, noti, 100, 0);
				verify_status = 1;
        	}
			else if (atoi(room_choice) == 3) {
				roomNode_t * node = searchRoom (roomHead, atoi(room));
				if(node != NULL) {
					int mem = node->room->memberCount + 1;
					if(mem > node->room->roomMaxClient) {
						sprintf(noti, "Max Client in this room!");
						send(cli->sockfd, noti, 100, 0);
						continue;
					}
					else {
						cli->room = atoi(room);
						node->room->memberCount = node->room->memberCount + 1;
						sprintf(noti, "Join in room %d successfully!", cli->room);
						send(cli->sockfd, noti, 100, 0);
						verify_status = 1;
					}
				}
				else {
					sprintf(noti, "Room %d not found", atoi(room));
					send(cli->sockfd, noti, 30, 0);
					continue;
				}
			}
		}
	}
	
	bzero(buff_out, BUFFER_SZ);

    if(!leave_flag) {
        strcpy(cli->name, name);
		sprintf(buff_out, "%s has joined in room %d\n", cli->name, cli->room); // copy string "%s has joined" to buff_out
		printf("%s", buff_out);
		send_message(buff_out, cli->uid, cli->room); // send message buff_out to all client in room chat
    }

	bzero(buff_out, BUFFER_SZ);

	while(1){
		if (leave_flag) {
			break;
		}

		receive = recv(cli->sockfd, buff_out, BUFFER_SZ, 0);
		if (receive > 0){
			if(strlen(buff_out) > 0){
				send_message(buff_out, cli->uid, cli->room);

				str_trim_lf(buff_out, strlen(buff_out));
				printf("%s -> Room %d\n", buff_out, cli->room);
			}
		} else if (receive == 0 || strcmp(buff_out, "exit") == 0){
			roomNode_t * node = searchRoom (roomHead, cli->room);
			node->room->memberCount = node->room->memberCount - 1;
			accNode_t* nodeAcc = searchAccount(accHead, name);
			nodeAcc->acc->status = 0;
			sprintf(buff_out, "%s has left\n", cli->name);
			printf("(Room %d) %s", cli->room, buff_out);
			send_message(buff_out, cli->uid, cli->room);
			leave_flag = 1;
		} else {
			printf("ERROR: -1\n");
			leave_flag = 1;
		}

		bzero(buff_out, BUFFER_SZ);
	}

  	/* Delete client from queue and yield thread */
	close(cli->sockfd);
  	queue_remove(cli->uid);
  	free(cli);
  	cli_count--;
  	pthread_detach(pthread_self());

	return NULL;
}

int main(int argc, char **argv){
    roomHead = createRoomNode(NULL);
    readRoomList(roomHead, roomFilePath);

	accHead = createAccNode(NULL);
	readAccountList(accHead, accountFilePath);

	if(roomHead-> next != NULL) {
		rid = lastRoomID(roomHead);
	}

	char *ip = "127.0.0.1";
	int port = 5000;
	int option = 1;
	int listenfd = 0, connfd = 0;
  	struct sockaddr_in serv_addr; //describe server socket address
  	struct sockaddr_in cli_addr;
  	pthread_t tid; //thread id

  	/* Socket settings */
  	listenfd = socket(AF_INET, SOCK_STREAM, 0);
  	serv_addr.sin_family = AF_INET; //ipv4
  	serv_addr.sin_addr.s_addr = inet_addr(ip);
  	serv_addr.sin_port = htons(port);

  	/* Ignore pipe signals */
	signal(SIGPIPE, SIG_IGN);

	// setsockopt set socket option
	if(setsockopt(listenfd, SOL_SOCKET,(SO_REUSEPORT | SO_REUSEADDR),(char*)&option,sizeof(option)) < 0){
		perror("ERROR: setsockopt failed");
    return EXIT_FAILURE;
	}

	/* Bind */
  	if(bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    	perror("ERROR: Socket binding failed");
    	return EXIT_FAILURE;
  	}

  	/* Listen */
  	if (listen(listenfd, 10) < 0) {
    	perror("ERROR: Socket listening failed");
    	return EXIT_FAILURE;
	}
	// connect successful
	printf("Server connect successully on port %d\n", port);

	while(1){
		socklen_t clilen = sizeof(cli_addr);
		connfd = accept(listenfd, (struct sockaddr*)&cli_addr, &clilen);

		/* Check if max clients is reached */
		if((cli_count + 1) == MAX_CLIENTS){
			printf("Max clients reached. Rejected: ");
			print_client_addr(cli_addr);
			printf(":%d\n", cli_addr.sin_port);
			close(connfd);
			continue;
		}

		/* Client settings */
		client_t *cli = (client_t *)malloc(sizeof(client_t));
		cli->address = cli_addr;
		cli->sockfd = connfd;
		cli->uid = uid++;

		queue_add(cli);
		pthread_create(&tid, NULL, &handle_client, (void*)cli);

		
		sleep(1);
	}

	return EXIT_SUCCESS;
}
