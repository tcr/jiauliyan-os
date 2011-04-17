#include <system.h>
#include <string.h>
#include <vga.h>
#include <stdio.h>

void kernel_serial_handler(unsigned char *buf, long int size)
{
	int i;
	for (i = 0; i < size; i++)
		vga_putchar((char) buf[i]);
}

void kernel_start()
{
	serial_set_handler(kernel_serial_handler);
	
	vga_setbg(BLUE);
	vga_cls();
	
	vga_setfg(LIGHT_BROWN);
	puts("Jiauliyan OS\n");
	vga_setfg(WHITE);
	puts("An OS by some cool dudes\n\n");

	puts("Address of kernel_start(): ");
	stream_putp(vgastream, &kernel_start);
	putchar('\n');

	puts("Writing to serial port...\n");
	stream_puts(serialstream, "[SERIAL] Testing serial ports from Jiauliyan OS!\n");

	puts("Waiting timer for 300 clicks:\n");
	timer_wait(300);	
	puts("300 click timer DONE!\n");
	putchar('\n');
	
	puts("Testing input. Serial input is ");
	vga_setfg(LIGHT_RED);
	puts("red");
	vga_setfg(WHITE);
	puts(" and keyboard input is ");
	vga_setfg(LIGHT_GREEN);
	puts("green");
	vga_setfg(WHITE);
	puts(".\n");

	/* Write your kernel here. */
	int c;
	for(;;) {
		//vga_setfg(LIGHT_RED);
		//while ((c = serialstream->read(serialstream)) != EOF)
		//	vga_putchar((char) c);
	}
}

void kmain( void* mbd, unsigned int magic )
{
	if ( magic != 0x2BADB002 )
	{
		/* Something went not according to specs. Print an error */
		/* message and halt, but do *not* rely on the multiboot */
		/* data structure. */
	}
	
	/* You could either use multiboot.h */
	/* (http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#multiboot_002eh) */
	/* or do your offsets yourself. The following is merely an example. */ 
	char *boot_loader_name = (char *) ((long *) mbd)[16];
	(void) boot_loader_name; // use this eventually
	
	// global interrupts table
    gdt_install();
    idt_install();
    isrs_install();
    irq_install();
    
    // services
    vga_init();
    timer_install();
    timer_phase(100);
    keyboard_install();
	serial_install();
	interrupts_init();
	
	// init std(in|out|err)
	stdio_init();
	
	// start kernel
	kernel_start();
}

/* put these somewhere else eventually */
char *reverse( char * s)
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

char *itoa( int i, char * s, int pos) 
{
	if (i == 0) 
		return s;
	itoa(i / 10, s, pos + 1);
	s[pos] = (char) ( (i % 10)  + 48);
	return s;
}
