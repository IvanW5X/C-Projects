/******************************************
 * File Name: line_processor.c
 * Description: Function definitions for Line
 *              Processor
 * Date: 11/22/2024
 * Author(s): Ivan Wong
 ******************************************/


#include "line_processor.h"


/*****************
 * Define Globals
 *****************/
BufferData input_buffer;
BufferData parse_data_buffer;
BufferData output_buffer;


/*****************************
 * Buffer Function Defnitions
 *****************************/
void init_buffer_data(BufferData* bd, int capacity) {
    // Allocate memory buffer data
    bd->buffer = (char*) calloc(capacity, sizeof(char));

    // Check memory allocation
    if (bd->buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    // Initialize buffer data variables
    bd->size = 0;
    bd->capacity = capacity;
    bd->producer_idx = 0;
    bd->consumer_idx = 0;

    pthread_mutex_init(&bd->lock, NULL);
    pthread_cond_init(&bd->can_produce, NULL);
    pthread_cond_init(&bd->can_consume, NULL);

    return;
}


void free_buffer_data(BufferData* bd) {
    if (bd == NULL)
        return;

    // Free memory and destroy pthread stuff
    if (bd->buffer) free(bd->buffer);

    pthread_mutex_destroy(&bd->lock);
    pthread_cond_destroy(&bd->can_produce);
    pthread_cond_destroy(&bd->can_consume);
}


char* get_one_line() {
    char* temp_buffer = (char*) calloc(BUFFER_SIZE, sizeof(char));

    // Check memory allocation
    if (temp_buffer == NULL) {
        fprintf(stdout, "Memory allocation failed\n");
        fflush(stdout);
        return NULL;
    }
    // Get line and return
    if (fgets(temp_buffer, BUFFER_SIZE, stdin) == NULL) {
        free(temp_buffer);
        return NULL;
    }
    return temp_buffer;
}


void add_to_buffer(BufferData* bd, char item) {
    pthread_mutex_lock(&bd->lock);

    // Wait until data available
    while (bd->size == bd->capacity)
        pthread_cond_wait(&bd->can_produce, &bd->lock);

    // Add char in circular fashion
    bd->buffer[bd->producer_idx] = item;
    bd->producer_idx = (bd->producer_idx + 1) % bd->capacity;
    bd->size++;

    // Send not empty signal and unlock
    pthread_cond_signal(&bd->can_consume);
    pthread_mutex_unlock(&bd->lock);
    return;
}

char get_from_buffer(BufferData* bd) {
    char temp;

    pthread_mutex_lock(&bd->lock);

    // Empty, wait for data
    while (bd->size == 0)
        pthread_cond_wait(&bd->can_consume, &bd->lock);

    // Remove char in circular fashion
    temp = bd->buffer[bd->consumer_idx];
    bd->consumer_idx = (bd->consumer_idx + 1) % bd->capacity;
    bd->size--;

    // Send not full signal and unlock
    pthread_cond_signal(&bd->can_produce);
    pthread_mutex_unlock(&bd->lock);
    return temp;
}


/***************************
 * Line Processor Functions
 ***************************/
void* get_input(void* args) {
    char* input;
    int length;
    int i;

    while (1) {
        // Get line from STDIN or other file
        input = get_one_line();

        // STOP found, send STOP char through and break
        if (input != NULL && strcmp(input, STOP) == 0) {
            add_to_buffer(&input_buffer, STOP_CHAR);
            free(input);
            break;
        }
        // EOF from file, send STOP char through and break
        else if (input == NULL) {
            add_to_buffer(&input_buffer, STOP_CHAR);
            free(input);
            break;
        }
        length = strlen(input);

        // Add to buffer char by char
        for (i = 0; i < length; i++)
            add_to_buffer(&input_buffer, input[i]);

        free(input);
    }
    return NULL;
}


void* line_separator(void* args) {
    char temp;

    while (1) {
        temp = get_from_buffer(&input_buffer);

        // STOP char found, break
        if (temp == STOP_CHAR) {
            add_to_buffer(&parse_data_buffer, STOP_CHAR);
            break;
        }
        // Replace new line with char
        if (temp == '\n') temp = ' ';

        add_to_buffer(&parse_data_buffer, temp);
    }
    return NULL;
}


void* find_plus_signs(void* args) {
    char first;
    char second;

    while (1) {
        first = get_from_buffer(&parse_data_buffer);

        // STOP char found, send through and break
        if (first == STOP_CHAR) {
            add_to_buffer(&output_buffer, STOP_CHAR);
            break;
        }
        // Check for ++ pair, if found add ^
        else if (first == '+') {
            second = get_from_buffer(&parse_data_buffer);

            if (second == '+')
                add_to_buffer(&output_buffer, '^');

            else {
                add_to_buffer(&output_buffer, first);
                add_to_buffer(&output_buffer, second);
            }
        }
        // Not found, add normal char found
        else
            add_to_buffer(&output_buffer, first);
    }
    return NULL;
}


void* output_line(void* args) {
    char line[OUTPUT_BUFFER_SIZE];
    char temp;
    int length = 0;

    while (1) {
        // 80 char processed, print
        if (length == OUTPUT_BUFFER_SIZE) {
            line[OUTPUT_BUFFER_SIZE] = '\0';

            fprintf(stdout, "%s\n", line);
            fflush(stdout);

            // Reset length and go again
            length = 0;
        }
        temp = get_from_buffer(&output_buffer);

        // STOP char found, break
        if (temp == STOP_CHAR)
            break;

        // Add char to line
        line[length] = temp;
        length++;
    }
    return NULL;
}

void run_line_process() {
    pthread_t th[NUM_THREADS];
    int i;

    // Initialize buffer data
    init_buffer_data(&input_buffer, BUFFER_SIZE);
    init_buffer_data(&parse_data_buffer, BUFFER_SIZE);
    init_buffer_data(&output_buffer, OUTPUT_BUFFER_SIZE);

    // Create threads
    pthread_create(&th[0], NULL, get_input, NULL);
    pthread_create(&th[1], NULL, line_separator, NULL);
    pthread_create(&th[2], NULL, find_plus_signs, NULL);
    pthread_create(&th[3], NULL, output_line, NULL);

    // Join threads
    for (i = 0; i < NUM_THREADS; i++)
        pthread_join(th[i], NULL);

    // Free buffer data memory
    free_buffer_data(&input_buffer);
    free_buffer_data(&parse_data_buffer);
    free_buffer_data(&output_buffer);

    return;
}