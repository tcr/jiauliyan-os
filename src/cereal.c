#include <system.h>
#define PORT 0x3f8   /* COM1 */
 
int serial_received() {
   return inportb(PORT + 5) & 1;
}
 
char read_serial() {
   while (serial_received() == 0);
 
   return inportb(PORT);
}

int is_transmit_empty() {
   return inportb(PORT + 5) & 0x20;
}
 
void write_serial(char a) {
   while (is_transmit_empty() == 0);
 
   outportb(PORT,a);
}

void serial_handler(struct regs *r) {
	if (serial_received()) {
		putscrnc(read_serial());
	}
}

void serial_install() {
	outportb(PORT + 1, 0x01);    // enable Received Data Available interrupt
	outportb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
	outportb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
	outportb(PORT + 1, 0x00);    //                  (hi byte)
	outportb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
	outportb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
	outportb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set

	/* Installs 'serial_handler' to IRQ4 */
	irq_install_handler(4, serial_handler);
}
