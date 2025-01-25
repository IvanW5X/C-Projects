/**************************************
 * File Name: movies.c
 * Date: 10/2/2024
 * File Description: Function definitions for
 *                   movies structure
 * Author(s): Ivan Wong
 **************************************/

#include "movies.h"

struct LinkedList* createLang(char* curLine) {
    char* token;
    char* saveptr;
    char* newLang;
    struct LinkedList* languages = (struct LinkedList*) malloc(sizeof(struct LinkedList));

    initList(languages);

    //Initial token from curLine
    token = strtok_r(curLine, ";", &saveptr);

    //Keep grabbing tokens
    while (token != NULL) {

        //Remove brackets
        if (token && *token == '[')
            token++;
        
        if (token[strlen(token) - 1] == ']')
            token[strlen(token) - 1] = '\0';

        //Allocate space for newLang string, copy from token, add to lang list
        newLang = calloc(strlen(token) + 1, sizeof(char));
        strcpy(newLang, token);
        addNode(languages, newLang);
        token = strtok_r(NULL, ";", &saveptr);
    }
    return languages;
}

struct Movie* createMovie(char* curLine) {
    struct Movie* newMovie = (struct Movie*) malloc(sizeof(struct Movie));
    char* saveptr;
    char* token;

    //Get title
    token = strtok_r(curLine, ",", &saveptr);
    newMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(newMovie->title, token);

    //Get year
    token = strtok_r(NULL, ",", &saveptr);
    newMovie->year = atoi(token);

    //Get languages list string (non-formatted)
    token = strtok_r(NULL, ",", &saveptr);
    
    //Get languages as a list
    newMovie->languages = createLang(token);

    //Get rating
    token = strtok_r(NULL, "\n", &saveptr);
    newMovie->rating = atof(token);

    return newMovie;
}

void printMovies(struct LinkedList* m) {
    assert(m != NULL);

    if (m->head == NULL)
        return;

    struct Node* temp = m->head;
    struct Movie* movie = NULL;

    //Traverse down list
    while (temp != NULL) {
        movie = (struct Movie*) temp->val;
        printf("%d %.1f %s\n", movie->year, movie->rating, movie->title);
        temp = temp->next;
    }
    return;
}

void freeMovie(struct Movie* m) {
    assert(m);

    //Free all pointers that used space on HEAP
    free(m->title);
    freeList(m->languages);
    free(m->languages);
    free(m);
}

void freeMovies(struct LinkedList* l) {
    assert(l);

    if (l->head == NULL)
        return;
    
    struct Node* cur = l->head;
    struct Node* prev = NULL;

    while (cur != NULL) {
        prev = cur;
        cur = cur->next;

        freeMovie(prev->val);
        free(prev);
    }
    l->length = 0;

    return;
}

void displayOptions() {
    printf("\n1. Show movies released in specified year.\n");
    printf("2. Show highest rated movie for each year.\n");
    printf("3. Show the title and year of release of all movies in a specific language.\n");
    printf("4. Exit program\n");
    printf("\nEnter 1 to 4: ");

    return;
}

void processOptions(struct LinkedList* m, int userOption) {
    assert(m);

    switch (userOption) {
        case 1:
            moviesInYear(m);
            break;
        case 2:
            moviesHighestRated(m);
            break;
        case 3:
            moviesInLanguage(m);
            break;
        case 4:
            break;
        default:
            printf("\nInvalid Option, Try Again\n");
    }
    return;
}

void processFile(struct LinkedList* m, const char* fileName) {
    assert(m);

    //Open file
    FILE* fileptr = fopen(fileName, "r");

    char* curLine = 0;
    size_t len = 0;
    ssize_t nread;

    //Get lines from file until EOF
    while ((nread = getline(&curLine, &len, fileptr)) != -1) {

        //Add movies to list
        struct Movie* newMovie = createMovie(curLine);
        addNode(m, (struct Movie*) newMovie);
    }
    free(curLine);
    fclose(fileptr);

    return;
}

void moviesInYear(struct LinkedList* m) {
    assert(m);

    struct Node* temp = m->head;
    struct Movie* movie = NULL;
    int input = 0;
    char flag = '0';

    printf("Enter the year for which you want to see movies: ");
    scanf("%d", &input);

    //Traverse movies list
    while (temp != NULL) {
        movie = (struct Movie*) temp->val;
        if (movie->year == input) {
            printf("%d %.1f %s\n", movie->year, movie->rating, movie->title);
            flag = '1';
        }
        temp = temp->next;
    }
    if (flag == '0')
        printf("No data about movies released in the year %d\n", input);

    return;
}

int containsYear(struct LinkedList* l, int year) {
    assert(l);

    struct Node* curNode = l->head;
    int curYear;

    //Traverse years list
    while (curNode != NULL) {
        curYear = *(int*) curNode->val;

        if (curYear == year)
            return 1;

        curNode = curNode->next;
    }
    return 0;
}

void moviesHighestRated(struct LinkedList* m) {
    assert(m);

    struct Node* curNode = m->head;
    struct Node* passingNode = NULL;
    struct Movie* curMovie = NULL;
    struct Movie* passingMovie = NULL;
    struct Movie* highestRatedMovie = NULL;

    //Create to keep track of years checked
    struct LinkedList* checkedYears = (struct LinkedList*) malloc(sizeof(struct LinkedList));
    int* addYear = NULL;

    initList(checkedYears);
    
    //Traverse movies list
    while (curNode != NULL) {
        passingNode = curNode->next;
        curMovie = (struct Movie*) curNode->val;

        //Year of movie not seen yet
        if (containsYear(checkedYears, curMovie->year) == 0) {

            //Add year to list
            addYear = (int*) malloc(sizeof(int));
            *addYear = curMovie->year;
            addNode(checkedYears, addYear);
            highestRatedMovie = (struct Movie*) curNode->val;

            //Traverse down list with the checking/passing node
            while (passingNode != NULL) {
                passingMovie = (struct Movie*) passingNode->val;

                //Higher rated movie found
                if ((passingMovie->year == curMovie->year) && 
                   (passingMovie->rating > highestRatedMovie->rating))
                    highestRatedMovie = passingMovie;

                passingNode = passingNode->next;
            }
            printf("%d %.1f %s\n", highestRatedMovie->year, highestRatedMovie->rating, highestRatedMovie->title);
        }
        //Year already seen, skip movie
        else {
            curNode = passingNode;
        }
    }
    freeList(checkedYears);
    free(checkedYears);

    return;
}

void moviesInLanguage(struct LinkedList* m) {
    assert(m);

    struct Node* temp = m->head;
    struct Movie* movie = NULL;
    struct Node* tempLang = NULL;
    char* comparedLang;
    char input[20];
    char flag = '0';

    printf("Enter the language for which you want to see movies: ");
    scanf("%s", input);

    //Traverse movies list
    while (temp != NULL) {
        movie = (struct Movie*) temp->val;
        tempLang = movie->languages->head;

        //Traverse down languages list of each movie
        while (tempLang != NULL) {
            comparedLang = (char*) tempLang->val;

            //Matching language found
            if (strcmp(comparedLang, input) == 0) {
                printf("%d %s\n", movie->year, movie->title);
                flag = '1';
            }
            tempLang = tempLang->next;
        }
        temp = temp->next;
    }
    if (flag == '0')
        printf("No data about movies released in %s\n", input);

    return;
}