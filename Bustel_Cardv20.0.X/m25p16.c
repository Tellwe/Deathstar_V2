#include "m25p16.h"
#include "variables.h"
#include <xc.h>
#include "functions.h"


/* Support for M25P16 2Mbyte flash RAM 


BEWARE  Programming can only set bits to zero. IT CANNOT CHANGE A ZERO TO A ONE
You must use the Bulk or sector erase to set bits back to a one


	RAM_PORT		PORT for the RAM CS signal
	RAM_CS


and RAM registers

	RAM_RDID
	RAM_WREN
	RAM_BE
	RAM_PP
	RAM_RDSR
	
and of functions:

	xmit_spi
	recv_spi

Additionally, RAM_CS must be defined as an output on the appropriate DDR

*/


void read_ram_id(unsigned char* mem_ptr) {

/*
	RAM_PORT &= ~(1<<RAM_CS);			// assert cs 
	xmit_spi(RAM_RDID);
	mem_ptr[0] = recv_spi();
	mem_ptr[1] = recv_spi();
	mem_ptr[2] = recv_spi();
	RAM_PORT |= 1<<RAM_CS;				// deassert cs 
*/
	memCE = 0;
	WriteSPI(RAM_RDID);
	mem_ptr[0] = ReadSPI();
	mem_ptr[1] = ReadSPI();
	mem_ptr[2] = ReadSPI();
	memCE = 1;

}



unsigned char read_ram_status(void) {

	unsigned char status;
	memCE = 0;
	WriteSPI(RAM_RDSR);
	status = ReadSPI();
	memCE = 1;
	return status;

}


void ram_bulk_erase(void) {

	memCE = 0;
	WriteSPI(RAM_WREN);
	memCE = 1;
	__delay_us(1);
	memCE = 0;
	WriteSPI(RAM_BE);
	memCE = 1;
	while(read_ram_status());
}


void ram_sector_erase(unsigned char sector) {

	memCE = 0;
	WriteSPI(RAM_WREN);
	memCE = 1;
	__delay_us(1);
	memCE = 0;
	WriteSPI(RAM_SE);
	WriteSPI(sector);
	WriteSPI(0x00);
	WriteSPI(0x00);
	memCE = 1;
	while(read_ram_status());

}

void write_flash_page(unsigned char flash_sector,unsigned char flash_page,unsigned char* mem_ptr) {
	read_write_flash_ram(0,256,flash_sector,flash_page,0,mem_ptr);
}
void read_flash_page(unsigned char flash_sector,unsigned char flash_page,unsigned char* mem_ptr) {
	read_write_flash_ram(1,256,flash_sector,flash_page,0,mem_ptr);
}



void read_write_flash_ram(unsigned char one_read_zero_write,unsigned short int bytes_to_readwrite,unsigned char flash_sector,unsigned char flash_page,unsigned char offset,unsigned char* mem_ptr) {

// NB CAUTION page writes which cross page boundaries will wrap 


// parameters 

// one_read_zero_write = 1 for read, 0 for write 
// bytes_to_readwrite to read or write 
// flash sector within device 
// flash page within device 
// offset for first byte to transfer 
// POINTER TO ram address for first byte to transfer 


	unsigned short int i;

// for ram device, enter and leave with SCK low 

	memCE = 0;
	if(one_read_zero_write)
	{
		WriteSPI(RAM_READ);
	}	
	else
	{
		WriteSPI(RAM_WREN);
		memCE = 1;
		__delay_us(1);
		memCE = 0;
		WriteSPI(RAM_PP);

	}
	
	WriteSPI(flash_sector);
	WriteSPI(flash_page);
	WriteSPI(offset);

	for (i=0;i<bytes_to_readwrite;i++) 
	{
		if (one_read_zero_write) 
		{
			mem_ptr[i] = ReadSPI();
		} 
		else 
		{
			WriteSPI(mem_ptr[i]);
		}
	}	

	memCE = 1;
	
	__delay_us(1);
	while (read_ram_status() & 0b00000011) {
		__delay_ms(1); 
	}


}


// write to the RAM status byte. 0 in bottom bit position = ready 
void write_ram_status(unsigned char status) {


	memCE = 0;
	WriteSPI(RAM_WREN);
	memCE = 1;			// deassert cs 
	__delay_us(2); 
	memCE = 0;			// assert cs 
	WriteSPI(RAM_WRSR);
	WriteSPI(status);
	memCE = 1;				// deassert cs 
	__delay_us(2);
	while (read_ram_status() & 0x01);	

}


void power_up_flash_ram(void) {

	memCE = 0;			// assert cs 
	WriteSPI(RAM_RES);
	memCE = 1;				// deassert cs 
	__delay_us(30);
}


void power_down_flash_ram(void) {

	memCE = 0;			// assert cs 
	WriteSPI(RAM_DP);
	memCE = 1;				// deassert cs 
}

