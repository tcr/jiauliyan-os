/*
 * Jiauliyan OS - Released under the MIT License
 * Copyright (C) 2011 Paul Booth, Jialiya Huang, Tim Ryan
 * https://github.com/timcameronryan/jiauliyan
 */

#ifndef __LOCALE_H
#define __LOCALE_H

struct lconv {
	int decimal_point[5];
};

#define LC_ALL 0
#define LC_COLLATE 1
#define LC_CTYPE 2
#define LC_MONETARY 3
#define LC_NUMERIC 4
#define LC_TIME 5

extern struct lconv *localeconv(void);
extern char *setlocale(int category, const char *locale);

#endif
