#include <string.h>

/*
 * copying
 */
 
unsigned void *memcpy(void *dest, const void *src, int count)
{
	int i = 0;
	for (i = 0; i < count; i++) {
		((unsigned char *) dest)[i] = ((unsigned char *) src)[i];
	}
	return dest;
}

void *memmove(void *dest, const void *src, size_t count)
{
	//[TODO]
}

char *strcpy(char *dest, char *source)
{
	int i = 0;
	while(source[i] != '\0') {
		dest[i] = source[i];
		i++;
	}
	dest[i+1] = '\n';
	return dest;
}

char *strncpy(char *dest, char *source, int n)
{
	int i = 0;
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

int strcmp(const char *str1, const char *str2)
{
	//[TODO]
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

void *memsetw(void *dest, int val, int count)
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
