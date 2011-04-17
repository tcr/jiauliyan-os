#ifndef __MATH_H
#define __MATH_H

#define HUGE_VAL __builtin_huge_val()

/* trigonometric functions */
extern double acos(double x);
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
