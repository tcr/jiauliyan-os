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
	int (*read)(stream_s *stream),
	int (*write)(stream_s *stream, unsigned char s),
	int (*seek)(stream_s *stream, long pos, int whence),
	void *data)
{
	stream_s *s = (stream_s *) malloc(sizeof(stream_s));
	s->read = read;
	s->write = write;
	s->seek = seek;
	s->data = data;
	s->ferr = 0;
	return s;
}

void stream_destroy(stream_s *stream)
{
	free(stream);
}

/*
 * stream impl dummies
 */

int stream_no_read(stream_s *stream)
{
	(void) stream;
	return EOF;
}

int stream_no_write(stream_s *stream, unsigned char s)
{
	(void) stream; (void) s;
	return EOF;
}

int stream_no_seek(stream_s *stream, long pos, int whence)
{
	(void) stream; (void) pos; (void) whence;
	return -1;
}

/*
 * formatting
 * modeled after: https://github.com/nickbjohnson4224/rhombus/blob/master/libc/stdio/__format.c
 */
 
char *
 strdup(const char *string)
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

#define FLAG_SIGN		0x01	// Always show sign
#define FLAG_LEFT		0x02	// Left align
#define FLAG_ALT		0x04	// Alternate form
#define FLAG_ZERO		0x08	// Zero-pad
#define FLAG_UPPER		0x10	// Upper case
#define FLAG_OCTAL		0x20	// Octal output
#define FLAG_HEX		0x40	// Hexadecimal output
#define FLAG_EXP		0x80	// Use scientific notation
#define FLAG_MEXP		0x100	// Maybe use scientific notation

#define TYPE_STRING		0		// String
#define TYPE_CHAR		1		// Character
#define TYPE_LITERAL	2		// Literal '%"
#define TYPE_INT		3		// Signed integer
#define TYPE_UINT		4		// Unsigned integer
#define TYPE_DOUBLE		5		// Floating point

#define LENGTH_BYTE		0		// char
#define LENGTH_SHORT	1		// short int
#define LENGTH_LONG		3		// int or double
#define LENGTH_LLONG	4		// long long int or long double

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

char *__format(const char **_fmt, va_list *argp) {
	int flags, type, length;
	size_t width, precision;
	const char *fmt = *_fmt;
	char *string;
	char *string1;

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
	case 'X': case 'p': flags |= FLAG_UPPER;
	case 'x':
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

	/* format value */
	switch (type) {
	case TYPE_STRING:
		if (!val_s) string = strdup("(null)");
		else string = strdup(val_s);
		break;
	case TYPE_CHAR: case TYPE_LITERAL:
		string = malloc(sizeof(char) * 2);
		string[0] = val_c;
		string[1] = '\0';
		break;
	case TYPE_INT:
		string = __format_int(val_i, flags);
		break;
	case TYPE_UINT:
		string = __format_uint(val_u, flags);
		break;
	case TYPE_DOUBLE:
		string = __format_double(val_d, flags, precision);
		break;
	}

	if (strlen(string) < width) {
		string1 = malloc(sizeof(char) * (width + 1));

		if (flags & FLAG_LEFT) {
			strlcpy(string1, string, width + 1);

			if (flags & FLAG_ZERO) {
				strlcat(string1, "000000000000000000000000", width + 1);
			}
			else {
				strlcat(string1, "                        ", width + 1);
			}
		}
		else {
			if (flags & FLAG_ZERO) {
				strlcpy(string1, "000000000000000000000000", width - strlen(string) + 1);
			}
			else {
				strlcpy(string1, "                        ", width - strlen(string) + 1);
			}

			strlcat(string1, string, width + 1);
		}

		free(string);
	}
	else {
		string1 = string;
	}

	*_fmt = fmt;
	return string1;
}

size_t stream_write(stream_s *stream, const void *ptr, size_t size)
{
	unsigned char *p = (unsigned char *) ptr;
	size_t i;
	for (i = 0; i < size; i++) {
		if (stream->write(stream, *p) == EOF)
			break;
		p++;
	}
	return i;
}

int stream_vformat(stream_s *stream, const char *format, va_list ap)
{
	size_t i, fbt;
	char *fmtbuffer;
	const char *format_tmp;
	char *string;

	fmtbuffer = malloc(sizeof(char) * (strlen(format) + 1));
	fbt = 0;

	for (i = 0; format[i]; i++) {
		if (format[i] == '%') {
			stream_write(stream, fmtbuffer, fbt);
			fbt = 0;

			format_tmp = &format[i];
			string = __format(&format_tmp, &ap);
			i = (uintptr_t) format_tmp - (uintptr_t) format;
			if (!string)
				continue;
			
			//fwrite(string, strlen(string), sizeof(char), stream);
			stream_write(stream, string, strlen(string));
			free(string);
		}
		else {
			fmtbuffer[fbt++] = format[i];
		}
	}
	
	//fwrite(fmtbuffer, fbt, sizeof(char), stream);
	stream_write(stream, fmtbuffer, fbt);
	free(fmtbuffer);

	return 0;
} 

/*
int stream_format(stream_s *stream, concat char * format, ...)
{
	
}
*/

/* put a string */
size_t stream_puts(stream_s *stream, const char *text)
{
	size_t i;
	for (i = 0; i < strlen(text); i++)
		stream_putc(stream, text[i]);
	return 1;
}

/* put a decimal integer */
void stream_puti(stream_s *stream, unsigned int a)
{
	do {
		/* print highest digit */
		unsigned int b = a, d = 1;
		while ((b / 10) > 0) {
			d *= 10;
			b /= 10;
		}
		stream_putc(stream, 0x30 + b);
	
		/* print rest */
		while (a >= d)
			a -= d;
	} while (a > 0);
}

/* put a hex char */
void stream_puthc(stream_s *stream, unsigned char h)
{
	int b = h & 0xF, a = (h >> 4) & 0xF;
	stream_putc(stream, a > 0x9 ? 0x61 - 0x0A + a : 0x30 + a);
	stream_putc(stream, b > 0x9 ? 0x61 - 0x0A + b : 0x30 + b);
}

/* put a pointer */
void stream_putp(stream_s *stream, void *p)
{
	int i, siz = sizeof(void *);
	stream_puts(stream, "0x");
	for (i = siz - 1; i >= 0; i--) {
		stream_puthc(stream, (((int) p) >> (i*8)) & 0xFF);
	}
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

static int bytestream_read(stream_s *stream)
{
	bytestream_s *data = (bytestream_s *) stream->data;
	if (data->pos >= data->size)
		return EOF;
	return data->buf[data->pos++];
}

static int bytestream_write(stream_s *stream, unsigned char c)
{
	bytestream_s *data = (bytestream_s *) stream->data;
	
	// expand file size
	if (data->pos >= data->capacity) {
		data->capacity += 1024;
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
	stream->read = bytestream_read;
	stream->write = bytestream_write;
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
