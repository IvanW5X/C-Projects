CC=gcc
CFLAGS=-Wall -Wextra -std=c99
EXE_FILE=prog

all: $(EXE_FILE)

$(EXE_FILE): hashmap.o book.o library.o main.o
	$(CC) $(CFLAGS) hashmap.o book.o library.o main.o -o $(EXE_FILE)

hashmap.o: hashmap.h hashmap.c
	$(CC) $(CFLAGS) -c hashmap.c

book.o: book.h book.c
	$(CC) $(CFLAGS) -c book.c

library.o: hashmap.h book.h library.h library.c
	$(CC) $(CFLAGS) -c library.c

main.o: library.h main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o $(EXE_FILE)