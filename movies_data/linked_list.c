/**************************************
 * File Name: linked_list.c
 * Date: 10/2/2024
 * File Description: Function definitions
 *                   for a linked list
 * Author(s): Ivan Wong
 **************************************/

#include "linked_list.h"
#include "movies.h"

void initList(struct LinkedList* l) {
    assert(l);

    l->head = NULL;
    l->length = 0;

    return;
}

void addNode(struct LinkedList* l, void* e) {
    assert(l);

    struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));
    assert(newNode);

    //Add node to front of list
    newNode->val = e;
    newNode->next = l->head;
    l->head = newNode;
    l->length++;

    return;
}

void freeList(struct LinkedList* l) {
    assert(l);

    if (l->head == NULL)
        return;
    
    struct Node* cur = l->head;
    struct Node* prev = NULL;

    while (cur != NULL) {
        prev = cur;
        cur = cur->next;

        free(prev->val);
        free(prev);
    }
    l->length = 0;

    return;
}