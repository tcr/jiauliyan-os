#ifndef __ERRNO_H
#define __ERRNO_H

extern int errno;

// parameter outside a function's domain, eg sqrt(-1)
#define EDOM 1
// result outside a function's range, eg strtol("0xfffffffff",NULL,0)
#define ERANGE 2
// illegal character sequence, eg wcstombs(str, L"\xffff", 2)
#define EILSEQ 3

#endif
