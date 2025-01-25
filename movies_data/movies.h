/**************************************
 * File Name: movies.h
 * Date: 10/2/2024
 * File Description: Header file for 
 *                   movies structure
 * Author(s): Ivan Wong
 **************************************/

#ifndef MOVIES_H
#define MOVIES_H

#include "linked_list.h"

/******************
 * Movies Functions
 ******************/
struct LinkedList* createLang(char* curLine);
struct Movie* createMovie(char* curLine);
void printMovies(struct LinkedList* m);
void freeMovie(struct Movie* m);
void freeMovies(struct LinkedList* l);

/*******************
 * Program Functions
 *******************/
void displayOptions();
void processOptions(struct LinkedList* m, int userOption);
void processFile(struct LinkedList* m, const char* fileName);

void moviesInYear(struct LinkedList* m);
int containsYear(struct LinkedList* l, int year);
void moviesHighestRated(struct LinkedList* m);
void moviesInLanguage(struct LinkedList* m);

#endif