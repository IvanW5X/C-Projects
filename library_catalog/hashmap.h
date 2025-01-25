/*******************************************************
 * File Name:
 * Date: 1/24/2025
 * Description: 
 * Author(s): Ivan Wong
 *******************************************************/


#pragma once


#include <stdio.h>
#include <stdlib.h>

#include "book.h"

#define TYPE Book
#define KEY_TYPE const char*
#define INIT_CAP 10

struct HashNode {
    TYPE value;
    KEY_TYPE key;
    struct HashNode* next;
};
typedef struct HashNode HashNode;


struct HashMap{
    HashNode **table;
    size_t size;
    size_t capacity;
};
typedef struct HashMap HashMap;


size_t HASH(KEY_TYPE key);
HashMap* initialize_map();
void free_map(HashMap* map);
