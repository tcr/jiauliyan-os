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

sighandler_t signal(int signum, sighandler_t handler)
{
	stream_puts(serialstream, "signal() called.\n");
	return NULL;
}

char *no_data = "";

char *getenv(const char *name)
{
	stream_puts(serialstream, name);
	stream_puts(serialstream, " getenv() called.\n");
	return no_data;
}

void exit(int exit_code)
{
	stream_puts(serialstream, "exit() called.\n");
	puts("\nEXITING\n");
}

int system(const char *string)
{
	stream_puts(serialstream, "system() called.\n");
	return 1;
}

double strtod(const char *start, char **endptr)
{
	stream_puts(serialstream, "strtod() called.\n");
	return 0;
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
	stream_puts(serialstream, "gmtime() called.\n");
	return NULL;
}

struct tm *localtime(const time_t *timer)
{
	stream_puts(serialstream, "localtime() called.\n");
	return NULL;
}

clock_t clock()
{
	stream_puts(serialstream, "clock() called.\n");
	return 0;
}

time_t time(time_t *timeptr)
{
	stream_puts(serialstream, "time() called.\n");
	return 0;
}

struct lconv *localeconv(void)
{
	stream_puts(serialstream, "localeconv() called.\n");
	return NULL;
}

char *setlocale(int category, const char *locale)
{
	stream_puts(serialstream, "setlocale() called.\n");
	return locale;
}

time_t mktime(struct tm *tmbuf)
{
	stream_puts(serialstream, "mktime() called.\n");
	return 0;
}

size_t strftime(char *s, size_t maxsize, const char *format, const struct tm *t)
{
	stream_puts(serialstream, "strftime() called.\n");
	return 0;
}
