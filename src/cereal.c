#include <system.h>
#include <stream.h>
#include <string.h>

#define PORT 0x3f8   /* COM1 */
#define SERIAL_BUF_SIZE 8
//1024*4
 
// flag if serial data has been received
int serial_received()
{
   return inportb(PORT + 5) & 1;
}

// flag if transit is clear to send data
int serial_transit_empty()
{
   return inportb(PORT + 5) & 0x20;
}
 
unsigned char serial_read()
{
   while (serial_received() == 0);
   return inportb(PORT);
}
 
void serial_write(unsigned char a)
{
   while (serial_transit_empty() == 0); 
   outportb(PORT, a);
}

unsigned char serial_buf[SERIAL_BUF_SIZE];
int serial_buf_len = 0;
void (*serial_handler)(unsigned char *buf, long int size) = NULL;

void serial_set_handler(void (*callback)(unsigned char *buf, long int size))
{
	serial_handler = callback;
}

void serial_interrupt(struct regs *r)
{
	(void) r;
	if (serial_received()) {
		serial_buf[serial_buf_len] = serial_read();
		if (serial_buf_len == SERIAL_BUF_SIZE) {
			if (serial_handler != NULL)
				serial_handler(serial_buf, SERIAL_BUF_SIZE);
			serial_buf_len = 0;
		} else
			serial_buf_len++;
	}
}

void serial_install()
{
	outportb(PORT + 1, 0x01);    // enable Received Data Available interrupt
	outportb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
	outportb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
	outportb(PORT + 1, 0x00);    //                  (hi byte)
	outportb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
	outportb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
	outportb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set

	/* Installs 'serial_handler' to IRQ4 */
	irq_install_handler(4, serial_interrupt);
}

/*
 * serial stream implementation
 */

int serialstream_read(stream_s *stream)
{
	(void) stream;
	
	// read from serial buffer
	if (serial_buf_len == 0)
		return EOF;
	unsigned char c = serial_buf[0];
	memcpy(serial_buf, serial_buf + 1, --serial_buf_len);
	return c;
}

int serialstream_write(stream_s *stream, unsigned char c)
{
	(void) stream;
	serial_write((char) c);
	return (int) c;
}

int serialstream_seek(stream_s *stream, long pos, int origin)
{
	(void) stream; (void) pos; (void) origin;
	return 0;
}

stream_s serialstream_s = { serialstream_read, serialstream_write, serialstream_seek, NULL };
stream_s *serialstream = &serialstream_s;

