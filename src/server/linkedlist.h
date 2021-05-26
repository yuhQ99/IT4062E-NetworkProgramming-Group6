#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#define NAMELENGTH 32
#define PWDLENGTH 20

typedef struct {
    char name[NAMELENGTH];
    char pwd[PWDLENGTH];
    int status;
} account_t;

typedef struct NodeAcc {
    account_t* acc;
    struct NodeAcc* next;
} accNode_t;

typedef struct {
    int roomID;
    char hostname[NAMELENGTH];
    int roomMaxClient;
    int memberCount;
} room_t;

typedef struct NodeRoom
{
    room_t* room;
    struct NodeRoom* next;
} roomNode_t;

accNode_t* createAccNode (account_t* acc);

accNode_t* createAccLinkedList ();

accNode_t* insertAccNode (accNode_t* head, accNode_t* inode);

accNode_t* searchAccount (accNode_t* head, char* name);

void freeAccNode (accNode_t* p);

void freeLinkedList (accNode_t* head);

int removeAccount (accNode_t* head, const char* usr);

roomNode_t* createRoomNode (room_t* room);

roomNode_t* createRoomLinkedList ();

roomNode_t* insertRoomNode (roomNode_t* head, roomNode_t* inode);

roomNode_t* searchRoom (roomNode_t* head, const int id);

void freeRoomNode (roomNode_t* p);

int removeRoom (roomNode_t* head, const int id);

int lastRoomID (roomNode_t* head);

#endif // !LINKEDLIST_H