# Library Catalog

## Overview

This is a library catalog C program that simulates a library management system. Using the C programming language, the user is able to...

* Add book information
* Display book information
* List all books of a given author
* List count of books
* List all books and authors
* Provide a sorted list of books based on either the title or author in alphabetical order. Choice selected by the user
* Remove a book given the title

The design of the library catalog program will include complex features, such as implementing my own data structures to store data, file I/O, multi-threading, dual-design mode, sorting algoritms, and an optimized hashing function. To simply put it, the library catalog is simply a host to practice more complex, Computer Science concepts. The motivation of creating such a program is to practice and hone my programming skills in low level programming languages and franky, will a lot of fun to create such a beefy program that has simple task functionality.

## Technical Approach

The library catalog will be implemented using object oriented paradigms and file pointers to store book data from previous sessions of the program. A library will contain books and will only manage routing the books for the user at a high level, so that the book object will maintain its own functionality of being displayed and created using the books.txt file with a file pointer. Additionally, the catalog will be implemented with scalability in mind, so if there were to be many books in the txt file, they shall be stored in a hashmap that I will be implementing myself to demonstrate knowledge of data structures.

On top of implementing simple library functionality, I will also include optimization techniques, multi-threading, and more complex hashing functionlaities to display and practice skills in memory management, concurrency, thread safety, and algorithms that I've covered in univerity and from independent research, making this an absurdly secure and optimized library catalog.

I will also implement a duel-mode design, so that users are able to quickly use a library function in the command line when first booting up the program. Utitlizing the argument vector, I will be able to parse the arguments and count the number of arguments provided to allow users to use a single function of the library catalog program, so that is no arguments are provided, they are loaded into the program loop until they exit.

### Hashmap Functionality

### Book Functionality

### Library Functionality

## Contributers

* Ivan Wong
