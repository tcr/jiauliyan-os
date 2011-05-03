/*
 * Jiauliyan OS - Released under the MIT License
 * Copyright (C) 2011 Paul Booth, Jialiya Huang, Tim Ryan
 * https://github.com/timcameronryan/jiauliyan
 * 
 * Based on work by Nick Johnson's Rhombus OS, under the OpenBSD license
 * Copyright (C) 2009, 2010 Nick Johnson <nickbjohnson4224 at gmail.com>
 * https://github.com/nickbjohnson4224/rhombus/
 */

#ifndef __MATH_H
#define __MATH_H

/* constants */
#define INFINITY (1.0/0.0)
#define NAN (0.0/0.0)
#define HUGE_VAL INFINITY

/* numeric */
#define M_E        2.71828182845904523536     // e
#define M_LOG2E    1.44269504088896340736     // log2(e)
#define M_LOG10E   0.434294481903251827651    // log10(e)
#define M_LN2      0.693147180559945309417    // ln(2)
#define M_LN10     2.30258509299404568402     // ln(10)
#define M_PI       3.14159265358979323846     // pi
#define M_PI_2     1.57079632679489661923     // pi/2
#define M_PI_4     0.785398163397448309616    // pi/4
#define M_1_PI     0.318309886183790671538    // 1/pi
#define M_2_PI     0.636619772367581343076    // 2/pi
#define M_2_SQRTPI 1.12837916709551257390     // 2/sqrt(pi)
#define M_SQRT2    1.41421356237309504880     // sqrt(2)
#define M_SQRT1_2  0.707106781186547524401    // 1/sqrt(2)

/* macros */
#define isnan(x) ((x) != (x))
#define isinf(x) (((x) == INFINITY) || ((x) == -INFINITY))

/* trigonometric functions */
extern double acos(double x);
extern double asin(double x);
extern double atan(double x);
extern double atan2(double y, double x);
extern double cos(double x);
extern double cosh(double x);
extern double sin(double x);
extern double sinh(double x);
extern double tan(double x);
extern double tanh(double x);

/* exponential/logarithmic/power */
extern double exp(double x);
extern double frexp(double x, int *exponent);
extern double ldexp(double x, int exponent);
extern double log(double x);
extern double log10(double x);
extern double modf(double x, double *integer);
extern double pow(double x, double y);
extern double sqrt(double x);

/* other */
extern double ceil(double x);
extern double fabs(double x);
extern double floor(double x);
extern double fmod(double x, double y);

#endif
