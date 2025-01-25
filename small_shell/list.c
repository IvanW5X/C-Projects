/**************************************
 * File Name: list.c
 * Date: 10/23/2024
 * File Description: Function definitions
 *                   for a linked list
 * Author(s): Ivan Wong
 **************************************/


#include "list.h"


void initList(List* l) {
    if (l == NULL)
        return;

    l->head = NULL;
    l->size = 0;
    return;
}


void addList(List* l, pid_t e) {
    if (l == NULL)
        return;

    Node* newNode = (Node*) malloc(sizeof(Node));

    //Check for successful memory allocation
    if (newNode == NULL) {
        fprintf(stdout, "Error, memory allocation failed at addNode()\n");
        fflush(stdout);
        return;
    }
    //Add node to front of list
    newNode->next = l->head;
    newNode->val = e;
    l->head = newNode;
    l->size++;
    return;
}


void removeList(List* l, pid_t e) {
    if (l == NULL)
        return;

    Node* temp = l->head;
    Node* prev = NULL;

    //Special case, remove first node
    if (temp != NULL && temp->val == e) {
        l->head = temp->next;
        free(temp);
        l->size--;
        return;
    }
    //Search for node
    while (temp != NULL && temp->val != e) {
        prev = temp;
        temp = temp->next;
    }
    //Not found
    if (temp == NULL)
        return;

    //Link list accordingly and delete node
    prev->next = temp->next;
    free(temp);
    l->size--;
    return;
}


void freeList(List* l) {
    if (l == NULL)
        return;
    
    Node* cur = l->head;
    Node* prev = NULL;

    //Free nodes with a passing and prev node
    while (cur != NULL) {
        prev = cur;
        cur = cur->next;
        free(prev);
    }
    l->size = 0;
    return;
}
