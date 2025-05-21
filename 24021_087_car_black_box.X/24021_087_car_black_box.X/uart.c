#include <xc.h>
#include "uart.h"

void init_uart(void)
{
	/* Serial initialization */
	RX_PIN = 1;
	TX_PIN = 0;

	/* TXSTA:- Transmitor Status and control Register */
	/* 9bit TX enable or disable bit */ 
	TX9 = 0;//Selects 8-bit transmission
	/* UART Tarsmition enable bit */
	TXEN = 1;
	/* Synchronous or Asynchronous mode selection */
	/* Asynchronous */
	SYNC = 0;
	/* Send the Break character bit */
	SENDB = 0;//to send the data continuosly 
	/* Low or High baud rate selection bit */
	/* High Baud Rate */
	BRGH = 1;

	/* RCSTA :- Recepition Status and control Register */
	/* TX/RC7 and RX/RC6 act as serial port */ 
	SPEN = 1;
	/* 9bit RX enable or disable bit */
	RX9 = 0;//no parity
	/* Continous reception enable or disable */ 
	CREN = 1;

	/* BAUDCTL:- Baud rate control register */
	/* 16bit baud generate bit */ 
	BRG16 = 0;//8-bit for high speed baud rate
	
	/* Baud Rate Setting Register */
	/* Set to 10 for 115200, 64 for 19200 and 129 for 9600 */
	SPBRG = 129;

	/* TX interrupt flag bit */
	TXIF = 0;
	/* RX interrupt enable bit */
	RCIF = 0;
}

void putch(unsigned char byte) //to print on teratermdd
{
	/* Output one byte */
	/* Set when register is empty */
	while(!TXIF)//1 means completed
	{
		continue;
	}
	TXIF = 0;
	TXREG = byte;//storing data
} 

int puts(const char *s)
{
	while(*s)//upto null character	
	{
		putch(*s++);	
	}
	return 0;
}

unsigned char getch(void)//reading from the teraterm
{
	/* Retrieve one byte */
	/* Set when register is not empty */
	while(!RCIF)//status flag//1 means completed
	{
		continue;
	}
	RCIF = 0;
	return RCREG;//storing data
}

unsigned char getche(void)//combination of putch and getch
{
	unsigned char c;

	putch(c = getch());//reading and printing at a time

	return (c);
}
