all: output part2 dep

output:	output.c
	gcc -o output output.c

part2:	part2.c
	gcc -o part2 part2.c

dep:
	gcc -M *.c > dep

clean:
	rm output part2 dep
