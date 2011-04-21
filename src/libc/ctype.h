#ifndef __CTYPE_H
#define __CTYPE_H

#define isalnum(c)  (isalpha(c) || isdigit(c))
#define isalpha(c)  (isupper(c) || islower(c))
#define isascii(c)  (c > 0 && c <= 0x7f)
#define iscntrl(c)  ((c >= 0) && ((c <= 0x1F) || (c == 0x7f)))
#define isdigit(c)  (c >= '0' && c <= '9')
#define isgraph(c)  (c != ' ' && isprint(c))
#define islower(c)  (c >=  'a' && c <= 'z')
#define isprint(c)  (c >= ' ' && c <= '~')
#define ispunct(c)  ((c > ' ' && c <= '~') && !isalnum(c))
#define isspace(c)  (c ==  ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v')
#define isupper(c)  (c >=  'A' && c <= 'Z')
#define isxdigit(c) (isxupper(c) || isxlower(c))
#define isxlower(c) (isdigit(c) || (c >= 'a' && c <= 'f'))
#define isxupper(c) (isdigit(c) || (c >= 'A' && c <= 'F'))
#define tolower(c)  (isupper(c) ? ( c - 'A' + 'a') : (c))
#define toupper(c)  (islower(c) ? (c - 'a' + 'A') : (c))

#endif
