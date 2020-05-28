all:	test dep

test:	mm.o test.c
	gcc -o test mm.o test.c

mm.o:	mm.c mm.h
	gcc -c mm.c

dep:
	gcc -M *.c > dep

clean:
	rm test dep *.o
