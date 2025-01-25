/**************************************
 * File Name: smallsh.c
 * Date: 10/23/2024
 * File Description: Function definitions for
 *                   Smallsh assignment
 * Author(s): Ivan Wong
 **************************************/


#include "smallsh.h"


// Use Global Ptr to Shell State
ShellState* accessShellState;


int selfCd(char** args) {
    //No args provided past cd, redirect to HOME
    if (args[1] == NULL) {
        chdir(getenv("HOME"));
    }
    //Run chdir(), print error message if failed
    else if (chdir(args[1]) != 0) {
        fprintf(stdout, "Error changing directory in selfCd()\n");
        fflush(stdout);
    }
    return 0;
}


int selfExit(char* input, ShellState* shellState) {
    killProcesses(shellState->backgroundProcesses);

    //Free dynamic memory on heap
    free(input);
    freeShellStateMemory(shellState);

    fprintf(stdout, "Exiting shell program...\n\n");
    fflush(stdout);

    exit(EXIT_SUCCESS);
}


int selfStatus(int status) {
    //Print exit status
    if (status < 2) {
        fprintf(stdout, "Exit value: %d\n", status);
        fflush(stdout);
    }
    //Last process was terminated, print signal
    else {
        fprintf(stdout, "Terminated by signal: %d\n", status);
        fflush(stdout);
    }
    return 0;
}


char* expandProcessIdVar(char* input) {
    char* subInput = strstr(input, "$$");
    char* newInput = NULL;
    char pidString[32];

    //Instance of $$ not found in input
    if (subInput == NULL) {
        return input;
    }
    //Split string at instance of $$ and remove it
    input[strlen(input) - strlen(subInput)] = '\0';
    subInput += 2;

    //Get the pid as a string
    sprintf(pidString, "%d", getpid());

    //Allocate more memory for pid and create new string
    newInput = (char*) calloc(strlen(input) + strlen(pidString) + strlen(subInput) + 1, sizeof(char));

    //Check memory allocation
    if (newInput == NULL) {
        fprintf(stdout, "Error, allocating memory in expandProcessIdVar()\n");
        fflush(stdout);
        return NULL;
    }
    sprintf(newInput, "%s%s%s", input, pidString, subInput);
    free(input);

    //Look for another instance of $$ in new string
    return expandProcessIdVar(newInput);
}


char* readInput() {
    char* input = (char*) calloc(BUFFER_SIZE, sizeof(char));

    //Check memory allocation
    if (input == NULL) {
        fprintf(stdout, "Error allocating memory in readInput()\n");
        fflush(stdout);
        return NULL;
    }
    //Get input and error check
    if (fgets(input, BUFFER_SIZE, stdin) == NULL) {
        free(input);
        return NULL;
    }
    //Replace new line with null char
    input[strcspn(input, "\n")] = '\0';

    //Find instances of $$ and replace
    return expandProcessIdVar(input);
}


char** parseInput(char* input) {
    //Check for valid input
    if (input == NULL)
        return NULL;

    char** tokens = (char**) calloc(MAX_ARGS, sizeof(char*));
    char* saveptr = NULL;
    char* token = NULL;
    int i = 0;

    //Check memory allocation
    if (tokens == NULL) {
        fprintf(stdout, "Error allocating memory in parseInput()\n");
        fflush(stdout);
        return NULL;
    }
    //Get first argument
    token = strtok_r(input, " ", &saveptr);

    while (token != NULL) {
        //Add argument to array
        tokens[i] = token;
        i++;

        //Check if input exeeds max args
        if (i > MAX_ARGS) {
            fprintf(stdout, "Error, too many arguments\n");
            fflush(stdout);
            free(tokens);
            return NULL;
        }
        //Get next token
        token = strtok_r(NULL, " ", &saveptr);
    }
    //Set end of array to NULL for execvp
    tokens[i] = NULL;
    return tokens;
}


void killProcesses(List* backgroundProcesses) {
    Node* temp = backgroundProcesses->head;

    //Go through each process ID and terminate them
    while (temp != NULL) {
        kill(temp->val, SIGTERM);
        waitpid(temp->val, NULL, 0);
        temp = temp->next;
    }
    return;
}


void shiftLeft(char** args, int index) {
    if (args == NULL)
        return;

    //Shift all args on the right of index, left one index
    while (args[index] != NULL) {
        args[index] = args[index + 1];
        index++;
    }
    return;
}


bool specificFileRedirection (char* fileName, char* flag) {
    if (fileName == NULL)
        return false;

    int fdOutput = 0;
    int fdInput = 0;

    //Redirect input file
    if (strcmp(flag, IN) == 0) {
        //Set input to fileName
        fdInput = open(fileName, O_RDONLY);

        //Error handle opening file
        if (fdInput == -1) {
            fprintf(stdout, "Failed to open file for input redirection in specificFileRedirection()\n");
            fflush(stdout);
            return false;
        }
        //Redirect input to fileName
        dup2(fdInput, STDIN_FILENO);
        close(fdInput);
    }
    //Redirect output file
    else if (strcmp(flag, OUT) == 0) {
        //Set output to fileName
        fdOutput = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, RW_R_R);

        //Error handle opening file
        if (fdOutput == -1) {
            fprintf(stdout, "Failed to open file for output redirection in specificFileRedirection()\n");
            fflush(stdout);
            return false;
        }
        //Redirect output to fileName
        dup2(fdOutput, STDOUT_FILENO);
        close(fdOutput);
    }
    return true;
}


bool fileRedirection(char** args) {
    if (args == NULL)
        return false;

    bool success = true;
    bool flag = false;
    int i = 0;

    //Look for I/O commands
    while (args[i] != NULL) {
        //IN string found and file name provided
        if ((strcmp(args[i], IN) == 0) && (args[i + 1] != NULL)) {
            //Redirect file input
            success = specificFileRedirection(args[i + 1], IN);

            //Reformat args
            args[i] = NULL;
            shiftLeft(args, i);
            flag = true;
        }
        //OUT string found and file name provided
        else if ((strcmp(args[i], OUT) == 0) && (args[i + 1] != NULL)) {
            //Redirect file output
            success = specificFileRedirection(args[i + 1], OUT);

            //Reformat args
            args[i] = NULL;
            shiftLeft(args, i);
            flag = true;
        }
        i++;
    }
    //Background process with no specific file redirection
    if ((strcmp(args[i - 1], BACKGROUND_PROCESS) == 0) && (flag == false)) {
        //Set file I/O to /dev/null
        specificFileRedirection("/dev/null", IN);
        specificFileRedirection("/dev/null", OUT);

        //Reformat args
        args[i - 1] = NULL;
    }
    return success;
}


int foregroundProcess(char* input, ShellState* shellState) {
    if (shellState->args == NULL)
        return 0;

    char** commands = shellState->args;
    bool success = true;
    int childStatus = 0;

    pid_t pid = fork();

    switch (pid) {
        //Forking failed
        case -1:
            fprintf(stdout, "Error forking process in foregroundProcess()\n");
            fflush(stdout);
            return 1;
        //Run commands in child process
        case 0:
            //Child uses default sigaction for SIGINT
            if (setupSignalHandling(SIGINT, SIG_DFL) == -1) {
                //Free memory before exiting
                free(input);
                freeShellStateMemory(shellState);

                exit(EXIT_FAILURE);
            }
            //Child ignores SIGTSTP
            if (setupSignalHandling(SIGTSTP, SIG_IGN) == -1) {
                //Free memory before exiting
                free(input);
                freeShellStateMemory(shellState);

                exit(EXIT_FAILURE);
            }
            //Check for file redirection
            success = fileRedirection(commands);

            //Execute user commands and error check
            if (success == false  || execvp(commands[0], commands) == -1) {
                fprintf(stdout, "%s: no such file found\n", commands[0]);
                fflush(stdout);

                //Free memory before exiting
                free(input);
                freeShellStateMemory(shellState);

                exit(EXIT_FAILURE);
            }
            break;
        default:
            //Wait for child to be killed, save it's status
            pid = waitpid(pid, &childStatus, 0);

            //Get child process exit status
            if (WIFEXITED(childStatus))
                shellState->lastStatus = WEXITSTATUS(childStatus);
            
            //Get terminating signal
            else if (WIFSIGNALED(childStatus)) {
                //Print terminating signal
                fprintf(stdout, "Foreground process terminated with signal: %d\n", WTERMSIG(childStatus));
                fflush(stdout);
                shellState->lastStatus = WTERMSIG(childStatus);
            }
            break;
    }
    return 0;
}


int backgroundProcess(char* input, ShellState* shellState) {
    if (shellState->args == NULL)
        return 0;

    char** commands = shellState->args;
    bool success = true;

    pid_t pid = fork();

    switch (pid) {
        //Forking failed
        case -1:
            fprintf(stdout, "Error forking process in backgroundProcess()\n");
            fflush(stdout);
            return 1;
        //Run commands in child process
        case 0:
            //Child ignores SIGTSTP
            if (setupSignalHandling(SIGTSTP, SIG_IGN) == -1) {
                //Free memory before exiting
                free(input);
                freeShellStateMemory(shellState);

                exit(EXIT_FAILURE);
            }
            //Redirect file I/O
            success = fileRedirection(commands);

            //Execute user commands and error check
            if (success == false || execvp(commands[0], commands) == -1) {
                fprintf(stdout, "%s: no such file found\n", commands[0]);
                fflush(stdout);

                //Free memory before exiting
                free(input);
                freeShellStateMemory(shellState);

                exit(EXIT_FAILURE);
            }
            break;
        //Parent process actions
        default:
            fprintf(stdout, "Started background process with PID: %d\n", pid);
            fflush(stdout);

            //Add child PID to background list
            addList(shellState->backgroundProcesses, pid);
            break;
    }
    return 0;
}


int executeArgs(char* input, ShellState* shellState) {
    //No input provided
    if (shellState->args == NULL)
        return 0;

    char** commands = shellState->args;
    int lastArg = 0;

    //Empty line or comment
    if (commands[0] == NULL || commands[0][0] == COMMENT)
        return 0;

    //cd command found, use built in function
    if (strcmp(commands[0], "cd") == 0)
        return selfCd(shellState->args);

    //exit command found, use built in function
    else if (strcmp(commands[0], "exit") == 0)
        return selfExit(input, shellState);

    //status command found, use built in function
    else if (strcmp(commands[0], "status") == 0)
        return selfStatus(shellState->lastStatus);

    //Check for background process argument
    while (commands[lastArg] != NULL)
        lastArg++;

    //Background process command found, run as background process if not in foreground mode
    if (strcmp(commands[lastArg - 1], BACKGROUND_PROCESS) == 0 && shellState->foregroundMode == false)
        return backgroundProcess(input, shellState);

    //No built in functions, &, or foreground mode on, run as foreground process
    return foregroundProcess(input, shellState);
}


void checkBackgroundProcesses(List* backgroundProcesses) {
    if (backgroundProcesses == NULL)
        return;

    Node* temp = backgroundProcesses->head;
    Node* tempNext = NULL;
    int status = 0;
    pid_t result;

    //Check each background PID
    while (temp != NULL) {
        tempNext = temp->next;
        result = waitpid(temp->val, &status, WNOHANG);

        //Process ended, print the ID and exit status, remove node in list
        if (result > 0) {
            fprintf(stdout, "Background PID %d done\n", result);
            selfStatus(status);
            removeList(backgroundProcesses, result);
        }
        temp = tempNext;
    }
    return;
}


void initShellState(ShellState* shellState) {
    //Init shellState vars
    shellState->backgroundProcesses = (List*) malloc(sizeof(List));
    shellState->foregroundMode = false;
    shellState->lastStatus = 0;
    shellState->curStatus = 0;
    shellState->args = NULL;

    //Check memory allocation
    if (shellState->backgroundProcesses == NULL) {
        fprintf(stdout, "Error allocating memory at initShellState()\n");
        fflush(stdout);
        exit(EXIT_FAILURE);
    }
    initList(shellState->backgroundProcesses);
    return;
}


void freeShellStateMemory(ShellState* shellState) {
    //Check if memory exists, if so delete it
    if (shellState->args != NULL)
        free(shellState->args);
    if (shellState->backgroundProcesses != NULL) {
        freeList(shellState->backgroundProcesses);
        free(shellState->backgroundProcesses);
    }
    if (shellState != NULL)
        free(shellState);
    return;
}


void handleSigTSTP() {
    //Error check
    if (accessShellState == NULL)
        return;

    //Turn foreground mode on
    if (accessShellState->foregroundMode == false) {
        accessShellState->foregroundMode = true;
        fprintf(stdout, "Entering foreground-only mode (& is now ignored)\n");
    }
    //Turn foreground mode off
    else {
        accessShellState->foregroundMode = false;
        fprintf(stdout, "Exiting foreground-only mode\n");
    }
    fflush(stdout);
    return;
}


int setupSignalHandling(int signal, void (*handler)(int)) {
    struct sigaction action;

    //Edit action attributes
    action.sa_handler = handler;
    sigfillset(&action.sa_mask);
    action.sa_flags = 0;

    //Modify SIGINT
    if (sigaction(signal, &action, NULL) == -1) {
        fprintf(stdout, "Error modifying signal at sigaction()\n");
        fflush(stdout);
        return -1;
    }
    return 0;
}


void interactShell(ShellState* shellState) {
    char* input = NULL;

    //Shell loop
    do {
        fprintf(stdout, ": ");
        fflush(stdout);

        input = readInput();
        shellState->args = parseInput(input);
        shellState->curStatus = executeArgs(input, shellState);

        //Free user input memory
        if (input != NULL)
            free(input);
        if (shellState->args != NULL)
            free(shellState->args);

        input = NULL;
        shellState->args = NULL;

        checkBackgroundProcesses(shellState->backgroundProcesses);
    } while (shellState->curStatus == 0);
    return;
}
