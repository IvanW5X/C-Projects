/**************************************
 * File Name: smallsh.h
 * Date: 10/23/2024
 * File Description: Function prototypes
 *                   for Smallsh assignment
 * Author(s): Ivan Wong
 **************************************/


#ifndef SMALLSH_H
#define SMALLSH_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <stdbool.h>
#include "macros.h"
#include "list.h"


/*********************
 * Shell State Struct
 *********************/
struct ShellState {
    char** args;
    List* backgroundProcesses;
    int lastStatus;
    int curStatus;
    bool foregroundMode;
};


/********************************
 * Global Pointer to Shell State
 ********************************/
extern ShellState* accessShellState;


/*****************************
 * Built in Command Functions
 *****************************/
int selfCd(char** args);
int selfExit(char* input, ShellState* ShellState);
int selfStatus(int status);


/******************
 * Shell Functions
 ******************/
char* expandProcessIdVar(char* input);
char* readInput();
char** parseInput(char* input);

void killProcesses(List* backgroundProcesses);
void shiftLeft(char** args, int index);
bool specificFileRedirection(char* fileName, char* flag);
bool fileRedirection(char** args);

int foregroundProcess(char* input, ShellState* shellState);
int backgroundProcess(char* input, ShellState* shellState);
int executeArgs(char* input, ShellState* shellState);
void checkBackgroundProcesses(List* backgroundProcesses);

void initShellState(ShellState* shellState);
void freeShellStateMemory(ShellState* shellState);

void handleSigTSTP();


/**************************
 * Shell Program Functions
 **************************/
int setupSignalHandling(int signal, void (*handler)(int));
void interactShell(ShellState* shellState);

#endif
