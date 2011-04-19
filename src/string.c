#include <string.h>

/*
 * copying
 */
 
void *memcpy(void *dest, const void *src, size_t count)
{
	unsigned int i;
	for (i = 0; i < count; i++) {
		((unsigned char *) dest)[i] = ((unsigned char *) src)[i];
	}
	return dest;
}

void *memmove(void *dest, const void *src, size_t count)
{
	/* http://www.student.cs.uwaterloo.ca/~cs350/common/os161-src-html/memmove_8c-source.html */
	size_t i;
 
	/*
	 * If the buffers don't overlap, it doesn't matter what direction
	 * we copy in. If they do, it does, so just assume they always do.
	 * We don't concern ourselves with the possibility that the region
	 * to copy might roll over across the top of memory, because it's
	 * not going to happen.
	 *
	 * If the destination is above the source, we have to copy
	 * back to front to avoid overwriting the data we want to
	 * copy.
	 *
	 *      dest:       dddddddd
	 *      src:    ssssssss   ^
	 *              |   ^  |___|
	 *              |___|
	 *
	 * If the destination is below the source, we have to copy
	 * front to back.
	 *
	 *      dest:   dddddddd
	 *      src:    ^   ssssssss
	 *              |___|  ^   |
	 *                     |___|
	 */
 
	if ((uintptr_t)dest < (uintptr_t)src) {
		/*
		 * As author/maintainer of libc, take advantage of the
		 * fact that we know memcpy copies forwards.
		 */
		return memcpy(dest, src, count);
	}
 
	/*
	 * Copy by words in the common case. Look in memcpy.c for more
	 * information.
	 */
 
	if ((uintptr_t)dest % sizeof(long) == 0 &&
	    (uintptr_t)src % sizeof(long) == 0 &&
	    count % sizeof(long) == 0) {
 
		long *d = dest;
		const long *s = src;
 
		/*
		 * The reason we copy index i-1 and test i>0 is that
		 * i is unsigned - so testing i>=0 doesn't work.
		 */
 
		for (i=count/sizeof(long); i>0; i--) {
			d[i-1] = s[i-1];
		}
	}
	else {
		char *d = dest;
		const char *s = src;
 
		for (i=count; i>0; i--) {
			d[i-1] = s[i-1];
		}
	}
 
	return dst;
}

char *strcpy(char *dest, const char *source)
{
	int i = 0;
	while(source[i] != '\0') {
		dest[i] = source[i];
		i++;
	}
	dest[i+1] = '\n';
	return dest;
}

char *strncpy(char *dest, const char *source, size_t n)
{
	unsigned int i = 0;
	while(i < n) {
		dest[i] = source[i];
		i++;
	}
	dest[i+1] = '\n';
	return dest;
}

/*
 * concatenation
 */

char *strcat(char *str1, const char *str2)
{
	strcpy(str1 + strlen(str1), str2);
	return str1;
}

char *strncat(char *str1, const char *str2, size_t n)
{
	strncpy(str1 + strlen(str1), str2, n);
	return str1;
}

/*
 * comparison
 */

int memcmp(const void *str1, const void *str2, size_t n)
{
	//[TODO]
}

int strcmp(const char *a, const char *b)
{
	signed int i, val, len1 = strlen(a), len2 = strlen(b);
	for (i = 0; i < len1 && i < len2; i++) {
		if ((val = a[i] - b[i]) != 0)
			return val;
	}
	if (i == len1 && i < len2)
		return 0 - b[i];
	else if (i == len1 && i < len2)
		return a[i] - 0;
	else
		return 0;
}

int strcoll(const char *str1, const char *str2)
{
	//[TODO]
}

int strncmp(const char *str1, const char *str2, size_t n)
{
	//[TODO]
}

size_t strxfrm(char *str1, const char *str2, size_t n)
{
	//[TODO]
}


/*
 * searching
 */

void *memchr(const void *str, int c, size_t n)
{
	//[TODO]
}

char *strchr(const char *str, int c)
{
	//[TODO]
}

size_t strcspn(const char *str1, const char *str2)
{
	//[TODO]
}

char *strpbrk(const char *str1, const char *str2)
{
	//[TODO]
}

char *strrchr(const char *str, int c)
{
	//[TODO]
}

size_t strspn(const char *str1, const char *str2)
{
	//[TODO]
}

char *strstr(const char *str1, const char *str2)
{
	//[TODO]
}

char *strtok(char *str1, const char *str2)
{
	//[TODO]
}


/*
 * other
 */

void *memset(void *dest, int val, int count)
{
	/* Add code here to set 'count' bytes in 'dest' to 'val'.
	 *  Again, return 'dest' */
	int i = 0;
	for (i = 0; i < count; i++) {
		((unsigned char *) dest)[i] = (unsigned char) val;
	}
	return dest;
}

void *memsetw(void *dest, short int val, int count)
{
	/* Same as above, but this time, we're working with a 16-bit
	 *  'val' and dest pointer. Your code can be an exact copy of
	 *  the above, provided that your local variables if any, are
	 *  unsigned short */
	
	unsigned short i = 0;
	for (i = 0; i < count; i++) {
		((unsigned short *) dest)[i] = (unsigned short) val;
	}
	return dest;
}

char *strerror(int errnum)
{
	//[TODO]
}

int strlen(const char *str)
{
    /* This loops through character array 'str', returning how
    *  many characters it needs to check before it finds a 0.
    *  In simple words, it returns the length in bytes of a string */
    int i = 0;
    while(str[i] != '\0'){
		i++;
	}
	return i;

}
