#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include "controller.h"
#include "linkedlist.h"

roomNode_t* readRoomList (roomNode_t* head, char* filePath) {
    FILE* fp = fopen(filePath, "r");
    if (fp != NULL) {
        while(!feof(fp)) {
            room_t* room = (room_t*) malloc(sizeof(room_t));
            fscanf(fp, "%d %s %d", &room->roomID, room->hostname, &room->roomMaxClient);
            room->memberCount = 0;
            roomNode_t *node = createRoomNode (room);
            insertRoomNode(head, node);
        }
        fclose(fp);
    } else {
        char err[100];
        sprintf(err, "Cannot open %s for reading ", filePath);
        perror(err);
    }

    return head;
}

int saveRoom (roomNode_t* head, char* filePath) {
    FILE* fp = fopen(filePath, "a");
    int ret = 0;

    if (fp != NULL) {
        roomNode_t* i = head;
        while(i->next != NULL) {
           i=i->next; 
        }
        if(i->room != NULL)
            fprintf(fp, "%d %s %d\n", i->room->roomID, i->room->hostname, i->room->roomMaxClient);
    } else {
        char err[100];
        sprintf(err, "Cannot open %s for writing ", filePath);
        perror(err);
        ret = -1;
    }
    
    fclose(fp);
    return ret;
}

accNode_t* readAccountList (accNode_t* head, char* filePath) {
    FILE* fp = fopen(filePath, "r");
    if (fp != NULL) {
        while(!feof(fp)) {
            account_t* acc = (account_t*) malloc(sizeof(account_t));
            fscanf(fp, "%s %s", acc->name, acc->pwd);
            acc->status = 0;
            accNode_t *node = createAccNode(acc);
            insertAccNode(head, node);
        }
        fclose(fp);
    } else {
        char err[100];
        sprintf(err, "Cannot open %s for reading ", filePath);
        perror(err);
    }

    return head;
}

int saveAcc (accNode_t* head, char* filePath) {
    FILE* fp = fopen(filePath, "a");
    int ret = 0;

    if (fp != NULL) {
        accNode_t* i = head;
        while(i->next != NULL) {
           i=i->next; 
        }
        if(i->acc != NULL)
            fprintf(fp, "%s %s\n", i->acc->name, i->acc->pwd);
    } else {
        char err[100];
        sprintf(err, "Cannot open %s for writing ", filePath);
        perror(err);
        ret = -1;
    }
    
    fclose(fp);
    return ret;
}