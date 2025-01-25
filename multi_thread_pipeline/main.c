/******************************************
 * File Name: main.c
 * Description: Driver/main file for Multi-
 *              Threaded Producer Consumer 
 *              Pipeline
 * Date: 11/22/2024
 * Author(s): Ivan Wong
 ******************************************/


#include "line_processor.h"

// int argc, char** argv
int main() {
    int check_input = isatty(STDOUT_FILENO);

    if (check_input == 1) {
        fprintf(stdout, "This is assignment 4, Multi-threaded Producer Consumer Pipeline\n\n");
        fflush(stdout);
    }
    run_line_process();

    return EXIT_SUCCESS;
}
