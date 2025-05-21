#include <xc.h>
#include "clcd.h"

void clcd_write(unsigned char byte, unsigned char control_bit)
{
	CLCD_RS = control_bit;//RC1 -> RS = *****
	CLCD_PORT = byte;//PORTD -> DB0 to DB7 = *****

	/* Should be atleast 200ns */
	CLCD_EN = HI;//RC2 ->   E  = 1
	CLCD_EN = LO;//RC2 ->   E  = 0

	PORT_DIR = INPUT;//TRISD7 -> input
	CLCD_RW = HI;//RC0 -> RW = 1
	CLCD_RS = INSTRUCTION_COMMAND;//RC1 -> RS = 0

	do
	{
		CLCD_EN = HI;//E = 1
		CLCD_EN = LO;//E = 0
	} while (CLCD_BUSY);//RD7

	CLCD_RW = LO;//RW = 0 //write operation for to send data
	PORT_DIR = OUTPUT;//TRISD7 = 0
}

void init_clcd()
{
	/* Set PortD as output port for CLCD data */
	TRISD = 0x00;
	/* Set PortC as output port for CLCD control */
	TRISC = TRISC & 0xF8;
	CLCD_RW = LO;//write//RC0->RW = 0;
     /* Startup Time for the CLCD controller */
    __delay_ms(30);//blocking delay
    /* The CLCD Startup Sequence */
    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND	);//0x33(don't care * either 0 or 1(0x30/0x31/0x32/0x33))  ,  RS=0
    __delay_us(4100);//4.1 milli seconds
    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND	);//same as above
    __delay_us(100);//100us
    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND	);//same as above
    __delay_us(1); //not mentioned in data sheet //for our configuration
    CURSOR_HOME;
    __delay_us(100);//not required
    TWO_LINE_5x8_MATRIX_8_BIT;
    __delay_us(100);//not required
    CLEAR_DISP_SCREEN;
    __delay_us(500);//not required
    DISP_ON_AND_CURSOR_OFF;
    __delay_us(100);//not required
}

void clcd_print(const unsigned char *data, unsigned char addr)//entire string(base address,address where to print)
{
	clcd_write(addr, INSTRUCTION_COMMAND);//0  where to print
	while (*data != '\0')
	{
		clcd_write(*data++, DATA_COMMAND);//1  display data
	}
}

void clcd_putch(const unsigned char data, unsigned char addr)//one character
{
	clcd_write(addr, INSTRUCTION_COMMAND);//0  where to print
	clcd_write(data, DATA_COMMAND);//1  display data
}
