/*
 * Jiauliyan OS - Released under the MIT License
 * Copyright (C) 2011 Paul Booth, Jialiya Huang, Tim Ryan
 * https://github.com/timcameronryan/jiauliyan
 */

#ifndef __STDLIB_H
#define __STDLIB_H

/* reference: http://www.acm.uiuc.edu/webmonkeys/book/c_guide/2.13.html */

#include <stddef.h>

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

/* string functions */
extern double atof(const char *str);
extern int atoi(const char *nptr);
extern long int atol(const char *str);
extern double strtod(const char *start, char **endptr);
extern long int strtol(const char *start, char **endptr, int radix);
extern unsigned long int strtoul(const char *start, char **endptr, int radix);

/* memory functions */
extern void free(void *ptr);
extern void *malloc(size_t size);
extern void *realloc(void *ptr, size_t size);

/* environment functions */
extern void abort(void);
extern int atexit(void (*func)(void));
extern void exit(int exit_code);
extern char *getenv(const char *name);
extern int system(const char *string);

/* math functions */
extern int abs(int value);
extern int rand(void);
extern void srand(unsigned int seed);

#endif
