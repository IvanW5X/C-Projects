/*******************************************************
 * File Name: book.h
 * Date: 1/24/2025
 * Description: Header file for book structure and it
 *              specific functions
 * Author(s): Ivan Wong
 *******************************************************/


#pragma once

struct Book {
    char* title;
    char* author;
    int publication_year;
    char* genre;
    int copies;
};
typedef struct Book Book;


