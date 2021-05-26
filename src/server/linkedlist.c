#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

accNode_t* createAccNode (account_t* acc) {
    accNode_t* p = (accNode_t*) malloc(sizeof(accNode_t));
    p->acc = acc;
    p->next = NULL;

    return p;
}

accNode_t* createAccLinkedList () {
    accNode_t* head = (accNode_t*) malloc(sizeof(accNode_t));
    head->acc = NULL;
    head->next = NULL;

    return head;
}

accNode_t* insertAccNode (accNode_t* head, accNode_t* inode) {
    accNode_t* i = head;
    while (i->next != NULL) {
        i = i->next;
    }
    i->next = inode;

    return inode;
}

accNode_t* searchAccount (accNode_t* head, char* name) {
    accNode_t* i = head->next;
    while (i != NULL) {
        if (i->acc != NULL && !strcasecmp(i->acc->name, name)) {
            return i;
        }
        i = i->next;
    }

    return NULL;
}

void freeAccNode (accNode_t* p) {
    free(p->acc);
    free(p);
}

void freeLinkedList (accNode_t* head) {
    accNode_t* i = head;
    while (i != NULL) {
        accNode_t* p = i->next;
        freeAccNode(i);
        i = p;
    }
}

int removeAccount (accNode_t* head, const char* name) {
    accNode_t* i = head;
    while(i->next != NULL) {
        if (!strcmp(i->next->acc->name, name)) {
            accNode_t* p = i->next->next;
            freeAccNode(i->next);
            i->next = p;
            return 0;
        }
        i = i->next;
    }

    return 1;
}

roomNode_t* createRoomNode (room_t* room) {
    roomNode_t* p = (roomNode_t*) malloc(sizeof(roomNode_t));
    p->room = room;
    p->next = NULL;

    return p;
}

roomNode_t* createRoomLinkedList () {
    roomNode_t* head = (roomNode_t*) malloc(sizeof(roomNode_t));
    head->room = NULL;
    head->next = NULL;

    return head;
}

roomNode_t* insertRoomNode (roomNode_t* head, roomNode_t* inode) {
    roomNode_t* i = head;
    while (i->next != NULL) {
        i = i->next;
    }
    i->next = inode;

    return inode;
}

roomNode_t* searchRoom (roomNode_t* head, int id) {
    roomNode_t* i = head->next;
    while (i != NULL) {
        if (i->room != NULL && (i->room->roomID == id)) {
            return i;
        }
        i = i->next;
    }

    return NULL;
}

void freeRoomNode (roomNode_t* p) {
    free(p->room);
    free(p);
}

int removeRoom (roomNode_t* head, const int id) {
    roomNode_t* i = head;
    while(i->next != NULL) {
        if (i->next->room->roomID == id) {
            roomNode_t* p = i->next->next;
            freeRoomNode(i->next);
            i->next = p;
            return 0;
        }
        i = i->next;
    }

    return 1;
}

int lastRoomID (roomNode_t* head) {
    int tmp = 0;
    roomNode_t* i = head;
    while (i->next != NULL) {
        if(i->room != NULL) {
            tmp = i->room->roomID;
        }
        i = i->next;
    }
    
    return tmp;
}
