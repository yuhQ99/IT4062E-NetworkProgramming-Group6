#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "linkedlist.h"

roomNode_t* readRoomList (roomNode_t* head, char* filePath);
int saveRoom (roomNode_t* head, char* filePath);
accNode_t* readAccountList (accNode_t* head, char* filePath);
int saveAcc (accNode_t* head, char* filePath);

#endif // !CONTROLLER_H
