#include <string.h>
#include <stdint.h>

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
 
	return dest;
}

char *strcpy(char *to, const char *from)
{
	char *save = to;
	for (; (*to = *from) != '\0'; ++from, ++to);
	return save;
}

char *strncpy(char *dst, const char *src, size_t n)
{
	if (n != 0) {
		char *d = dst;
		const char *s = src;

		do {
			if ((*d++ = *s++) == 0) {
				/* NUL pad the remaining n-1 bytes */
				while (--n != 0)
					*d++ = 0;
				break;
			}
		} while (--n != 0);
	}
	return dst;
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
	const unsigned char *a = str1;
	const unsigned char *b = str2;
	size_t i;
 
	for (i=0; i<n; i++) {
		if (a[i] != b[i]) {
			return (int)(a[i] - b[i]);
		}
	}
	return 0;
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
	return strcmp(str1, str2);
}

int strncmp(const char *str1, const char *str2, size_t n)
{
        if (n == 0)
                return (0);
        do {
                if (*str1 != *str2++)
                        return (*(unsigned char *)str1 - *(unsigned char *)--str2);
                if (*str1++ == 0)
                        break;
        } while (--n != 0);
        return (0);
}

size_t strxfrm(char *s1, const char *s2, size_t n)
{
	size_t len = strlen(s2) + 1;

	if (len < n)
			n = len;
	memcpy(s1, s2, n);
	return len;

}


/*
 * searching
 */

void *memchr(const void *s, int c, size_t n)
{
	const unsigned char*  p   = s;
    const unsigned char*  end = p + n;

    for (;;) {
        if (p >= end || p[0] == c) break; p++;
        if (p >= end || p[0] == c) break; p++;
        if (p >= end || p[0] == c) break; p++;
        if (p >= end || p[0] == c) break; p++;
    }
    if (p >= end)
        return NULL;
    else
        return (void*) p;

}

char *strchr(const char *p, int ch)
{
	 for (;; ++p) {
			if (*p == ch)
					return((char *)p);
			if (!*p)
					return((char *)NULL);
	}

}

size_t strcspn(const char *s1, const char *s2)
{
	const char *p, *spanp;
	char c, sc;

	/*
	 * Stop as soon as we find any character from s2.  Note that there
	 * must be a NUL in s2; it suffices to stop when we find that, too.
	 */
	for (p = s1;;) {
			c = *p++;
			spanp = s2;
			do {
					if ((sc = *spanp++) == c)
							return (p - 1 - s1);
			} while (sc != 0);
	}

}

char *strpbrk(const char *s1, const char *s2)
{
	const char *scanp;
	int c, sc;

	while ((c = *s1++) != 0) {
			for (scanp = s2; (sc = *scanp++) != 0;)
					if (sc == c)
							return ((char *)(s1 - 1));
	}
	return (NULL);

}

char *strrchr(const char *p, int ch)
{
	char *save;

	for (save = NULL;; ++p) {
		if (*p == ch)
			save = (char *)p;
		if (!*p)
			return(save);
	}

}

size_t strspn(const char *s1, const char *s2)
{
	const char *p = s1, *spanp;
    char c, sc;

    /*
     * Skip any characters in s2, excluding the terminating \0.
     */
	cont:
		c = *p++;
		for (spanp = s2; (sc = *spanp++) != 0;)
			if (sc == c)
				goto cont;
    return (p - 1 - s1);

}

char *strstr(const char *s1, const char *s2)
{
	int l1 = strlen(s1);
    int l2 = strlen(s2);

    while (l1 >= l2) {
        if (strcmp(s1, s2) != 0) {
            return s1;
        }
        s1++;
        l1--;
    }
    return 0;

}

char * strtok_r(char *s, const char *delim, char **last)
{
    char *spanp;
    int c, sc;
    char *tok;

    if (s == NULL && (s = *last) == NULL)
    {
        return NULL;
    }

    /*
     * Skip (span) leading delimiters (s += strspn(s, delim), sort of).
     */
cont:
    c = *s++;
    for (spanp = (char *)delim; (sc = *spanp++) != 0; )
    {
        if (c == sc)
        {
            goto cont;
        }
    }

    if (c == 0)         /* no non-delimiter characters */
    {
        *last = NULL;
        return NULL;
    }
    tok = s - 1;

    /*
     * Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
     * Note that delim must have one NUL; we stop if we see that, too.
     */
    for (;;)
    {
        c = *s++;
        spanp = (char *)delim;
        do
        {
            if ((sc = *spanp++) == c)
            {
                if (c == 0)
                {
                    s = NULL;
                }
                else
                {
                    char *w = s - 1;
                    *w = '\0';
                }
                *last = s;
                return tok;
            }
        }
        while (sc != 0);
    }
    /* NOTREACHED */
}



char *strtok(char *s, const char *delim)
{
	static char *last;

    return strtok_r(s, delim, &last);
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
	return "Error";
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
