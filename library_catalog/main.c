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

    HashMap* test;

    test = initialize_map();

    free_map(test);    

    return EXIT_SUCCESS;
}