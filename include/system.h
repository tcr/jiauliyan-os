#ifndef __SYSTEM_H
#define __SYSTEM_H

/* ports.c */

extern unsigned char inportb (unsigned short _port);
extern void outportb (unsigned short _port, unsigned char _data);

/* gdt.c */

extern void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
extern void gdt_install();

/* idt.c */

extern void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
extern void idt_install();

/* isrs.c */

extern void isrs_install();

/* This defines what the stack looks like after an ISR was running */
struct regs
{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};

/* irq.c */

extern void irq_install();
extern void irq_uninstall_handler(int irq);
extern void irq_install_handler(int irq, void (*handler)(struct regs *r));

/* timer.c */

extern void timer_wait(unsigned int ticks);
extern void timer_install();
extern void timer_phase(int hz);

/* keyboard.c */

extern void keyboard_install();

/* cereal ports */

extern int serial_received();
extern char read_serial();
extern int is_transmit_empty();
extern void write_serial(char a);
extern void serial_handler();
extern void serial_install();

/* interrupts */

#define interrupts_init() __asm__ __volatile__ ("sti")

#endif
