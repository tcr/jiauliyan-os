#include <common.h>
#include <stddef.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <locale.h>
#include <setjmp.h>

#include <stream.h>
#include <system.h>
#include <serial.h>

#include <errno.h>
#include <limits.h>
#include <ctype.h>
#include <math.h>
#include <float.h>

sighandler_t signal(int signum, sighandler_t handler)
{
	UNUSED(signum); UNUSED(handler);
	stream_puts(serialout, "signal() called.\n");
	return NULL;
}

char *no_data = "";

char *getenv(const char *name)
{
	stream_puts(serialout, name);
	stream_puts(serialout, " getenv() called.\n");
	return no_data;
}

void exit(int exit_code)
{
	UNUSED(exit_code);
	stream_puts(serialout, "exit() called.\n");
	puts("\nEXITING\n");
}

int system(const char *string)
{
	UNUSED(string);
	stream_puts(serialout, "system() called.\n");
	return 1;
}

double strtod(const char *str, char **endptr)
{
	double number;
	int exponent;
	int negative;
	char *p = (char *) str;
	double p10;
	int n;
	int num_digits;
	int num_decimals;

	// Skip leading whitespace
	while (isspace(*p)) p++;

	// Handle optional sign
	negative = 0;
	switch (*p) 
	{             
	case '-': negative = 1; // Fall through to increment position
	case '+': p++;
	}

	number = 0.;
	exponent = 0;
	num_digits = 0;
	num_decimals = 0;

	// Process string of digits
	while (isdigit(*p))
	{
		number = number * 10. + (*p - '0');
		p++;
		num_digits++;
	}

	// Process decimal part
	if (*p == '.') 
	{
		p++;

		while (isdigit(*p))
		{
			number = number * 10. + (*p - '0');
			p++;
			num_digits++;
			num_decimals++;
		}

		exponent -= num_decimals;
	}

	if (num_digits == 0)
	{
		errno = ERANGE;
		return 0.0;
	}

	// Correct for sign
	if (negative) number = -number;

	// Process an exponent string
	if (*p == 'e' || *p == 'E') 
	{
		// Handle optional sign
		negative = 0;
		switch(*++p) 
		{   
		case '-': negative = 1;   // Fall through to increment pos
		case '+': p++;
		}

		// Process string of digits
		n = 0;
		while (isdigit(*p)) 
		{   
			n = n * 10 + (*p - '0');
			p++;
		}

		if (negative) 
			exponent -= n;
		else
			exponent += n;
	}

	if (exponent < DBL_MIN_EXP  || exponent > DBL_MAX_EXP)
	{
		errno = ERANGE;
		return HUGE_VAL;
	}

	// Scale the result
	p10 = 10.;
	n = exponent;
	if (n < 0) n = -n;
	while (n) 
	{
		if (n & 1) 
		{
			if (exponent < 0)
				number /= p10;
			else
				number *= p10;
		}
		n >>= 1;
		p10 *= p10;
	}

	if (number == HUGE_VAL) errno = ERANGE;
	if (endptr) *endptr = p;

	return number;
}

unsigned long strtoul(char *nptr, char **endptr, int base)
{
	char *s;
	unsigned long acc, cutoff;
	int c;
	int neg, any, cutlim;
 
	/*
	 * See strtol for comments as to the logic used.
	 */
	s = nptr;
	do {
		c = (unsigned char) *s++;
	} while (isspace(c));
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else {
		neg = 0;
		if (c == '+')
			c = *s++;
	}
	if ((base == 0 || base == 16) && c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;
 
	cutoff = ULONG_MAX / (unsigned long) base;
	cutlim = ULONG_MAX % (unsigned long) base;
	for (acc = 0, any = 0;; c = (unsigned char) *s++) {
		if (isdigit(c))
			c -= '0';
		else if (isalpha(c))
			c -= isupper(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0)
			continue;
		if ((acc > cutoff || acc == cutoff) && c > cutlim) {
			any = -1;
			acc = ULONG_MAX;
			errno = ERANGE;
		} else {
			any = 1;
			acc *= (unsigned long) base;
			acc += c;
		}
	}
	if (neg && any > 0)
		acc = -acc;
	if (endptr != 0)
		*endptr = (char *) (any ? s - 1 : nptr);
	return (acc);
}

struct tm *gmtime(const time_t *timer)
{
	UNUSED(timer);
	stream_puts(serialout, "gmtime() called.\n");
	return NULL;
}

struct tm *localtime(const time_t *timer)
{
	UNUSED(timer);
	stream_puts(serialout, "localtime() called.\n");
	return NULL;
}

clock_t clock(void)
{
	stream_puts(serialout, "clock() called.\n");
	return 0;
}

time_t time(time_t *timeptr)
{
	UNUSED(timeptr);
	stream_puts(serialout, "time() called.\n");
	return 0;
}

struct lconv *localeconv(void)
{
	stream_puts(serialout, "localeconv() called.\n");
	return NULL;
}

char *setlocale(int category, const char *locale)
{
	UNUSED(category); UNUSED(locale);
	stream_puts(serialout, "setlocale() called.\n");
	return (char *) locale;
}

time_t mktime(struct tm *tmbuf)
{
	UNUSED(tmbuf);
	stream_puts(serialout, "mktime() called.\n");
	return 0;
}

size_t strftime(char *s, size_t maxsize, const char *format, const struct tm *t)
{
	UNUSED(s); UNUSED(maxsize); UNUSED(format); UNUSED(t);
	stream_puts(serialout, "strftime() called.\n");
	return 0;
}
