/*******************************************************
 * File Name:
 * Date: 1/24/2025
 * Description: Main/driver file for library catalog
 *              program
 * Author(s): Ivan Wong
 *******************************************************/


#include <stdio.h>
#include <stdlib.h>


#include "hashmap.h"

int main(int argc, char** argv) {

    FILE* book_data = fopen("book.csv", "r");

    if (book_data == NULL) {
        fprintf(stdout, "Error opening book data file, exiting...\n");
        return EXIT_FAILURE;
    }


    HashMap* test;

    test = initialize_map();

    free_map(test);    

    fclose(book_data);

    return EXIT_SUCCESS;
}