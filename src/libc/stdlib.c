#include <common.h>
#include <stdlib.h>
#include <stream.h>
#include <system.h>
#include <stdio.h>
#include <string.h>

#include <vga.h>

/*
 * K&R malloc
 */

typedef long Align;		/* for alignment to long boundary */

union header {			/* block header */
    struct {
		union header	*ptr;  	/* next block if on free list */
		unsigned 	size;	/* size of this block */
    } s;
    Align x;			/* force alignment of blocks */
};

typedef union header Header;

static Header *morecore(unsigned nu);

static Header base;		/* empty list to get started */
static Header *freep = NULL;	/* start of free list */

void* malloc(unsigned nbytes) {
    Header *p, *prevp;		
    Header *morecore(unsigned);
    unsigned nunits;

    nunits = (nbytes+sizeof(Header)-1)/sizeof(Header) + 1;

    if ((prevp = freep) == NULL) {	/* no free list yet */
		base.s.ptr = freep = prevp = &base;		base.s.size = 0;
    }
    for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
		if (p->s.size >= nunits) {	/* big enough */
			if (p->s.size == nunits) { 	/* exactly */
				prevp->s.ptr = p->s.ptr;
			} else {			/* allocate tail end */
				p->s.size -= nunits;
				p += p->s.size;		
				p->s.size = nunits;
			}
			freep = prevp;
			return (void*) (p+1);
		}
		if ( p == freep) {		/* wrapped around free list */
			if ((p = morecore(nunits)) == NULL)
				return NULL; /* none left */
		}
    }
}

#define NALLOC 	1024*1024*256/sizeof(Header)	/* minimum #units to request	*/

static int first = 1;

char *sbrk(int nbytes) {
	UNUSED(nbytes);
	if (first == 0) {
		puts("\nREQUESTING MEMORY WHERE THERE IS NONE\n");
		return NULL;
	}
	first--;
	return (char *) 0x10000000;
}

/* morecore: ask system for memory */
static Header *morecore(unsigned nu) {
    char *cp, *sbrk(int);
    void free(void*);
    Header *up;
    if (nu < NALLOC)
		nu = NALLOC;
    cp = sbrk(nu * sizeof(Header));
    if (cp == (char *) -1)		/* no space at all */
		return NULL;
    up = (Header*) cp;
    up->s.size = nu;
    free((void*)(up+1));
    return freep;
}

/* free: put block ap in free list */
void free(void *ap)
{
    Header *bp, *p;
    
	if (ap == NULL)
		return;
    
    bp = (Header*) ap - 1;		/* point to block header */
    for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
		if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
			break;	/* freed block at start or end of arena */
			
    if (bp + bp->s.size == p->s.ptr) {	/* join to upper nbr */
		bp->s.size += p->s.ptr->s.size;
		bp->s.ptr = p->s.ptr->s.ptr;
    } else
		bp->s.ptr = p->s.ptr;
    if ( p + p->s.size == bp ) {		/* join to lower nbr */
		p->s.size += bp->s.size;
		p->s.ptr = bp->s.ptr;
    } else
		p->s.ptr = bp;
    freep = p;
}

/*
 * realloc
 */

void *realloc(void *ptr, size_t size)
{
	void *newptr = NULL;
	if (size > 0)
		newptr = malloc(size);
	if (ptr != NULL) {
		if (size > 0) {
			Header *bp = (Header*) ptr - 1;		/* point to block header */
			unsigned int bsize = (bp->s.size - 1) * sizeof(Header);
			unsigned int dsize = size > bsize ? bsize : size;
			memcpy(newptr, ptr, dsize);
			memset(newptr + dsize, 0, size - dsize);
		}
		free(ptr);
	}
	return newptr;
}

/*
 * math functions
 */

int abs(int value)
{
	return value < 0 ? -value : value;
}

int rand(void)
{
	return 7;
}

void srand(unsigned int seed)
{
	(void) seed;
}

	        
	    
			 
