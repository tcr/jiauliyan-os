#include <common.h>
#include <stdlib.h>
#include <stream.h>
#include <system.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <vga.h>

/*
 * utility
 */

int atoi(const char *nptr) {
	int i, sum;

	for (sum = 0, i = 0; isdigit(nptr[i]); i++) {
		sum *= 10;
		sum += nptr[i] - '0';
	}

	return sum;
}

/*
 * K&R-style malloc
 */

typedef struct header {
	struct header *next;  	/* next block if on free list */
	unsigned int size;		/* size of this block in bytes */
} Header __attribute__ ((aligned));

#define BLOCKSIZ		sizeof(Header)
#define BLOCK_ALIGN(B)	((B+BLOCKSIZ-1)/BLOCKSIZ) + 1
#define BLOCK_PTR(B)	((void *) B+1)
#define PTR_BLOCK(B)	(((Header *) B)-1)

#define MEMLOC			((char *) 0x02000000)		/* memory start */
#define ALLOCSIZ		1024*1024*256/BLOCKSIZ		/* memory size in blocks */

static Header base; /* root block */
static Header *freep = NULL; /* start of free block list */

void* malloc(size_t nbytes)
{
	if (nbytes == 0)
		return NULL;
	
	/* round number of units up to block size */
    size_t nunits = BLOCK_ALIGN(nbytes);

    Header *prevp;
    if ((prevp = freep) == NULL) {
		/* initialize free segment */
		Header *e = (Header *) MEMLOC;
		e->next = freep = prevp = &base; e->size = ALLOCSIZ;
		base.next = e; base.size = 0;
    }
    
    Header *p;
    for (p = prevp->next; ; prevp = p, p = p->next) {
		/* check block size */
		if (p->size >= nunits) {
			if (p->size == nunits) {
				/* exact */
				prevp->next = p->next;
			} else {
				/* allocate tail end */
				p->size -= nunits;
				p += p->size;		
				p->size = nunits;
			}
			
			/* move free pointer */
			freep = prevp;
			return BLOCK_PTR(p);
		}
		
		if (p == freep) {
			/* wrapped around free list; no blocks left */
			return NULL;
		}
    }
}

/* free: put block ap in free list */

void free(void *ap)
{
    /* check */
	if (ap == NULL)
		return;
		
    Header *bp = PTR_BLOCK(ap), *p;
    
    /* free blocks at start or end of arena */
    for (p = freep; !(bp > p && bp < p->next); p = p->next)
		if (p >= p->next && (bp > p || bp < p->next))
			break;
			
    if (bp + bp->size == p->next) {
		/* join to upper nbr */
		bp->size += p->next->size;
		bp->next = p->next->next;
    } else
		bp->next = p->next;
		
    if (p + p->size == bp) {
		/* join to lower nbr */
		p->size += bp->size;
		p->next = bp->next;
    } else
		p->next = bp;
	
	/* move free list head */
    freep = p;
}

/* realloc */

void *realloc(void *ptr, size_t size)
{
	void *newptr = malloc(size);
	
	if (ptr != NULL) {
		Header *bp = PTR_BLOCK(ptr);		/* point to block header */
		unsigned int bsize = (bp->size - 1) * BLOCKSIZ;
		unsigned int dsize = size > bsize ? bsize : size;
		memcpy(newptr, ptr, dsize);
		memset(newptr + dsize, 0, size - dsize);
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
