/*
 * Jiauliyan OS - Released under the MIT License
 * Copyright (C) 2011 Paul Booth, Jialiya Huang, Tim Ryan
 * https://github.com/timcameronryan/jiauliyan
 * 
 * Based on work by Nick Johnson's Rhombus OS, under the OpenBSD license
 * Copyright (C) 2009, 2010 Nick Johnson <nickbjohnson4224 at gmail.com>
 * https://github.com/nickbjohnson4224/rhombus/
 */

#include <stream.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <common.h>
#include <vga.h>

/*
 * stream initializer
 */

stream_s *stream_create(
	int (*get)(stream_s *stream),
	int (*put)(stream_s *stream, unsigned char s),
	long int (*avail)(struct __stream *stream),
	int (*seek)(stream_s *stream, long pos, int whence),
	void *data)
{
	stream_s *s = (stream_s *) malloc(sizeof(stream_s));
	s->get = get;
	s->put = put;
	s->avail = avail;
	s->seek = seek;
	s->data = data;
	s->ferr = 0;
	return s;
}

void stream_destroy(stream_s *stream)
{
	free(stream);
}

/* stream impl dummies */

int stream_no_get(stream_s *stream)
{
	(void) stream;
	return EOF;
}

int stream_no_put(stream_s *stream, unsigned char s)
{
	(void) stream; (void) s;
	return EOF;
}

long int stream_no_avail(stream_s *stream)
{
	(void) stream;
	return 0;
}

int stream_no_seek(stream_s *stream, long pos, int whence)
{
	(void) stream; (void) pos; (void) whence;
	return -1;
}

/*
 * stream utilities
 */

size_t stream_write(stream_s *stream, const void *ptr, size_t size)
{
	unsigned char *p = (unsigned char *) ptr;
	size_t i;
	for (i = 0; i < size; i++) {
		if (stream->put(stream, *p) == EOF)
			break;
		p++;
	}
	return i;
}

/*
 * byte stream
 */

typedef struct {
	unsigned char *buf;
	long int size;
	long int capacity;
	long int pos;
} bytestream_s;

/* bytestream impl */

static int bytestream_get(stream_s *stream)
{
	bytestream_s *data = (bytestream_s *) stream->data;
	if (data->pos >= data->size)
		return EOF;
	return data->buf[data->pos++];
}

static int bytestream_put(stream_s *stream, unsigned char c)
{
	bytestream_s *data = (bytestream_s *) stream->data;
	
	// expand file size
	if (data->pos >= data->capacity) {
		data->capacity += BUFSIZ;
		data->buf = (unsigned char *) realloc(data->buf, data->capacity);
	}

	if (data->pos + 1 > data->size)
		data->size = data->pos + 1;
	data->buf[data->pos++] = c;
	return (int) c;
}

static int bytestream_seek(stream_s *stream, long pos, int origin)
{
	UNUSED(origin);
	bytestream_s *data = (bytestream_s *) stream->data;
	data->pos = pos;
	return 0;
}

/* bytestream api */

stream_s *bytestream_create(long int capacity)
{
	bytestream_s *data = malloc(sizeof(bytestream_s));
	data->buf = (unsigned char *) malloc(capacity);
	data->size = 0;
	data->capacity = capacity;
	data->pos = 0;
	stream_s *stream = malloc(sizeof(stream_s));
	stream->get = bytestream_get;
	stream->put = bytestream_put;
	stream->seek = bytestream_seek;
	stream->data = data;
	return stream;
}

void bytestream_destroy(stream_s *stream)
{
	free(stream->data);
	free(stream);
}

long int bytestream_size(stream_s *stream)
{
	bytestream_s *data = (bytestream_s *) stream->data;
	return data->size;
}

unsigned char *bytestream_data(stream_s *stream)
{
	bytestream_s *data = (bytestream_s *) stream->data;
	return data->buf;
}

/*
 * string formatting
 */

#define TYPE_STRING		0		// String
#define TYPE_CHAR		1		// Character
#define TYPE_LITERAL	2		// Literal '%"
#define TYPE_INT		3		// Signed integer
#define TYPE_UINT		4		// Unsigned integer
#define TYPE_DOUBLE		5		// Floating point

size_t stream_putbyte(stream_s *stream, unsigned char b)
{
	stream->put(stream, b);
	return 1;
}

size_t stream_putchar(stream_s *stream, char c)
{
	stream->put(stream, (unsigned char) c);
	return 1;
}

size_t stream_putstring(stream_s *stream, const char *text)
{
	if (text == NULL)
		text = "(null)";
		
	size_t i;
	for (i = 0; i < strlen(text); i++)
		stream_putchar(stream, text[i]);
	return i;
}

size_t stream_putuint(stream_s *stream, unsigned int value, int flags)
{
	/* shortcut */
	if (value == 0) {
		return stream_putchar(stream, '0');
	}
	
	const char *digits_lower = "0123456789abcdef";
	const char *digits_upper = "0123456789ABCDEF";
	const char *digits = flags & FLAG_UPPER ? digits_upper : digits_lower;

	int base = 10;
	if (flags & FLAG_OCTAL) 	base = 8;
	else if (flags & FLAG_HEX) 	base = 16;
	
	int len = 0;
	
	/* prefixes */
	if (flags & FLAG_SIGN) {
		stream_putchar(stream, '+');
		len++;
	}
	if (flags & FLAG_ALT) {
		if (flags & FLAG_OCTAL) {
			stream_putchar(stream, '0');
			len++;
		}
		if (flags & FLAG_HEX) {
			stream_putstring(stream, (flags & FLAG_UPPER) ? "0X" : "0x");
			len += 2;
		}
	}

	/* print digits top down */
	int i, divisor = 1;
	for (i = 0; i < 16; i++) {
		if (divisor * base > (int) value)
			break;
		divisor *= base;
	}
	for ( ; i >= 0; i--, len++) {
		stream_putchar(stream, digits[(int) (value / divisor) % base]);
		divisor /= base;
	}
	
	return len;
}

size_t stream_putint(stream_s *stream, int value, int flags)
{
	size_t len;

	if (value < 0) {
		stream_putchar(stream, '-');
		len = stream_putuint(stream, -value, flags) + 1;
	} else {
		len = stream_putuint(stream, value, flags);
	}

	return len;
}

/**********************************************************************/
/** double formatting code */
/** TODO: revise this to be smaller */
/**********************************************************************/
 
char *strdup(const char *string)
 {
     char       *nstr;
 
     nstr = (char *) malloc(strlen(string) + 1);
     if (nstr)
        strcpy(nstr, string);
     return nstr;
}

char *strvcat(const char *s1, ...) {
	va_list ap;
	const char *argv[16];
	char *fullstring;
	size_t i, length, pos;

	va_start(ap, s1);

	argv[0] = s1;

	for (i = 1; i < 16; i++) {
		argv[i] = va_arg(ap, const char*);
		if (!argv[i]) {
			break;
		}
	}

	for (length = 0, i = 0; i < 16 && argv[i]; i++) {
		length += strlen(argv[i]);
	}

	fullstring = malloc(length + 1);

	if (!fullstring) {
		errno = ENOMEM;
		return NULL;
	}

	fullstring[0] = '\0';

	for (i = 0, pos = 0; i < 16 && argv[i]; i++) {
		strcat(&fullstring[pos], argv[i]);
		pos += strlen(argv[i]);
	}

	return fullstring;
}

long double logl(long double x) {
	return __builtin_logl(x);
}

long double log10l(long double x)
{
	return (logl(x) / M_LN10);
}

long double expl(long double x)
{
	return __builtin_expl(x);
}

long double powl(long double b, long double e) {
	return expl(e * logl(b));
}

long double modfl(long double x, long double *iptr)
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
		*iptr = (long double) (int64_t) x;
		return (x - *iptr);
	}
}

long double floorl(long double x)
{
	modfl(x, &x);
	if (x < 0.0) x -= 1.0;
	return x;
}

size_t strlcpy(char *dst, const char *src, size_t siz) {
	char *d = dst;
	const char *s = src;
	size_t n = siz;

	/* Copy as many bytes as will fit */
	if (n != 0) {
		while (--n != 0) {
			if ((*d++ = *s++) == '\0') {
				break;
			}
		}
	}

	/* Not enough room in dst, add NUL and traverse rest of src */
	if (n == 0) {
		if (siz != 0) {
			*d = '\0';		/* NUL-terminate dst */
		}

		while (*s++);
	}

	return (s - src - 1);	/* count does not include NUL */
}

size_t strlcat(char *dst, const char *src, size_t siz) {
	char *d = dst;
	const char *s = src;
	size_t n = siz;
	size_t dlen;

	/* Find the end of dst and adjust bytes left but don't go past end */
	while (n-- != 0 && *d != '\0') {
		d++;
	}

	dlen = d - dst;
	n = siz - dlen;

	if (n == 0) {
		return (dlen + strlen(s));
	}
	while (*s != '\0') {
		if (n != 1) {
			*d++ = *s;
			n--;
		}
		s++;
	}
	*d = '\0';

	return (dlen + (s - src));	/* count does not include NUL */
}

static char *__format_uint(unsigned int value, int flags) {
	char buffer[16];
	int i;
	int base;

	const char *digits_lower = "0123456789abcdef";
	const char *digits_upper = "0123456789ABCDEF";
	const char *digits;

	if (flags & FLAG_OCTAL) 	base = 8;
	else if (flags & FLAG_HEX) 	base = 16;
	else 						base = 10;

	if (value == 0) {
		return strdup("0");
	}

	if (flags & FLAG_UPPER) {
		digits = digits_upper;
	}
	else {
		digits = digits_lower;
	}

	buffer[15] = '\0';
	for (i = 14; i >= 0; i--) {
		if (!value) break;
		buffer[i] = digits[value % base];
		value /= base;
	}

	if (flags & FLAG_SIGN) {
		buffer[i--] = '+';
	}

	i++;

	if (flags & FLAG_ALT) {
		if (flags & FLAG_OCTAL) {
			return strvcat("0", &buffer[i], NULL);
		}
		if (flags & FLAG_HEX) {
			return strvcat((flags & FLAG_UPPER) ? "0X" : "0x", &buffer[i], NULL);
		}
	}
	return strdup(&buffer[i]);
}

static char *__format_int(int value, int flags) {
	char *string, *temp;

	if (value < 0) {
		temp = __format_uint(-value, flags);
		string = strvcat("-", temp, NULL);
		free(temp);
	}
	else {
		string = __format_uint(value, flags);
	}

	return string;
}

static char *__format_double_frac(long double value, int flags, int precision) {
	char *string;
	int i;

	if (value < 0) value = -value;
	if (value > 1) value -= floorl(value);

	string = malloc(precision + 1);

	for (i = 0; i < precision; i++) {
		value *= 10;
		string[i] = (int) value + '0';
		value -= floorl(value);

		if (value == 0.0 && !(flags & FLAG_ALT && flags & FLAG_MEXP)) {
			i++;
			break;
		}
	}
	
	string[i] = '\0';

	return string;
}

static char *__format_double_int(long double value, int flags) {
	char *temp, *string;
	int expt;

	expt = log10l(value) + 1;
	value /= (int) powl(10, expt);

	temp = __format_double_frac(value, flags | FLAG_ALT | FLAG_MEXP, expt);

	if (value < 0) {
		string = strvcat("-", temp, NULL);
		free(temp);
	}
	else if (flags & FLAG_SIGN) {
		string = strvcat("+", temp, NULL);
		free(temp);
	}
	else {
		string = temp;
	}

	return string;
}

static char *__format_double(long double value, int flags, int precision) {
	char *int_part;
	char *dec_part;
	char *string;
	long double i;

	if (isnan(value)) {
		if (flags & FLAG_UPPER) {
			return strdup("NAN");
		}
		else {
			return strdup("nan");
		}
	}

	if (isinf(value)) {
		if (flags & FLAG_UPPER) {
			if (value < 0) {
				return strdup("-INF");
			}
			else {
				if (flags & FLAG_SIGN) {
					return strdup("+INF");
				}
				else {
					return strdup("INF");
				}
			}
		}
		else {
			if (value < 0) {
				return strdup("-inf");
			}
			else {
				if (flags & FLAG_SIGN) {
					return strdup("+inf");
				}
				else {
					return strdup("inf");
				}
			}
		}
	}

	if (flags & FLAG_EXP || (flags & FLAG_MEXP && value > 1000000000)) {
		i = log10l(value);
		value /= powl(10, floorl(i));

		int_part = __format_double(value, flags & ~(FLAG_EXP), precision);
		dec_part = __format_int(i, flags | FLAG_SIGN);

		string = strvcat(int_part, (flags & FLAG_UPPER) ? "E" : "e", dec_part, NULL);

		free(int_part);
		free(dec_part);
	}
	else {
		value = modfl(value, &i);

		if (value > 2000000000) {
			int_part = __format_double_int(i, flags);
		}
		else {
			int_part = __format_int(i, flags);
		}
		dec_part = __format_double_frac(value, flags, precision);

		if (i == 0.0 && value < 0) {
			if (value != 0.0 || flags & FLAG_ALT) {
				string = strvcat("-", int_part, ".", dec_part, NULL);
			}
			else {
				string = strvcat("-", int_part, NULL);
			}
		}
		else {
			if (value != 0.0 || flags & FLAG_ALT) {
				string = strvcat(int_part, ".", dec_part, NULL);
			}
			else {
				string = strdup(int_part);
			}
		}

		free(int_part);
		free(dec_part);
	}

	return string;
}

size_t stream_putdouble(stream_s *stream, long double value, int precision, int flags)
{
	char *string = __format_double(value, flags, precision);
	size_t len = strlen(string);
	stream_putstring(stream, string);
	free(string);
	return len;
}

/**********************************************************************/
/**********************************************************************/

static size_t stream_format_arg(stream_s *stream, const char **_fmt, va_list *argp)
{
	int flags, type, length;
	size_t width, precision;
	const char *fmt = *_fmt;

	long double val_d;
	int val_i;
	unsigned int val_u;
	const char *val_s;
	char val_c;

	flags = 0;
	type  = 0;
	width = 0;
	length = LENGTH_LONG;
	precision = 6;

	/* check for '%' */
	if (*fmt != '%') {
		return NULL;
	}

	/* read flags */
	do {
		fmt++;
		switch (*fmt) {
		case '+': flags |= FLAG_SIGN;  continue;
		case '-': flags |= FLAG_LEFT;  continue;
		case '#': flags |= FLAG_ALT;   continue;
		case '0': flags |= FLAG_ZERO;  continue;
		case '\0': return NULL;
		}
		break;
	} while (1);

	/* read width */
	if (*fmt == '*') {
		width = va_arg(*argp, int);
		fmt++;
	}
	else if (isdigit(*fmt)) {
		width = atoi(fmt);
		while (isdigit(*fmt)) fmt++;
	}

	/* read precision */
	if (*fmt == '.') {
		fmt++;

		if (*fmt == '*') {
			precision = va_arg(*argp, int);
			fmt++;
		}
		else if (isdigit(*fmt)) {
			precision = atoi(fmt);
			while (isdigit(*fmt)) fmt++;
		}
		else {
			return NULL;
		}
	}

	/* read length */
	switch (*fmt) {
	case 'h':
		fmt++;
		if (*fmt == 'h') {
			length = LENGTH_BYTE;
			fmt++;
		}
		else {
			length = LENGTH_SHORT;
		}
		break;
	case 'l':
		fmt++;
		if (*fmt == 'l') {
			length = LENGTH_LONG;
			fmt++;
		}
		else {
			length = LENGTH_LLONG;
		}
		break;
	case 'L': case 'j':
		length = LENGTH_LLONG;
		fmt++;
		break;
	case 'z': case 't':
		length = LENGTH_LONG;
		fmt++;
	case '\0':
		return NULL;
	}

	/* read type */
	switch (*fmt) {
	case 'i': case 'd':
		type = TYPE_INT;
		break;
	case 'u':
		type = TYPE_UINT;
		break;
	case 'F': flags |= FLAG_UPPER;
	case 'f':
		type = TYPE_DOUBLE;
		break;
	case 'G': flags |= FLAG_UPPER;
	case 'g':
		type = TYPE_DOUBLE;
		flags |= FLAG_MEXP;
		break;
	case 'E': flags |= FLAG_UPPER;
	case 'e':
		type = TYPE_DOUBLE;
		flags |= FLAG_EXP;
		break;
	case 'X': flags |= FLAG_UPPER;
	case 'x': case 'p': 
		type = TYPE_UINT;
		flags |= FLAG_HEX;
		break;
	case 'O': flags |= FLAG_UPPER;
	case 'o':
		type = TYPE_UINT;
		flags |= FLAG_OCTAL;
		break;
	case 's':
		type = TYPE_STRING;
		break;
	case 'c':
		type = TYPE_CHAR;
		break;
	case '%':
		type = TYPE_LITERAL;
		break;
	default:
		return NULL;
	}

	/* read value */
	switch (type) {
	case TYPE_STRING:	val_s = va_arg(*argp, const char *); 	break;
	case TYPE_CHAR:		val_c = va_arg(*argp, char); 			break;
	case TYPE_LITERAL:	val_c = '%';							break;
	case TYPE_INT:		val_i = va_arg(*argp, int);				break;
	case TYPE_UINT:		val_u = va_arg(*argp, unsigned int);	break;
	case TYPE_DOUBLE:
		if (length == LENGTH_LLONG)	val_d = va_arg(*argp, long double);
		else						val_d = va_arg(*argp, double);
		break;
	}
	
	/* create bytestream */
	stream_s *bs = bytestream_create(BUFSIZ);

	/* format value */
	switch (type) {
	case TYPE_STRING:
		stream_putstring(bs, val_s);
		break;
	case TYPE_CHAR: case TYPE_LITERAL:
		stream_putchar(bs, val_c);
		break;
	case TYPE_INT:
		stream_putint(bs, val_i, flags);
		break;
	case TYPE_UINT:
		stream_putuint(bs, val_u, flags);
		break;
	case TYPE_DOUBLE:
		stream_putdouble(bs, val_d, precision, flags);
		break;
	}
	
	/* get string data */
	size_t flen = bytestream_size(bs) - 1;
	char *fstr = (char *) bytestream_data(bs);
	
	if (flen < width) {
		if (flags & FLAG_LEFT)
			stream_putstring(stream, fstr);
			
		size_t i;
		for (i = 0; i < width - flen; i++)
			stream_putchar(stream, (flags & FLAG_ZERO) ? '0' : ' ');
			
		if (!(flags & FLAG_LEFT))
			stream_putstring(stream, fstr);
	} else {
		stream_putstring(stream, fstr);
	}

	*_fmt = fmt;
	
	return flen < width ? width : flen;
}

size_t stream_vformat(stream_s *stream, const char *format, va_list ap)
{
	size_t i, count = 0;
	const char *format_tmp;

	for (i = 0; format[i]; i++) {
		if (format[i] != '%') {
			/* push non-formatting option */
			stream->put(stream, (unsigned char) format[i]);
			count++;
			continue;
		}

		format_tmp = &format[i]; /* format string location */
		count += stream_format_arg(stream, &format_tmp, &ap);
		i = (uintptr_t) format_tmp - (uintptr_t) format;
	}

	return 0;
} 

size_t stream_format(stream_s *stream, const char *format, ...)
{
	va_list ap;
	int ret;

	va_start(ap, format);
	ret = stream_vformat(stream, format, ap);
	va_end(ap);

	return ret;
}
