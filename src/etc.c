#include <stddef.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <locale.h>
#include <setjmp.h>

sighandler_t signal(int signum, sighandler_t handler)
{
	return NULL;
}

char *no_data = "";

char *getenv(const char *name)
{
	return no_data;
}

void exit(int exit_code)
{
	puts("\nEXITING\n");
}

int system(const char *string)
{
	return 1;
}

double strtod(const char *start, char **endptr)
{
	return 0;
}

unsigned long int strtoul(const char *start, char **endptr, int radix)
{
	return 0;
}

int setjmp(jmp_buf env)
{
	return -1;
}

void longjmp(jmp_buf env, int value)
{
}

struct tm *gmtime(const time_t *timer)
{
	return NULL;
}

struct tm *localtime(const time_t *timer)
{
	return NULL;
}

clock_t clock()
{
	return 0;
}

time_t time(time_t *timeptr)
{
	return 0;
}

struct lconv *localeconv(void)
{
	return NULL;
}

char *setlocale(int category, const char *locale)
{
	return locale;
}

time_t mktime(struct tm *tmbuf)
{
	return 0;
}

size_t strftime(char *s, size_t maxsize, const char *format, const struct tm *t)
{
	return 0;
}
