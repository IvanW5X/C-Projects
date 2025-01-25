/**************************************
 * File Name: main.c
 * Date: 10/2/2024
 * File Description: Driver/main file for
 *                   Movies assignment
 * Author(s): Ivan Wong
 **************************************/

#include "movies.h"

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        printf("\nYou must provide the name of the file to process\n");
        printf("Example Usage: main movies_example.txt\n");

        return EXIT_FAILURE;
    }
    FILE* fileptr = fopen(argv[1], "r");

    if (fileptr == NULL) {
        printf("\nFailed to open file, exiting program...\n");

        return EXIT_FAILURE;
    }
    int userOption = 0;
    struct LinkedList* Movies = (struct LinkedList*) malloc(sizeof(struct LinkedList));

    initList(Movies);
    processFile(Movies, argv[1]);
    fclose(fileptr);

    printf("\nProcessed file %s and parsed data for %d movies\n", argv[1], Movies->length);

    //Main menu interface
    do {
        displayOptions();
        scanf("%d", &userOption);
        processOptions(Movies, userOption);
    } while (userOption != QUIT);

    freeMovies(Movies);
    free(Movies);

    return EXIT_SUCCESS;
}