#ifndef __MY_MM_H_INCLUDED__
#define __MY_MM_H_INCLUDED__

#include <stddef.h>
#include <unistd.h>

#define likely(_x) __builtin_expect(!!(_x), 1)

typedef struct _block
{
    size_t size;
    int free;
    struct _block *next;
} block;

void *mymalloc(size_t size);
void myfree(void *ptr);
void *myrealloc(void *ptr, size_t size);
void *mycalloc(size_t nmemb, size_t size);

#endif