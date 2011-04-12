unsigned char *memcpy(void *dest, const void *src, int count)
{
	/* Add code here to copy 'count' bytes of data from 'src' to
	 *  'dest', finally return 'dest' */
	int i = 0;
	for (i = 0; i < count; i++) {
		((unsigned char *) dest)[i] = ((unsigned char *) src)[i];
	}
	return dest;

}

unsigned char *memset(void *dest, int val, int count)
{
	/* Add code here to set 'count' bytes in 'dest' to 'val'.
	 *  Again, return 'dest' */
	int i = 0;
	for (i = 0; i < count; i++) {
		((unsigned char *) dest)[i] = (unsigned char) val;
	}
	return dest;
}

unsigned short *memsetw(void *dest, int val, int count)
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
