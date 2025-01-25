/**************************************
 * File Name: main.c
 * Date: 10/23/2024
 * File Description: Driver/main file for
 *                   Smallsh assignment
 * Author(s): Ivan Wong
 **************************************/


#include "smallsh.h"


int main() {
    fprintf(stdout, "\nThis is assignment 3. SmallSH.\n\n");
    fflush(stdout);

    ShellState* shellState = (ShellState*) malloc(sizeof(ShellState));

    //Check memory allocation
    if (shellState == NULL) {
        fprintf(stdout, "Error allocating memory in main()\n");
        fflush(stdout);
        exit(EXIT_FAILURE);
    }
    //Setup shell state
    initShellState(shellState);
    accessShellState = shellState;

    //Setup signal handlers
    if (setupSignalHandling(SIGINT, SIG_IGN) == -1 || 
        setupSignalHandling(SIGTSTP, handleSigTSTP) == -1) {
        freeShellStateMemory(shellState);
        exit(EXIT_FAILURE);
    }

    //Run shell program
    interactShell(shellState);
    freeShellStateMemory(shellState);

    return EXIT_SUCCESS;
}
