/**************************************
 * File Name: linked_list.h
 * Date: 10/2/2024
 * File Description: Header file for a
 *                   a linked list
 * Author(s): Ivan Wong
 **************************************/

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "structs.h"

/***********************
 * Linked List Functions
 ***********************/
void initList(struct LinkedList* l);
void addNode(struct LinkedList* l, void* e);
void freeList(struct LinkedList* l);

#endif