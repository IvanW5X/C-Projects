/*******************************************************
 * File Name:
 * Date: 1/24/2025
 * Description: 
 * Author(s): Ivan Wong
 *******************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include <math.h>
#include <stdbool.h>

#include "hashmap.h"


size_t HASH(KEY_TYPE key) {

}


HashMap* initialize_map() {
    HashMap* hash_map = (HashMap*) malloc(sizeof(HashMap));

    // Memory allocation failed
    if (hash_map == NULL) return NULL;

    // Initialize data
    hash_map->size = 0;
    hash_map->capacity = INIT_CAP;
    hash_map->table = (HashNode**) malloc(sizeof(HashNode*) * INIT_CAP);

    for (size_t i = 0; i < INIT_CAP; i++)
        hash_map->table[i] = NULL;

    return hash_map;
}


