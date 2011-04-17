#ifndef __SETJMP_H
#define __SETJMP_H

typedef struct 
{
  unsigned long ebp;
  unsigned long ebx;
  unsigned long edi;
  unsigned long esi;
  unsigned long esp;
  unsigned long eip;
} jmp_buf[1];

extern int setjmp(jmp_buf env);
extern void longjmp(jmp_buf env, int value);

#endif
