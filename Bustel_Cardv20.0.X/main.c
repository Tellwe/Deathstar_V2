#include <htc.h>
#include "MRF89XA.h"
#include <string.h>
#include "functions.h"
#include "variables.h"

/**********************		Description		*************************************** 					











***********************************************************************************/
//Configline for initial configuration of registers in the PIC

__CONFIG(DEBUG_OFF & LVP_OFF & FCMEN_OFF & IESO_OFF & BOREN_OFF & CPD_ON & CP_ON & MCLRE_OFF & PWRTE_OFF & WDTE_OFF & FOSC_INTRC_NOCLKOUT);


//*************************************************************************************

void main()
{ 
	//Initiation of hardware...
	init();

	__delay_ms(800);
	TransiverInit();
		
	while(1)
	{
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