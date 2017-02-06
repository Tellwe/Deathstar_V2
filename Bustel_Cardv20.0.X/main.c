#include <xc.h>
#include "MRF89XA.h"
#include <string.h>
#include "functions.h"
#include "variables.h"
#include "bustel_communication_variables.h"
#include "GenericTypeDefs.h"
#include "m25p16.h"
#include "MCP79510.h"

/**********************		Description		*************************************** 					











***********************************************************************************/
//Configline for initial configuration of registers in the PIC

//__CONFIG(DEBUG_OFF & LVP_OFF & FCMEN_OFF & IESO_OFF & BOREN_OFF & CPD_ON & CP_ON & MCLRE_OFF & PWRTE_OFF & WDTE_OFF & FOSC_INTRC_NOCLKOUT);
#pragma config DEBUG=OFF, LVP=OFF, FCMEN=OFF, IESO=OFF, BOREN=OFF, CPD=ON, CP=ON, MCLRE=OFF, PWRTE=OFF, WDTE=OFF, FOSC=INTRC_NOCLKOUT 

//*************************************************************************************
unsigned char var1[95];
unsigned char var2[95];

int main(void)
{ 
	//Initiation of hardware...
	PICInit();

	TransiverInit();

	ClockInit();

/*	Clear the entire memory.
	
	write_ram_status(read_ram_status() & 0b11100011);
	ram_bulk_erase();
*/
	oOnBoardLED = 0;
	__delay_ms(300);
	oOnBoardLED = 1;
	__delay_ms(300);


	eeprom_write(ADDRflashVal3,0);
	eeprom_write(ADDRflashVal2,0);
	eeprom_write(ADDRflashVal1,0);


//Initiation Complete
	
	oOnBoardLED = 0;
	__delay_ms(300);
	oOnBoardLED = 1;
	__delay_ms(300);



//Read adress from EEPROM and assign to internal variabel



	while(1)
	{
		if(!iSW1)
		{

			unsigned char seconds, minutes, hour, date, month, year;

			ReadClock(&seconds, &minutes, &hour, &date, &month, &year);

			TransmittPacket(1,seconds);
			__delay_ms(50);
			TransmittPacket(2,minutes);
			__delay_ms(50);
			TransmittPacket(3,hour);
			__delay_ms(50);
			TransmittPacket(4,date);
			__delay_ms(50);
			TransmittPacket(5,month);
			__delay_ms(50);
			TransmittPacket(6,year);

			__delay_ms(2400);


	



/*


//Example of how logging of data could look

			unsigned char value, addr1, addr2, addr3; 

			value = AnalogValue(anChLightSensor);
			ReadMemoryAdress(&addr3, &addr2, &addr1);
			read_write_flash_ram(
				0,
				1,
				addr3,
				addr2,
				addr1,
				&value);
			IncreaseMemoryAdress();	
			TransmittPacket(1,value);		

			value = AnalogValue(anChPot);
			ReadMemoryAdress(&addr3, &addr2, &addr1);
			read_write_flash_ram(
				0,
				1,
				addr3,
				addr2,
				addr1,
				&value);
			IncreaseMemoryAdress();			
			TransmittPacket(2,value);		


			value = AnalogValue(anChAmpSolar);
			ReadMemoryAdress(&addr3, &addr2, &addr1);
			read_write_flash_ram(
				0,
				1,
				addr3,
				addr2,
				addr1,
				&value);
			IncreaseMemoryAdress();			
			TransmittPacket(3,value);		


			value = AnalogValue(anChVoltSolar);
			ReadMemoryAdress(&addr3, &addr2, &addr1);
			read_write_flash_ram(
				0,
				1,
				addr3,
				addr2,
				addr1,
				&value);
			IncreaseMemoryAdress();			
			TransmittPacket(4,value);		


			value = AnalogValue(anChAmpBat);
			ReadMemoryAdress(&addr3, &addr2, &addr1);
			read_write_flash_ram(
				0,
				1,
				addr3,
				addr2,
				addr1,
				&value);
			IncreaseMemoryAdress();			
			TransmittPacket(5,value);	


			value = AnalogValue(anChVoltBat);
			ReadMemoryAdress(&addr3, &addr2, &addr1);
			read_write_flash_ram(
				0,
				1,
				addr3,
				addr2,
				addr1,
				&value);
			IncreaseMemoryAdress();			
			TransmittPacket(6,value);	

			*/	


			
			//while(!iSW1);
		
			
		}

		if(OperationMode() == 0)
		{
			Mode_0();			
		}
		if(OperationMode() == 1)
		{
			Mode_1();
		} 
		//Mode without blink, oLedBlink set to same sequence as oLEDLight
		if(OperationMode() == 2)
		{
			Mode_2();
		}
		//Transmittsequence
		if(OperationMode() == 4)
		{
			Mode_4();
		}
		//Transmittsequence
		if(OperationMode() == 5)
		{
			Mode_5();
		}

		//The functionallity for receiveing data over the wireless communication
		
		if(!bTransiverModeReceive)
			TransiverToReceive();
		if(trIRQ1 && bTransiverModeReceive)										
			TransiverReadFIFO();			

	}

}