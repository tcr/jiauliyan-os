char * reverse( char * s)
{
	int i, len;
	char temp;
	len = strlen(s);
	for ( i=0; i < len/2; i++) {
		temp = s[i];
		s[i] = s[len - 1 - i]; /*i=0 -> length -1 not outside bounds*/
		s[len - 1 - i] = temp;
	}
	return s;
}

char * itoa( int i, char * s, int pos) {
	if (i == 0) 
		return s;
	itoa(i / 10, char * s, pos + 1);
	s[pos] = (char) ( (i % 10)  + 48);
	return s;
}
