#include <system.h>
#include <stream.h>
#include <string.h>

/* These define our textpointer, our background and foreground
 *  colors (attributes), and x and y cursor coordinates */
unsigned short *textmemptr;
int attrib = 0x0F;
int csr_x = 0, csr_y = 0;

/* Scrolls the screen */
void scroll(void)
{
	unsigned blank, temp;

	/* A blank is defined as a space... we need to give it
	 *  backcolor too */
	blank = 0x20 | (attrib << 8);

	/* Row 25 is the end, this means we need to scroll up */
	if(csr_y >= 25)
	{
		/* Move the current text chunk that makes up the screen
		 *  back in the buffer by a line */
		temp = csr_y - 25 + 1;
		memcpy(textmemptr, textmemptr + temp * 80, (25 - temp) * 80 * 2);

		/* Finally, we set the chunk of memory that occupies
		 *  the last line of text to our 'blank' character */
		memsetw(textmemptr + (25 - temp) * 80, blank, 80);
		csr_y = 25 - 1;
	}
}

/* Updates the hardware cursor: the little blinking line
 *  on the screen under the last character pressed! */
void move_csr(void)
{
	unsigned temp;

	/* The equation for finding the index in a linear
	 *  chunk of memory can be represented by:
	 *  Index = [(y * width) + x] */
	temp = csr_y * 80 + csr_x;

	/* This sends a command to indicies 14 and 15 in the
	 *  CRT Control Register of the VGA controller. These
	 *  are the high and low bytes of the index that show
	 *  where the hardware cursor is to be 'blinking'. To
	 *  learn more, you should look up some VGA specific
	 *  programming documents. A great start to graphics:
	 *  http://www.brackeen.com/home/vga */
	outportb(0x3D4, 14);
	outportb(0x3D5, temp >> 8);
	outportb(0x3D4, 15);
	outportb(0x3D5, temp);
}

/* Clears the screen */
void vga_cls()
{
	unsigned blank;
	int i;

	/* Again, we need the 'short' that will be used to
	 *  represent a space with color */
	blank = 0x20 | (attrib << 8);

	/* Sets the entire screen to spaces in our current
	 *  color */
	for(i = 0; i < 25; i++)
		memsetw (textmemptr + i * 80, blank, 80);

	/* Update out virtual cursor, and then move the
	 *  hardware cursor */
	csr_x = 0;
	csr_y = 0;
	move_csr();
}

/* Puts a single character on the screen */
void vga_putchar(char c)
{
	unsigned short *where;
	unsigned att = attrib << 8;

	/* Handle a backspace, by moving the cursor back one space */
	if(c == 0x08)
	{
		if(csr_x != 0) csr_x--;
	}
	/* Handles a tab by incrementing the cursor's x, but only
	 *  to a point that will make it divisible by 8 */
	else if(c == 0x09)
	{
		csr_x = (csr_x + 8) & ~(8 - 1);
	}
	/* Handles a 'Carriage Return', which simply brings the
	 *  cursor back to the margin */
	else if(c == '\r')
	{
		csr_x = 0;
	}
	/* We handle our newlines the way DOS and the BIOS do: we
	 *  treat it as if a 'CR' was also there, so we bring the
	 *  cursor to the margin and we increment the 'y' value */
	else if(c == '\n')
	{
		csr_x = 0;
		csr_y++;
	}
	/* Any character greater than and including a space, is a
	 *  printable character. The equation for finding the index
	 *  in a linear chunk of memory can be represented by:
	 *  Index = [(y * width) + x] */
	else if(c >= ' ')
	{
		where = textmemptr + (csr_y * 80 + csr_x);
		*where = c | att;	/* Character AND attributes: color */
		csr_x++;
	}

	/* If the cursor has reached the edge of the screen's width, we
	 *  insert a new line in there */
	if(csr_x >= 80)
	{
		csr_x = 0;
		csr_y++;
	}

	/* Scroll the screen if needed, and finally move the cursor */
	scroll();
	move_csr();
}

/* Sets the forecolor and backcolor that we will use */
void vga_setbg(unsigned char bg)
{
	// upper nibble is bg
	attrib = ((bg & 0x0F) << 4) | (attrib & 0x0F);
}

void vga_setfg(unsigned char fg)
{
	// lower nibble is fg
	attrib = (attrib & 0xF0) | (fg & 0x0F);
}

/* Sets our text-mode VGA pointer, then clears the screen for us */
void vga_init(void)
{
	textmemptr = (unsigned short *)0xB8000;
	vga_cls();
}

/*
 * vga stream implementation
 */

int vgastream_read(stream_s *stream)
{
	(void) stream;
	return EOF;
}

int vgastream_write(stream_s *stream, unsigned char c)
{
	(void) stream;
	vga_putchar((char) c);
	return (int) c;
}

int vgastream_seek(stream_s *stream, long pos, int origin)
{
	(void) stream; (void) pos; (void) origin;
	return 0;
}

stream_s vgastream_s = { vgastream_read, vgastream_write, vgastream_seek, NULL };
stream_s *vgastream = &vgastream_s;
