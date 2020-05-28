#ifndef __MY_MM_H_INCLUDED__
#define __MY_MM_H_INCLUDED__

#include <stddef.h>
#include <string.h>

#define NALLOC 1024		// minimum #units to request

typedef long Align;		// for 

union header {	// block header
	struct {
		union header *ptr;	// next block if on free list
		size_t size;		// size of this block
	} s;
	Align x;	// force aligment of blocks (align to long)
};				// to simplify alignment, all blocks are multiples of the header size, and the header is aligned properly

typedef union header Header;

void *mymalloc(size_t size);
void myfree(void *ptr);
void *myrealloc(void *ptr, size_t size);
void *mycalloc(size_t nmemb, size_t size);

#endif // !MM_H
