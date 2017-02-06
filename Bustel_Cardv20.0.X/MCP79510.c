#include "MCP79510.h"
#include "variables.h"
#include <xc.h>
#include "functions.h"

void ClockInit()
{
	csClock = 0;
	WriteSPI(0x12);
	WriteSPI(0x01);
	WriteSPI(0x00);
	csClock = 1;		
   	__delay_us(40);
	csClock = 0;
	WriteSPI(0x12);
	WriteSPI(0x02);
	WriteSPI(0x00);
	csClock = 1;		
   	__delay_us(40);
	csClock = 0;
	WriteSPI(0x12);
	WriteSPI(0x08);
	WriteSPI(0x00);
	csClock = 1;		
	__delay_us(40);
	csClock = 0;
	WriteSPI(0x12);
	WriteSPI(0x01);
	WriteSPI(0x80);
	csClock = 1;

	while(1)
	{
		//TODO: FIX POTENTIAL ENDLESS LOOP!!!!
		csClock = 0;
		WriteSPI(0x13);
		WriteSPI(0x04);
		if((ReadSPI() & 0b00100000) != 0)
		{
			csClock = 1;
			break;
		
		}
		
	}		

}
void ReadClock(unsigned char* seconds, unsigned char* minutes, unsigned char* hour, unsigned char* date, unsigned char* month, unsigned char *year)
{
	unsigned char sec10, min10, hour10, date10, month10, year10, sec1, min1, hour1, date1, month1, year1;

	csClock = 0;

	WriteSPI(0x13); // Read Instruction
	WriteSPI(0x01); //Adress
	
	sec1 = ReadSPI() & 0x7F;
	min1 = ReadSPI() & 0x7F;
	hour1 = ReadSPI() & 0x3F;
	date1 = ReadSPI(); //Day, read and then overwrite with the date. Day not used
	date1 = ReadSPI() & 0x3F; 
	month1 = ReadSPI() & 0x1F;
	year1 = ReadSPI();

	csClock = 1;


	sec10 = (sec1 & 0x70) >> 4;
	sec1 = sec1 & 0x0F;
	*seconds = sec1 + sec10 * 10;	

	min10 = (min1 & 0x70) >> 4;
	min1 = min1 & 0x0F;
	*minutes = min1 + min10 * 10;
	
	hour10 = (hour1 & 0x30) >> 4;
	hour1 = hour1 & 0x0F;
	*hour = hour1 + hour10 * 10;

	date10 = (date1 & 0x30) >> 4;
	date1 = date1 & 0x0F;
	*date = date1 + date10 * 10;

	month10 = (month1 & 0x10) >> 4;
	month1 = month1 & 0x0F;
	*month = month1 + month10 * 10;

	year10 = (year1 & 0xF0) >> 4;
	year1 = year1 & 0x0F;
	*year = year1 + year10 * 10;

}

