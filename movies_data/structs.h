/**************************************
 * File Name: structs.h
 * Date: 10/2/2024
 * File Description: Header file for
 *                   structs used
 * Author(s): Ivan Wong
 **************************************/

#ifndef STRUCTS_H
#define STRUCTS_H

#ifndef QUIT
#define QUIT 4
#endif 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct Node {
    void* val;
    struct Node* next;
};

struct LinkedList {
    struct Node* head;
    int length;
};

struct Movie {
    char* title;
    struct LinkedList* languages;
    int year;
    float rating;
};

#endif