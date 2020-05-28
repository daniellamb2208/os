all:	test dep

test:	mm.o test.c
	gcc -Wall -w -o test mm.o test.c

mm.o:	mm.c mm.h
	gcc -c -Wall -w mm.c mm.h

dep:
	gcc -M *.c > dep

clean:
	rm test dep *.o *.gch
