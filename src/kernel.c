#include <system.h>

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHT_GREY 7
#define DARK_GREY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_CYAN 11
#define LIGHT_RED 12
#define LIGHT_MAGENTA 13
#define LIGHT_BROWN 14
#define WHITE 15

int textColors(int foreground, int background){
	return (foreground<<4) + background;
}

/* You will need to code these up yourself!  */
unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count)
{
	/* Add code here to copy 'count' bytes of data from 'src' to
	 *  'dest', finally return 'dest' */
	int i = 0;
	for (i = 0; i < count; i++) {
		dest[i] = src[i];
	}
	return dest;

}

unsigned char *memset(unsigned char *dest, unsigned char val, int count)
{
	/* Add code here to set 'count' bytes in 'dest' to 'val'.
	 *  Again, return 'dest' */
	int i = 0;
	for (i = 0; i < count; i++) {
		dest[i] = val;
	}
	return dest;
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, int count)
{
	/* Same as above, but this time, we're working with a 16-bit
	 *  'val' and dest pointer. Your code can be an exact copy of
	 *  the above, provided that your local variables if any, are
	 *  unsigned short */
	
	unsigned short i = 0;
	for (i = 0; i < count; i++) {
		dest[i] = val;
	}
	return dest;
}

int strlen(const char *str)
{
    /* This loops through character array 'str', returning how
    *  many characters it needs to check before it finds a 0.
    *  In simple words, it returns the length in bytes of a string */
    int i = 0;
    while(str[i] != '\0'){
		i++;
	}
	return i;
}

/* We will use this later on for reading from the I/O ports to get data
 *  from devices such as the keyboard. We are using what is called
 *  'inline assembly' in these routines to actually do the work */
unsigned char inportb (unsigned short _port)
{
	unsigned char rv;
	__asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
	return rv;
}

/* We will use this to write to I/O ports to send bytes to devices. This
 *  will be used in the next tutorial for changing the textmode cursor
 *  position. Again, we use some inline assembly for the stuff that simply
 *  cannot be done in C */
void outportb (unsigned short _port, unsigned char _data)
{
	__asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
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
	char * boot_loader_name =(char*) ((long*)mbd)[16];

	/* Print a letter to screen to see everything is working: */
	unsigned char *videoram = (unsigned char *) 0xb8000;

	const char char_A = 65;
	
	char test_string[15] = "jialiyahuahauah";
	
	const int SCREEN_WIDTH = 80;
	const int SCREEN_HEIGHT = 25;
	
	init_video();
	puts("Hello OSWORLD!!!");
/* 
	
	int max_x = strlen(test_string);
	int x, y;
	for (y = 0; y < SCREEN_HEIGHT; y++) {
		for (x = 0; x < max_x; x++) {
			videoram[y*2*SCREEN_WIDTH + x*2] = char_A;	/* character 'A' */
			videoram[y*2*SCREEN_WIDTH + x*2+1] = textColors(LIGHT_BROWN, MAGENTA);	/* forground, background color. */
		}
	}
*/
	/* Write your kernel here. */
}
