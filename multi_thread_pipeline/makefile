CC=gcc
CFLAGS=-Wall -Wextra
EXE_FILE=line_processor

all: $(EXE_FILE)

$(EXE_FILE): line_processor.o main.o
	$(CC) $(CFLAGS) line_processor.o main.o -o $(EXE_FILE)

line_processor.o: macros.h line_processor.h line_processor.c
	$(CC) $(CFLAGS) -c line_processor.c

main.o: line_processor.h main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o $(EXE_FILE)