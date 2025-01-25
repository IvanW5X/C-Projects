/**************************************
 * File Name: list.h
 * Date: 10/23/2024
 * File Description: Header file for a
 *                   linked list
 * Author(s): Ivan Wong
 **************************************/


#ifndef LIST_H
#define LIST_H


#include "macros.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 


struct Node {
    pid_t val;
    Node* next;
};


struct List {
    Node* head;
    int size;
};


void initList();
void addList(List* l, pid_t e);
void removeList(List* l, pid_t e);
void freeList(List* l);


#endif
