#ifndef __STDDEF_H
#define __STDDEF_H

#include <stdint.h>

/* reference: http://www.acm.uiuc.edu/webmonkeys/book/c_guide/2.11.html */

typedef intptr_t ptrdiff_t;
typedef uint32_t size_t;
typedef uint32_t wchar_t;

#ifndef NULL
#define NULL 0
#endif

#endif
