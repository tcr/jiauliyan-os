#include <system.h>
#include <string.h>
#include <vga.h>

void kmain( void* mbd, unsigned int magic )
{
	if ( magic != 0x2BADB002 )
	{
		/* Something went not according to specs. Print an error */
		/* message and halt, but do *not* rely on the multiboot */
		/* data structure. */
	}
	
    gdt_install();
    idt_install();
    isrs_install();
    irq_install();
    init_video();
    timer_install();
    timer_phase(100);
    keyboard_install();

    __asm__ __volatile__ ("sti"); /* start interrupts */
	
	/* You could either use multiboot.h */
	/* (http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#multiboot_002eh) */
	/* or do your offsets yourself. The following is merely an example. */ 
	char * boot_loader_name = (char*) ((long*)mbd)[16];
	
	settextcolor(LIGHT_BROWN, MAGENTA);
	putscrni(56);
	putscrni(1232);
	putscrns("Hello OSWORLD!!!\n");
	
	timer_wait(500);
	
	putscrns("TIMER DONE!!!");

	/* Write your kernel here. */
	for(;;);
}

/* put these somewhere else eventually */
char * reverse( char * s)
{
	int i, len;
	char temp;
	len = strlen(s);
	for ( i=0; i < len/2; i++) {
		temp = s[i];
		s[i] = s[len - 1 - i]; /*i=0 -> length -1 not outside bounds*/
		s[len - 1 - i] = temp;
	}
	return s;
}

char * itoa( int i, char * s, int pos) 
{
	if (i == 0) 
		return s;
	itoa(i / 10, s, pos + 1);
	s[pos] = (char) ( (i % 10)  + 48);
	return s;
}
