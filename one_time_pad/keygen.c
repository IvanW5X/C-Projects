/***********************************************
 * File Name: keygen.c
 * Date: 12/7/2024
 * File Description: Creates a key file of a
 *                   specified length provided
 *                   from the command line
 * Author(s): Ivan Wong
 ***********************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


// Error handle
void error(const char* message) {
    fprintf(stderr, "%s\n", message);
    fflush(stderr);
    exit(EXIT_SUCCESS);
}


// Helper Function
char generate_random_character() {
    int random_number = rand() % 27;

    // Map last 26 to a space char
    if (random_number == 26)
        return ' ';
    else
        return ('A' + random_number);
}


int main(int argc, char** argv) {
    // Check for key length provided
    if (argc < 2)
        error("Error: No length provided");

    char* key = NULL;
    int key_length = 0;

    srand(time(NULL));

    // Need key length greater than 0
    if ((key_length = atoi(argv[1])) < 1)
        error("Error: Length needs to be greater than 0");

    key = (char*) calloc(key_length + 1, sizeof(char));

    // Generate random key
    int i = 0;
    for (i = 0; i < key_length; i++) {
        key[i] = generate_random_character();
    }
    key[key_length] = '\n';

    // Output to stdout stream
    fprintf(stdout, "%s", key);
    fflush(stdout);

    return 0;
}