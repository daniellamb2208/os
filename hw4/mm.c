#include "mm.h"

static Header base;			 // empty list to get started
static Header *freep = NULL; // start of free list
							 // if freep is NULL, it is at the first call of mymalloc

static Header *morecore(size_t nu)
{ // ask system for more memory
	char *cp, *sbrk(int);
	Header *up;

	if (nu < NALLOC)				// requests at least NALLOC units
		nu = NALLOC;				// this larger block will be chopped up as needed
	cp = sbrk(nu * sizeof(Header)); // sbrk(n) returns a pointer to n more bytes of storage, return -1 if no more space
	if (cp == (char *)-1)			// no space at all (-1 must be cast to char * so it can be compared with the return value)
		return NULL;
	up = (Header *)cp;
	up->s.size = nu;		  // setting the size field
	myfree((void *)(up + 1)); // inserts the additional memory into the arena by clling free
	return freep;
}

void *mymalloc(size_t size)
{
	Header *p, *prevp;
	Header *morecore(size_t);
	size_t nunits;

	nunits = (size + sizeof(Header) - 1) / sizeof(Header) + 1; // alignment
	if ((prevp = freep) == NULL)
	{										// no free list yet, a degenerate free list will be created
		base.s.size = 0;					// degenerate free list contain one block of size zero
		base.s.ptr = freep = prevp = &base; // and pointer to itself
	}
	for (p = prevp->s.ptr;; prevp = p, p = p->s.ptr)
	{ // find for the next allocated block that have a size bigger than nunits
		if (p->s.size >= nunits)
		{								 // big enough
			if (p->s.size == nunits)	 // exactly
				prevp->s.ptr = p->s.ptr; // remove p from the list
			else
			{						 // allocate tail end
				p->s.size -= nunits; // cut the memory
				p += p->s.size;
				p->s.size = nunits;
			}
			freep = prevp;
			return (void *)(p + 1);
		}
		if (p == freep) // wrapped around free list
			if ((p = morecore(nunits)) == NULL)
				return NULL; // none left
	}
}

void myfree(void *ptr) // put block ap in free list
{
	Header *bp, *p;

	bp = (Header *)ptr - 1;									  // point to block header
	for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr) // scans the free list, starting at freep
		if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))		  // looking for the place to insert the free block
			break;
	// either between two exisiting blocks or at one end of the list
	// if block being freed is adjacent to either neighbor, the adjacent blocks are combined
	if (bp + bp->s.size == p->s.ptr)
	{ // join to upper nbr
		bp->s.size += p->s.ptr->s.size;
		bp->s.ptr = p->s.ptr->s.ptr; // point to the next block
	}
	else
		bp->s.ptr = p->s.ptr;
	if (p + p->s.size == bp)
	{ // join to lower nbr
		p->s.size += bp->s.size;
		p->s.ptr = bp->s.ptr;
	}
	else
		p->s.ptr = bp; // point the next block
	freep = p;
}

void *myrealloc(void *ptr, size_t size)
{
	if (ptr == NULL) // NULL ptr. realloc should act like malloc.
		return mymalloc(sizeof(size));
	if (size == 0)
	{
		myfree(ptr);
		return NULL;
	}
	Header *p = (Header *)ptr - 1; // point to the header
	size_t n = (p->s.size - 1) * sizeof(Header);
	void *temp = mymalloc(size); // Allocate a new bloc of the given size using malloc
	if (n < size)
		memcpy(temp, ptr, size); // Copy data from the old one to the new one
	else
		memcpy(temp, ptr, size);
	myfree(ptr); // Free the old block
	return temp; // return the new pointer
}

void *mycalloc(size_t nmemb, size_t size)
{
	void *ptr = mymalloc(nmemb * size); // use malloc to get the memory
	if (ptr != NULL)
	{ // clear the memory on successful alloction
		memset(ptr, 0, nmemb * size);
	}
	return ptr;
}
