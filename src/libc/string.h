/*
 * Jiauliyan OS - Released under the MIT License
 * Copyright (C) 2011 Paul Booth, Jialiya Huang, Tim Ryan
 * https://github.com/timcameronryan/jiauliyan
 * 
 * Based on code from the Android project
 * Copyright (c) 1990 The Regents of the University of California
 * http://android.git.kernel.org/
 */

#ifndef __STRING_H
#define __STRING_H

#include <stddef.h>

/* copying */
extern void *memcpy(void *dest, const void *src, size_t count);
extern void *memmove(void *dest, const void *src, size_t count);
extern char *strcpy(char *dest, const char *src);
extern char *strncpy(char *dest, const char *src, size_t count);

/* concatenation */
extern char *strcat(char *str1, const char *str2);
extern char *strncat(char *str1, const char *str2, size_t n);

/* comparison */
extern int memcmp(const void *str1, const void *str2, size_t n);
extern int strcmp(const char *str1, const char *str2);
extern int strcoll(const char *str1, const char *str2);
extern int strncmp(const char *str1, const char *str2, size_t n);
extern size_t strxfrm(char *str1, const char *str2, size_t n);

/* searching */
extern void *memchr(const void *str, int c, size_t n);
extern char *strchr(const char *str, int c);
extern size_t strcspn(const char *str1, const char *str2);
extern char *strpbrk(const char *str1, const char *str2);
extern char *strrchr(const char *str, int c);
extern size_t strspn(const char *str1, const char *str2);
extern char *strstr(const char *str1, const char *str2);
extern char *strtok_r(char *s, const char *delim, char **last);
extern char *strtok(char *str1, const char *str2);

/* other */
extern void *memsetw(void *dest, short int val, int count);
extern void *memset(void *dest, int val, int count);
extern char *strerror(int errnum);
extern size_t strlen(const char *str);

#endif
