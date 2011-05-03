/*
 * Jiauliyan OS - Released under the MIT License
 * Copyright (C) 2011 Paul Booth, Jialiya Huang, Tim Ryan
 * https://github.com/timcameronryan/jiauliyan
 * 
 * Based on work by Nick Johnson's Rhombus OS, under the OpenBSD license
 * Copyright (C) 2009, 2010 Nick Johnson <nickbjohnson4224 at gmail.com>
 * https://github.com/nickbjohnson4224/rhombus/
 */

#include <math.h>
#include <stdint.h>

/*
 * trigonometric functions
 */

double acos(double x)
{
	return (M_PI_2 - asin(x));
}

double asin(double x)
{
	return __builtin_asin(x);
}

double atan(double x)
{
	return asin(x / sqrt(x * x + 1));
}

double atan2(double y, double x)
{
	if (y == 0.0) {
		if (x >= 0.0) {
			return 0.0;
		}
		else {
			return M_PI;
		}
	}
	else if (y > 0.0) {
		if (x == 0.0) {
			return M_PI_2;
		}
		else if (x > 0.0) {
			return atan(y / x);
		}
		else {
			return M_PI - atan(y / x);
		}
	}
	else {
		if (x == 0.0) {
			return M_PI + M_PI_2;
		}
		else if (x > 0.0) {
			return 2 * M_PI - atan(y / x);
		}
		else {
			return M_PI + atan(y / x);
		}
	}
}

double cos(double x)
{
	return sin(x + M_PI_2);
}

double cosh(double x)
{
	x = exp(x);
	return ((x + (1.0 / x)) / 2.0);
}

double sin(double x)
{
	return __builtin_sin(x);
}

double sinh(double x)
{
	x = exp(x);
	return ((x - (1.0 / x)) / 2.0);
}

double tan(double x)
{
	return (sin(x) / cos(x));
}

double tanh(double x)
{
	double x0 = exp(x);
	double x1 = 1.0 / x0;

	return ((x0 + x1) / (x0 - x1));
}

/* 
 * exponential/logarithmic/power
 */
 
double exp(double x)
{
	return __builtin_exp(x);
}

double frexp(double x, int *exp)
{
	return __builtin_frexp(x, exp);
}

double ldexp(double x, int n)
{
	return __builtin_ldexp(x, n);
}

double log(double x)
{
	return __builtin_log(x);
}

double log10(double x)
{
	return (log(x) / M_LN10);
}

double modf(double x, double *iptr)
{
	if (fabs(x) >= 4503599627370496.0) {
		*iptr = x;
		return 0.0;
	}
	else if (fabs(x) < 1.0) {
		*iptr = 0.0;
		return x;
	}
	else {
		*iptr = (double) (int64_t) x;
		return (x - *iptr);
	}
}

double pow(double b, double e)
{
	return exp(e * log(b));
}

#define DBL_EPSILON 2.2204460492503131e-16

double sqrt(double x)
{
	double y0, y1, e;

	if (x < 0.0) {
		return NAN;
	}

	y0 = x;
	y1 = 0.0;
	e  = 1.0;

	while (fabs(y0 - y1) > e) {
		y0 = y1;
		y1 = y1 - (y1 * y1 - x) / (2.0 * y1);
		e  = DBL_EPSILON * y1;
	}

	return y1;
}

/*
 * other
 */
 
double ceil(double x)
{
	return __builtin_ceil(x);
}

double fabs(double x)
{
	return ((x < 0) ? -x : x);
}

double floor(double x)
{
	modf(x, &x);
	if (x < 0.0) x -= 1.0;
	return x;
}

double fmod(double x, double div)
{
	return __builtin_fmod(x, div);
}
