/*
 * Jiauliyan OS - Released under the MIT License
 * Copyright (C) 2011 Paul Booth, Jialiya Huang, Tim Ryan
 * https://github.com/timcameronryan/jiauliyan
 * 
 * Based on code from the OSDev wiki
 * http://wiki.osdev.org/Serial_ports
 */

#include <system.h>
#include <stream.h>
#include <string.h>
#include <common.h>

#define PORT 0x3f8   /* COM1 */
#define SERIAL_BUF_SIZE 1024*128

/*
 * serial port
 */
 
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
 
unsigned char serial_get()
{
   while (serial_received() == 0);
   return inportb(PORT);
}
 
void serial_put(unsigned char a)
{
   while (serial_transit_empty() == 0); 
   outportb(PORT, a);
}

/*
 * interrupt/buffer
 */

unsigned char serial_buf[SERIAL_BUF_SIZE];
int serial_buf_len = 0;
void (*serial_handler)(unsigned char *buf, long int size) = NULL;

void serial_set_handler(void (*callback)(unsigned char *buf, long int size))
{
	serial_handler = callback;
}

void serial_flush()
{
	if (serial_buf_len > 0) {
		disable_interrupts();
		if (serial_handler != NULL)
			serial_handler(serial_buf, serial_buf_len);
		serial_buf_len = 0;
		enable_interrupts();
	}
}

void serial_interrupt(struct regs *r)
{
	UNUSED(r);
	
	if (serial_received()) {
		serial_buf[serial_buf_len++] = serial_get();
		if (serial_buf_len == SERIAL_BUF_SIZE)
			serial_flush();
	}
}

/*
 * serial in implementation
 */

int serialin_get(stream_s *stream)
{
	UNUSED(stream);
	// read from serial buffer
	if (serial_buf_len == 0)
		return EOF;
	unsigned char c = serial_buf[0];
	memcpy(serial_buf, serial_buf + 1, --serial_buf_len);
	return c;
}

long int serialin_avail(stream_s *stream)
{
	UNUSED(stream);
	return serial_buf_len;
}

int serialout_put(stream_s *stream, unsigned char c)
{
	UNUSED(stream);
	serial_put((char) c);
	return (int) c;
}

stream_s *serialout;
stream_s *serialin;

/*
 * install
 */

void serial_install()
{
	outportb(PORT + 1, 0x01);    // enable Received Data Available interrupt
	outportb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
	outportb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
	outportb(PORT + 1, 0x00);    //                  (hi byte) * 3
	outportb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
	outportb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
	outportb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set

	/* Installs 'serial_handler' to IRQ4 */
	irq_install_handler(4, serial_interrupt);
	
	// create streams
	serialout = stream_create(stream_no_get, serialout_put, stream_no_avail, stream_no_seek, NULL);
	serialin = stream_create(serialin_get, stream_no_put, serialin_avail, stream_no_seek, NULL);
}
