default: smallsh

list.o: list.h list.c
	gcc -Wall -Wextra -c list.c

smallsh.o: macros.h smallsh.h list.h smallsh.c
	gcc -Wall -Wextra -c smallsh.c

smallsh: list.o smallsh.o main.c
	gcc -Wall -Wextra -o smallsh list.o smallsh.o main.c

clean:
	rm -f *.o smallsh
