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

int main(void)
{ 
	//Initiation of hardware
	PICInit();
	TransiverInit();
	ClockInit();

	//Initiation Complete
	oOnBoardLED = 0;
	__delay_ms(300);
	oOnBoardLED = 1;
	__delay_ms(300);

	while(1)
	{
		if(!iSW1)
		{

			oAnalogInputsOFF = 0;
			TransmittPacket(1,AnalogValue(anChPot));
			__delay_ms(50);
			TransmittPacket(2,AnalogValue(anChVoltBat));
			__delay_ms(50);
			TransmittPacket(3,AnalogValue(anChAmpBat));
			__delay_ms(50);
			TransmittPacket(4,AnalogValue(anChVoltSolar));
			__delay_ms(50);
			TransmittPacket(5,AnalogValue(anChAmpSolar));
			__delay_ms(2400);
			oAnalogInputsOFF = 1;

	



/*


//Example of how logging of data could look

			
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