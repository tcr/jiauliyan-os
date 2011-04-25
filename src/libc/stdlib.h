#ifndef __STDLIB_H
#define __STDLIB_H

// http://www.acm.uiuc.edu/webmonkeys/book/c_guide/2.13.html#rand

#include <stddef.h>

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

/* utility */
extern int atoi(const char *nptr);

/* environment functions */
extern char *getenv(const char *name);
extern void exit(int exit_code);
extern int system(const char *string);

/* string functions */
extern double strtod(const char *start, char **endptr);
extern unsigned long int strtoul(const char *start, char **endptr, int radix);

/* memory functions */
extern void free(void *ptr);
extern void *malloc(size_t size);
extern void *realloc(void *ptr, size_t size);

/* math functions */
extern int abs(int value);
extern int rand(void);
extern void srand(unsigned int seed);

#endif
