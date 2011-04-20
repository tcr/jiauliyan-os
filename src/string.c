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
	//[TODO]
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
