#include <stddef.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <locale.h>
#include <setjmp.h>

#include <stream.h>
#include <system.h>
#include <serial.h>

sighandler_t signal(int signum, sighandler_t handler)
{
	stream_puts(serialstream, "signal() called.\n");
	return NULL;
}

char *no_data = "";

char *getenv(const char *name)
{
	stream_puts(serialstream, "getenv() called.\n");
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

unsigned long int strtoul(const char *start, char **endptr, int radix)
{
	stream_puts(serialstream, "strtoul() called.\n");
	return 0;
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
