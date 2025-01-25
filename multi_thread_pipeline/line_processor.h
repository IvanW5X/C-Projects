/******************************************
 * File Name: line_processor.h
 * Description: Header file for Line Processor
 * Date: 11/22/2024
 * Author(s): Ivan Wong
 ******************************************/


#ifndef LINE_PROCESSOR_H
#define LINE_PROCESSOR_H


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include "macros.h"


/*********************
 * Buffer Data Struct
 *********************/
struct BufferData {
    char* buffer;
    int size;
    int capacity;
    int producer_idx;
    int consumer_idx;
    pthread_mutex_t lock;
    pthread_cond_t can_produce;
    pthread_cond_t can_consume;
};


/*******************
 * Buffer Functions
 *******************/
void init_buffer_data(BufferData* bd, int capacity);

void free_buffer_data(BufferData* bd);

char* get_one_line();

void add_to_buffer(BufferData* bd, char item);

char get_from_buffer(BufferData* bd);

/***************************
 * Line Processor Functions
 ***************************/
void* get_input(void* args);

void* line_separator(void* args);

void* find_plus_signs(void* args);

void* output_line(void* args);

void run_line_process();


#endif
