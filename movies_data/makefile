default: movies

linked_list.o: structs.h linked_list.h linked_list.c
	gcc -Wall -c linked_list.c

movies.o: linked_list.h movies.h movies.c
	gcc -Wall -c movies.c

movies: linked_list.o movies.o main.c
	gcc -Wall -o movies linked_list.o movies.o main.c

clean:
	rm -f *.o movies