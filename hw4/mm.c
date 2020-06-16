#include "mm.h"

static block *free_list;

void *mymalloc(size_t size)
{
    block *prev, *current;
    block *newblock;

    if (!free_list)
    {
        newblock = (block *)sbrk(0);

        void *request = sbrk(size + sizeof(block));
        if (request == (void *)-1)
            return NULL; //malloc is fail

        newblock->next = NULL;
        newblock->free = 0;
        newblock->size = size;
        free_list = newblock;

        return (void *)(++newblock); //fixed
    }

    current = free_list;
    while ((current != NULL && current->free == 0) || (current->size < size))
    {
        prev = current;
        current = current->next;
        if (current == NULL)
            break;
    }
    if (!current)
    {
        current = (block *)sbrk(0);

        void *request = sbrk(size + sizeof(block));
        if (request == (void *)-1)
            return NULL; //malloc is fail

        current->next = NULL;
        current->free = 0;
        current->size = size;
        prev->next = current;
    }

    else if (current->size > size + sizeof(block))
    {
        block *new = (void *)((void *)current + size + sizeof(block));
        new->size = current->size - (size + sizeof(block));
        new->free = 1;
        new->next = current->next;
        current->size = size;
        current->free = 0;
        current->next = new;
    }

    return (void *)(++current);
}

void merge()
{
    block *current = free_list;
    while (current && current->next)
    {
        if (current->free && current->next->free)
        {
            current->size += current->next->size + sizeof(block);
            current->next = current->next->next;
        }
        current = current->next;
    }
}

void myfree(void *ptr)
{
    if (likely(ptr))
    {
        block *current = ptr;
        current--;
        current->free = 1;

        merge();
    }
}

void *myrealloc(void *ptr, size_t size)
{
    if (!ptr)
        return mymalloc(size);

    void *newptr = mymalloc(size);
    memcpy(newptr, ptr, ((block *)(ptr - sizeof(block)))->size);
    myfree(ptr);

    return newptr;
}

void *mycalloc(size_t nmemb, size_t size)
{
    size_t all = nmemb * size;
    void *newptr = mymalloc(all);
    if (newptr)
        memset(newptr, 0, nmemb);       //fixed

    return newptr;
}
