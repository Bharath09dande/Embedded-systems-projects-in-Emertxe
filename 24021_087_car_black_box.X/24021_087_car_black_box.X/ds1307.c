
#include "main.h"
#include "ds1307.h"
#include "i2c.h"
#include <xc.h>
#include "ds1307.h"
/* 
 * DS1307 Slave address
 * D0  -  Write Mode
 * D1  -  Read Mode
 */

void init_ds1307(void)
{
    unsigned char dummy;
    dummy = read_ds1307(SEC_ADDR);
    dummy = dummy & 0x7F;
    write_ds1307(SEC_ADDR, dummy);
}

void write_ds1307(unsigned char address, unsigned char data)
{
	i2c_start();
	i2c_write(SLAVE_WRITE);
	i2c_write(address);
	i2c_write(data);
	i2c_stop();
}

unsigned char read_ds1307(unsigned char address)
{
	unsigned char data;

	i2c_start();
	i2c_write(SLAVE_WRITE);
	i2c_write(address);
	i2c_rep_start();
	i2c_write(SLAVE_READ);
	data = i2c_read();
	i2c_stop();

	return data;
}