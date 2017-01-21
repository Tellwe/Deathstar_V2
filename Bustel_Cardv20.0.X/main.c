#include <xc.h>
#include "MRF89XA.h"
#include <string.h>
#include "functions.h"
#include "variables.h"
#include "bustel_communication_variables.h"
#include "GenericTypeDefs.h"
#include "m25p16.h"

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
	init();

	TransiverInit();
	write_ram_status(read_ram_status() & 0b11100011);
	ram_bulk_erase();

//Initiation Complete
			
	oOnBoardLED = 0;
	__delay_ms(300);
	oOnBoardLED = 1;
	__delay_ms(300);




	while(1)
	{
		if(!iSW1)
		{

			unsigned char value; 
			for(int i = 0; i < 100; i++)
			{
				value = i;
				read_write_flash_ram(0,1,0,0,i,&value);

			}

			//TransmittPacket(100, read_ram_status());
			read_write_flash_ram(1,20,0,0,0,&var2);
			
			for(int i = 0; i < 1; i++)
			{
				__delay_ms(100);
				//TransmittPacket(i,var2[i]);
			}
			
			while(!iSW1);
		
			
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