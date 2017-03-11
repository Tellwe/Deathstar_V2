#include <xc.h>
#include "MRF89XA.h"
#include <string.h>
#include "functions.h"
#include "variables.h"
#include "bustel_communication_variables.h"
#include "m25p16.h"
#include "MCP79510.h"



//*************************************************************************************
/****************Function description**********************
Function sets the transiver to receive mode
*******************************************************/
void TransiverToReceive()
{
	timerFunction(2, 5); //Set a timeout timer on five seconds for problem with the transiver


	//Cycle for going into RX-mode
	SetRFMode(RF_STANDBY);								//Transiver into Standby
	__delay_ms(10);										//Wait for oscillator to wake up
	RegisterSet(FTPRIREG,(RegisterRead(FTPRIREG)|0x02));//Clear the bit for detection for the PLL Lock
	SetRFMode(RF_SYNTHESIZER);							//Transiver into syntesize
	while((RegisterRead(FTPRIREG) & 0b00000010) == 0)	//PLL Lock
	{
		if(bTimerComplete[2])
		{
			FindChannel();								//If no PLL-lock, find new values for the frequency
			break;
		}
	}
			
	SetRFMode(RF_RECEIVER);
	__delay_us(500);
	bTransiverModeReceive = 1;						
}
/****************Function description**********************
Function reads the fifo from the transiver, and returns the sent data package.
*******************************************************/
void TransiverReadFIFO()
{
	unsigned char ReceivedString[30];//Char string containing the data received from the transiver
	timerFunction(3, 5); //Set a timeout timer on five seconds for problem with the transiver
	
	SetRFMode(RF_STANDBY);
	bTransiverModeReceive = 0;
	int i = 0;
	for (int j = 0; j < 30; ++j)
	{
		ReceivedString[j] = 0;
	}
	while((trIRQ0)&&(!bTimerComplete[3]))										//Read the FIFO from the transiver until the FIFO is empty 
	{
		ReceivedString[i] = ReadFIFO();				//Place bytes in the string for received data
		i++;
	}

	__delay_ms(10);	
	SetRFMode(RF_SLEEP);								//Set the transiver into sleep-mode	

	ReceivedPacketHandler(ReceivedString);
}
/*********************************************************************
*Function for decrypting the package received and handling the data 
*
*
*
*
**********************************************************************/

void ReceivedPacketHandler(unsigned char Data[])
{
	if(Data[0] != STARTCHAR || Data[3] != ENDCHAR) //Check if the packet is complete and correct
		return;
	
	oOnBoardLED = 0;
	DelayDs(10);
	oOnBoardLED = 1;
	DelayDs(5);
	

	switch (Data[1])
	{
		case FLASH:
			switch (Data[2])
			{
				case LED1:
					oLEDLight = 1;
					DelayDs(10);
					oLEDLight = 0;
					DelayDs(5);
					break;
		
				case LED2:
					oLEDBlink = 1;
					DelayDs(10);
					oLEDBlink = 0;
					DelayDs(5);
					break;

				case LED3:
					oOnBoardLED = 0;
					DelayDs(10);
					oOnBoardLED = 1;
					DelayDs(5);
					break;

				default:
					break;
					
			}
			break;
		case BUSSIGNAL:
			if(Data[2] == NODE1 && OperationMode() == 6)
			{
				intBlinkCycle = 1;
				intBlinkCounter = 0;
				DelayDs(100);			//Delay between succesfull recived commands
			}
			if(Data[2] == NODE2 && OperationMode() == 7)
			{
				intBlinkCycle = 1;
				intBlinkCounter = 0;
				DelayDs(100);			//Delay between succesfull recived commands
			}
			break;

		case LIGHTSENSVAL:
			if(Data[2] != 0)
				break;
			TransmittPacket(LIGHTSENSVAL, AnalogValue(0));		
			break;

		case POTVAL:
			if(Data[2] != 0)
				break;
			TransmittPacket(POTVAL, AnalogValue(3));		
			break;

		case PROGVAL:
			if(Data[2] != 0)
				break;
			TransmittPacket(PROGVAL, eeprom_read(ADDRdarknessValue));
			break;

		case USEPOT:
			if(Data[2] != 0)
				break;
			if(bValueFromPot == TRUE)
				TransmittPacket(USEPOT, YES);
			else
				TransmittPacket(USEPOT, NO);
			break;

		case DARKCALC:
			if(Data[2] != 0)
				break;
			DarknessCheck();
			if(bDark)	
				TransmittPacket(DARKCALC, DARK);
			else
				TransmittPacket(DARKCALC, NOTDARK);
			break;

		case CLEARMEMORY:
			write_ram_status(read_ram_status() & 0b11100011);
			ram_bulk_erase();
			ResetMemoryAdress();
			TransmittPacket(CLEARMEMORY, DONE);
			break;

		case READMEMORY:
			SendMemoryData();

		default:
			break;
	}
}
/*********************************************************************
 * void TransmittPacket(char, char)
 *
 * Overview:        
 *              Sends the variable "dataString" through the transiver
 *
 * PreCondition:    
 *              Hardware initiated
 *
 * Input:       
 *              The string to be sent
 *				
 *
 * Output:      
 *
 * Side Effects:    
 *              
 *
 ********************************************************************/
void TransmittPacket(BYTE topic, BYTE value)
{
	timerFunction(4, 5); //Set a timeout timer on five seconds for problem with the transiver
	//Local variables
	int i = 0;

	//Initiation of transmitt sequence
		SetRFMode(RF_STANDBY);								//Transiver into Standby
		__delay_ms(10);										//Wait for oscillator to wake up
		RegisterSet(FTPRIREG,(RegisterRead(FTPRIREG)|0x02));//Clear the bit for detection for the PLL Lock
		SetRFMode(RF_SYNTHESIZER);							//Transiver into syntesize
		while((RegisterRead(FTPRIREG) & 0b00000010) == 0)	//Wait for the PLL to lock
		{
			if(bTimerComplete[4])
			{
				FindChannel();
				break;	
			}
			
		}
		SetRFMode(RF_TRANSMITTER);							//Set the transiver into tranmitt mode
		__delay_us(500);									//Transmitter wake-up time


		WriteFIFO('S');
		WriteFIFO('Y');
		WriteFIFO('N');
		WriteFIFO('C');

	//Transmitt data packet
		WriteFIFO(STARTCHAR);
		WriteFIFO(topic);
		WriteFIFO(value);
		WriteFIFO(ENDCHAR);

	//wait for transmitt done, set the transiver back to sleep
		while(!trIRQ1)
		{
			if(bTimerComplete[4])
			break;
		}
		__delay_us(10);
		SetRFMode(RF_SLEEP);
		__delay_ms(50);

}
//Separate functions for each of the operation modes, for the functionallity connected to the mode.

/****************Mode description**********************
Light with PIR and light sensor
Blink by button
*******************************************************/
void Mode_0()
{
	LightWithSensController();
}
/****************Mode description**********************
Light with button and lightsensor
Blink by button
*******************************************************/
void Mode_1()
{
	//Do a check of wheter it's dark or not, when the second counter has a certain value to get a sleepfunction
	if((intSecondCounter == 30)&&(!oLEDLight))
		DarknessCheck();
	if((intBlinkCycle == 1)&&(bDark == 1))
		oLEDLight = 1;
	else
		oLEDLight = 0;
}
/****************Mode description**********************
Light with PIR and lightsensor 
Blink not used, blink same function as the light so that two light outputs are available.
*******************************************************/
void Mode_2()
{
	LightWithSensController();
	if(oLEDLight)
		oLEDBlink = 1;
	else
		oLEDBlink = 0;
}
/****************Mode description**********************
Not used
*******************************************************/
void Mode_3()
{

}
/****************Mode description**********************
Transmitter node 1
Light with PIR and lightsensor
*******************************************************/
void Mode_4()
{
	if(!iButton)
	{
		TransmittPacket(BUSSIGNAL, NODE1);
	}
	LightWithSensController();
}
/****************Mode description**********************
Transmitter node 2
Light with PIR and lightsensor
*******************************************************/
void Mode_5()
{
	if(!iButton)
		{
			TransmittPacket(BUSSIGNAL, NODE2);
		}
		LightWithSensController();
}
/****************Function description**********************
Function for the light with both pir and light sensor. 
Handles the darkness check and motion supervision.
The function uses timers
*******************************************************/
void LightWithSensController(void)
{

			//Do a check of wheter it's dark or not, when the second counter has a certain value to get a sleepfunction
		if((intSecondCounter == 30)&&(!oLEDLight))
			DarknessCheck();

		//If it's dark, check the motion sensor and turn on the light
		//Set the time when the system should recheck the conditions for having the light on
		if((bDark == 1)&&(iPIR)&&(!oLEDLight))
		{
			//Set the light to ON
			oLEDLight = 1;
			//Initiate timer
			timerFunction(0,120);
		}

		//Check if the time limit is reached
		if(bTimerComplete[0])
		{
			DarknessCheck();
			if((bDark)&&(iPIR))
			{
				timerFunction(0,120);
			}
			else
				oLEDLight = 0;
			
		}

}
/*********************************************************************
 * unsigned char timerFunction(int intTimerNumber, bool bInitiate, intSetSeconds )
 *
 * Overview:        
 *              Handler of timer functions
 *
 * PreCondition:    
 *              Global timer variables initiated
 *
 * Input:       
 *              intTimerNumber	-	ID if the used timer
 *				intSeconds 		-	The seconds of the timer
 *
 * Output:      1 	-	Timer initiated and started correctly
 *				0 	-	Timer not initiated and started correctly
 *
 * Side Effects:    
 *              Toggel of the global status bits of the timer
 *
 ********************************************************************/
unsigned char timerFunction(int intTimerNumber, int intSetSeconds )
{
	intClockTimer[intTimerNumber] = intSetSeconds;			//Set the timer to the desired time
	bTimerComplete[intTimerNumber] = 0;
	return 1;
}
/*********************************************************************
 * void TimerCounter()
 *
 * Overview:        
 *              Handler of timer counting
 *
 * PreCondition:    
 *              Global timer variables initiated
 *
 * Input:       
 *
 * Output:      
 * Side Effects:    
 *              Counting of timers
 *
 ********************************************************************/
void TimerCounter()
{
	for (int i = 0; i < 5; i++)
	{
		if(intClockTimer[i]>0)
			intClockTimer[i]--;
		else
			bTimerComplete[i] = 1;
	}
}
//**************************************************************************************
// Initiation dof the PIC Microconoller
//**************************************************************************************
void PICInit()
{

	// set up oscillator control register
    OSCCONbits.IRCF2 = 1; //set OSCCON IRCF bits to select OSC frequency=4Mhz
    OSCCONbits.IRCF1 = 1; //set OSCCON IRCF bits to select OSC frequency=4Mhz
    OSCCONbits.IRCF0 = 0; //set OSCCON IRCF bits to select OSC frequency=4Mhz
    OSCCONbits.SCS = 1; //set the SCS bits to select internal oscillator block

	//Init port configuration
	PORTA = 0x00;
	PORTB = 0x00;
	PORTC = 0x00;
	PORTD = 0x00;
	PORTE = 0x00;

	TRISA = 0b00001011; //RA0 and RA1 inputs, RA2, RA4 and RA5 DI on transmitter
	TRISB = 0b00111101; //RB3/AN9 Voltage Battery. RB5, RB1 inputs, RB3 and RB4 connected to transiver 
	TRISC = 0b00010000; //RC4 connected to transiver
	TRISD = 0b00000111; //RD0, RD1, RD2 for the brackets
	TRISE = 0b00000111; //RE1, RE2, RE3 connected to amp and volt measurements
	
	ANSEL = 0b00000000; //Analog select
	ANSELH = 0b00000000; //Analog Select
	ANSELbits.ANS0 = 1;	//RA0 = analog input
	ANSELbits.ANS3 = 1;	//RA3 = analog input
	ANSELbits.ANS5 = 1;	//RE0 = analog input
	ANSELbits.ANS6 = 1;	//RE1 = analog input
	ANSELbits.ANS7 = 1;	//RE2 = analog input
	ANSELHbits.ANS8 = 1;	//RB2 = analog input

	OPTION_REGbits.nRBPU = 0; //For enabling of pull-ups
	WPUBbits.WPUB0 = 1; //Weak pull-up enabled RB0
	while(!RB0);		//Wait for the input to stabilize
	WPUBbits.WPUB5 = 1; //Weak pull-up enabled RB5
	while(!RB5);		//Wait for the input to stabilize

	//Configuration of the SPI communication
	SSPCONbits.SSPM=0x00;       // SPI Master mode, clock = Fosc/4 (1000 Khz)
    SSPCONbits.CKP=0;           // Idle state for clock is low
    SSPSTATbits.CKE=1;          // Transmit occurs on transition from active to idle clock state
    SSPSTATbits.SMP=0;          // Data is sampled at middle of data output time
    SSPCONbits.SSPEN=0x01;      // Enable SPI Port
	SSPIE = 0; //Dectivates the SPI-interrupt

	//Initial values to the SPI Nodes
	trCSDATA = 1; //initial value of CSDATA
	trCSCON = 1; //initial value of CSCON
	csClock = 1;	//Initial value of csClock
	csMem = 1;	//Initial value of csMem

	oOnBoardLED = 1;



	//Configuration of interrupt handler
	INTCONbits.RBIE = 1;	//Enable interrupt when change on PORTB
	INTCONbits.PEIE = 1;	//Enable peripheal interrupts
	PIE1bits.TMR1IE = 1;	//Enable overflow interrupt TMR1
	INTCONbits.GIE = 1;  	//Enable all unmasked interrupts
	IOCBbits.IOCB5 = 1;		//Enable interrupt on change for input RB5

	//Configuration of timers
	T1CON = 0b00110000;
	TMR1H = 0x0B;
	TMR1L = 0xDB;
	T1CONbits.TMR1ON = 1;

	//Initiation of ADC Conversion
	ADCON0bits.ADCS0 = 0;		//ADC Clock source
	ADCON0bits.ADCS1 = 0;		//ADC Clock source
	ADCON1bits.ADFM = 0;		//ADC result left justified
	ADCON1bits.VCFG1 = 0;		//ADC Ref voltage at VSS
	ADCON1bits.VCFG0 = 0;		//ADC Ref voltage at VDD

	//Initiation of variables
	intSecondCounter = 0;
	intHalfSecondCounter = 0;
	intMinuteCounter = 0;
}
//**************************************************************************************
// Delay function for setting delay in 0.1 sec steps
//**************************************************************************************
void DelayDs(int cnt){
	int i;
	for(i=0;i<cnt;i++){
		__delay_ms(100);
	}
}
//**************************************************************************************
// Send one byte on the SPI
//**************************************************************************************
void WriteSPI(unsigned char databyte)
{
    unsigned char buffer;
    buffer = SSPBUF;            // Read the buffer to clear any remaining data and clear the buffer full bit
	PIR1bits.SSPIF=0;           // clear SSP interrupt bit
    SSPBUF = databyte;          // Write data byte to the buffer to initiate transmission  	
 	while(PIR1bits.SSPIF == 0);
 	PIR1bits.SSPIF = 0;
}
//**************************************************************************************
// Read one byte on the SPI
//**************************************************************************************
unsigned char ReadSPI(void)
{
 	WriteSPI(0x00);
    return SSPBUF; 
}
//**************************************************************************************
// Initiate the transiver
//**************************************************************************************
void TransiverInit(void)
{  
	
	RegisterSet(GCONREG, 0x30);					//Set to the right frequency 
	RegisterSet(DMODREG,(0xA8)); 				//Set transiver into buffer mode
	RegisterSet(FDEVREG, 0x09);	//0x09				//Set the frequency deeviation to 40KHz
	RegisterSet(BRSREG, 0x07);	//0x09				//Set the bitrate to 25kbps	
	RegisterSet(FIFOCREG,(0x0A));				//Set the FIFO-size to 16 bytes and the treshhold for interrupts to 10
	RegisterSet(FTXRXIREG,(0x00|0xB0|0x09));	//IRQ0RX = nFIFOEmpty, IRQ1RX = FIFO_THRESHOLD, IRQ0TX = nFIFOEMPTY, IRQ1TX = TXDONE
	RegisterSet(FTPRIREG,(0x01|0x10|0x01));	//	//Start to till FIFO when sync word is detected, start transmission when FIFO_not_full
	RegisterSet(FILCREG, 0xA3);					//FIltersetting for the transiver
	RegisterSet(SYNCREG,(0x18|0x20|0x06));		//SYNC-word enabled, 32 bit size, 3 errors allowed
	RegisterSet(SYNCV31REG,('S'));				//
	RegisterSet(SYNCV23REG,('Y'));				//
	RegisterSet(SYNCV15REG,('N'));				//
	RegisterSet(SYNCV07REG,('C'));				//
	RegisterSet(TXCONREG, 0x72);			 		//Transmitt parameters
	RegisterSet(CLKOUTREG, 0x3C);					//Dissable CLKOUT
	RegisterSet(FCRCREG, 0x80);
	RegisterSet(GCONREG, (RegisterRead(GCONREG)|0x01));

	if(FindChannel() == 1)
		Blink(1);
	SetRFMode(RF_SLEEP);

}
/*********************************************************************
 * WORD FindChannel()
 *
 * Overview:        
 *              The function uses the arrays of valuse calculated for the RPS-valuse in the transiver
 *				and tests these to get the PLL-lock bit OK
 *
 * PreCondition:    
 *              Arrays OK
 *
 * Input:       
 *          None
 *
 * Output:  1 - if OK
 *			0 if NOK    
 *
 * Side Effects: Communication with the transiver   
 *
 ********************************************************************/
char FindChannel(void)
{
	int cnt = 0;
	bChannelFound = 0;
	channelFound = 0;
	
	__delay_ms(1000);
	for (int i = 0; i < 13; i++)
	{	
		RegisterSet(R1CREG,ChannelR[i]);					//R-value 
		RegisterSet(P1CREG,ChannelP[i]);					//P-value 
		RegisterSet(S1CREG,ChannelS[i]);					//S-value 
		RegisterSet(R2CREG,ChannelR[i]);					//R-value 
		RegisterSet(P2CREG,ChannelP[i]);					//P-value 
		RegisterSet(S2CREG,ChannelS[i]);					//S-value 
		RegisterSet(FTPRIREG,(RegisterRead(FTPRIREG)|0x02));//Clear PLL_LOCK-flag so we can see it restore on the new frequency
		SetRFMode(RF_SYNTHESIZER);							//Set the transiver to syntesize
		cnt = 0;
		while(cnt < 5)
		{
			
			if((RegisterRead(FTPRIREG) & 0b00000010) != 0)
			{
				bChannelFound = 1;
				channelFound = i;
				return 1;
			}
			cnt++;
			__delay_us(1000);			
		}
		
		
	}
return 0;	

}
/*********************************************************************
 * void SetRFMode(mode)
 *
 * Overview:        
 *              Sets the transiver to the desired mode
 *
 * PreCondition:    
 *              Hardware initiated
 *
 * Input:       
 *              the desired mode of the transiver
 *				
 *
 * Output:      
 *
 * Side Effects:    
 *              
 *
 ********************************************************************/
void SetRFMode(BYTE mode)
{
	trCSCON = 0;
	BYTE	mcparam0_read;
	mcparam0_read = RegisterRead(REG_MCPARAM0);
	switch (mode) {
		case RF_TRANSMITTER:
			RegisterSet(REG_MCPARAM0, (mcparam0_read & 0x1F) | RF_TRANSMITTER);
			//RF_Mode = RF_TRANSMITTER;				//RF in TX mode
			break;
		case RF_RECEIVER:
			RegisterSet(REG_MCPARAM0, (mcparam0_read & 0x1F) | RF_RECEIVER);
			//RF_Mode = RF_RECEIVER;					//RF in RX mode
			break;
		case RF_SYNTHESIZER:
			RegisterSet(REG_MCPARAM0, (mcparam0_read & 0x1F) | RF_SYNTHESIZER);
			//RF_Mode = RF_SYNTHESIZER;				//RF in Synthesizer mode
			break;
		case RF_STANDBY:
			RegisterSet(REG_MCPARAM0, (mcparam0_read & 0x1F) | RF_STANDBY);
			//RF_Mode = RF_STANDBY;					//RF in standby mode
			break;
		case RF_SLEEP:
			RegisterSet(REG_MCPARAM0, (mcparam0_read & 0x1F) | RF_SLEEP);
			//RF_Mode = RF_SLEEP;						//RF in sleep mode
			break;
	} /* end switch (mode) */
	trCSCON = 1;
}
/*********************************************************************
 * void Blink(n)
 *
 * Overview:        
 *              flashes the blink output "n" times
 *
 * PreCondition:    
 *              Hardware initiated
 *
 * Input:       
 *              number of flashes
 *				
 *
 * Output:      
 *
 * Side Effects:    
 *              
 *
 ********************************************************************/
void Blink(int n)
{
	for(int i = 1;i<=n;i++)
	{
		oLEDBlink = 1;
		DelayDs(2);
		oLEDBlink = 0;
		DelayDs(6);	
	}
	
}
/*********************************************************************
 * WORD RegisterRead()
 *
 * Overview:        
 *              This function access the control register of MRF89XA.
 *              The register address and the register settings are
 *              the input
 *
 * PreCondition:    
 *              None
 *
 * Input:       
 *          WORD    setting     The address of the register and its
 *                              corresponding settings
 *
 * Output:  None    
 *
 * Side Effects:    Register settings have been modified
 *
 ********************************************************************/
unsigned char RegisterRead(unsigned char adress)
{
	BYTE value;
    trCSCON = 0;
    adress = ((adress<<1)|0x40);
    WriteSPI(adress); 
	value = ReadSPI();
    trCSCON = 1;
	return value;
}
/*********************************************************************
 * void RegisterSet(BYTE address, BYTE value)
 *
 * Overview:        
 *              This function access the control register of MRF89XA.
 *              The register address and the register settings are
 *              the input
 *
 * PreCondition:    
 *            	 None
 *
 * Input:       
 *          WORD    setting     The address of the register and its
 *                              corresponding settings
 *
 * Output:  None    
 *
 * Side Effects:    Register settings have been modified
 *
 ********************************************************************/
void RegisterSet(unsigned char adress, unsigned char value)
{
    trCSCON = 0;
    adress = (adress<<1);
    WriteSPI(adress);
    WriteSPI(value);
    trCSCON = 1;
}
/*********************************************************************
 * BYTE ReadFIFO(void)
 *
 * Overview:        
 *              The function reads one byte from the FIFO
 *
 * PreCondition:    
 *              MRF89XA transceiver has been properly initialized
 *
 * Input:       
 *              None
 *
 * Output:      Data from FIFO
 *
 * Side Effects:    
 *              The packet has been sent out
 *
 ********************************************************************/
unsigned char ReadFIFO(void)
{
	BYTE value;
    trCSDATA = 0;
	value = ReadSPI();
    trCSDATA = 1;
	return value;
	
}
/*********************************************************************
 * void WriteFIFO(BYTE Data)
 *
 * Overview:        
 *              This function fills the FIFO
 *
 * PreCondition:    
 *              MRF89XA transceiver has been properly initialized
 *
 * Input:       
 *              BYTE   Data - Data to be sent to FIFO.
 *
 * Output:      None
 *
 * Side Effects:    
 *              The packet has been sent out
 *
 ********************************************************************/
void WriteFIFO(unsigned char Data)
{
    trCSDATA = 0;
    WriteSPI(Data);
    trCSDATA = 1;
}
/*********************************************************************
 * void interrupt tc_int(void)
 *
 * Overview:        
 *              This function controlls the interrupt routine generated in the controller
 *				The interupt occurs every half-second or on the button being pushed
 * PreCondition:    
 *              
 *
 * Input:       
 *             
 * Output:      None
 *
 * Side Effects:    
 *              
 *
 ********************************************************************/

void interrupt tc_int(void){

	//Make sure that the application is without wireless
	if((iButton == 0)&&(intBlinkCycle == 0)&&(OperationMode() < 2)){
		intBlinkCycle = 1;
		intBlinkCounter = 0;

	}

	//
	if(TMR1IF ==1){
		TMR1ON = 0;
		TMR1H = 0x0B;
		TMR1L = 0xDB;
		//Control of the blinkcycle 
		if((intBlinkCycle == 1)&&(intBlinkCounter<intNumberOfBlinks*2)){
			intBlinkCounter++;
			
			if(oOnBoardLED == 0)
				oOnBoardLED = 1;
			else if(oOnBoardLED == 1)
				oOnBoardLED = 0;
		}
		
		else if((intBlinkCycle == 1)&&(intBlinkCounter >=intNumberOfBlinks*2)){
			intBlinkCycle = 0;
			oLEDBlink = 0;
		}

		//Control of the counter counting seconds for the application
		intHalfSecondCounter++;
		if(intHalfSecondCounter>=2)
		{
			intHalfSecondCounter = 0;
			intSecondCounter++;
			TimerCounter();

		}
		if(intSecondCounter >=60)
		{
			intSecondCounter = 0;
			intMinuteCounter++;
			saveDataToFlash();

		}
		if(intMinuteCounter >= 60)
		{
			intMinuteCounter = 0;
		}

		//Start Timer again
		TMR1ON = 1;	
}
	TMR1IF = 0;
	RBIF = 0;
    GIE = 1;						//Re-enable all interrupts
return;
}
/*********************************************************************
 * unsigned char AnalogValue(int channel)
 *
 * Overview:        
 *              Reads and returns the analog value from the requested channel
 *
 * PreCondition:    
 *              The channel need to be configured as an analog input
 *
 * Input:       Requested input channel
 *             
 * Output:      Analog value of the input
 *
 * Side Effects:    
 *             
 ********************************************************************/

unsigned char AnalogValue(unsigned char channel)
{
	ADCON0bits.CHS = channel;
	ADCON0bits.ADON = 1;
	DelayDs(1);
	ADCON0bits.GO = 1;
	DelayDs(1);
	while(ADCON0bits.GO);
	ADCON0bits.ADON = 0;
	return ADRESH;

}
/*********************************************************************
 * unsigned char DarknessCheck(void)
 *
 * Overview:        
 *              Reads and returns the status whether it's dark or not
 *
 * PreCondition:    
 *              
 * Input:       
 *             
 * Output:      1 if dark, 0 if not
 *
 * Side Effects  :    
 *             
 ********************************************************************/
void DarknessCheck(void)
{
	unsigned char darknessValue;
	oAnalogInputsOFF = 0;
	// determine whether we use valuie from pic or potentiometer.
	if(bValueFromPot == TRUE)
		darknessValue= AnalogValue(anChPot);
	else
		darknessValue=eeprom_read(ADDRdarknessValue);

	if(AnalogValue(0)>darknessValue)
		bDark = 1;
	else
		bDark = 0;
	oAnalogInputsOFF = 1; 
		
}
/*********************************************************************
 * unsigned char OperationMode(void);
 *
 * Overview:        
 *              Reads and returns the operation mode from reading the bracketsetting 
 *				on the board
 *
 * PreCondition:    
 *              
 * Input:       
 *             
 * Output:      0 	-	Blink by button, Light by pir and lightsens, solarpanel
 *				1 	-	Blink by button, Light by button, no pir or lightsens, solarpanel
 *
 * Side Effects:    
 *             
 ********************************************************************/
unsigned char OperationMode(void)
{
	unsigned char bracketStatus;
	bracketStatus = ((~PORTB) & 0b00000111) ;		//Read the status of PORTB, remove the unread bits, invert and remove bit 3-7 again.

	return bracketStatus;
}
/*********************************************************************
 * 
 *
 * Overview:        
 *              
 *				
 *
 * PreCondition:    
 *              
 * Input:       
 *             
 * Output:      
 *				
 *
 * Side Effects:    
 *             
 ********************************************************************/
void ReadMemoryAdress(unsigned char* var3,unsigned char* var2,unsigned char* var1)
{
	*var3 = eeprom_read(ADDRflashVal3);
	*var2 = eeprom_read(ADDRflashVal2);
	*var1 = eeprom_read(ADDRflashVal1);


}
/*********************************************************************
 * 
 *
 * Overview:        
 *              
 *				
 *
 * PreCondition:    
 *              
 * Input:       
 *             
 * Output:      
 *				
 *
 * Side Effects:    
 *             
 ********************************************************************/
void IncreaseMemoryAdress(void)
{
	unsigned char var1, var2, var3;
	var1 = eeprom_read(ADDRflashVal1);
	var1++;
	eeprom_write(ADDRflashVal1,var1);
	if(var1 == 0)
	{
		var2 = eeprom_read(ADDRflashVal2);
		var2++;
		eeprom_write(ADDRflashVal2,var2);
		if(var2 == 0)
		{
			var3 = eeprom_read(ADDRflashVal3);
			var3++;
			if(var3 > 0x1F)
				var3 = 0x1F;

			eeprom_write(ADDRflashVal3, var3);
		}
	}
}
/*********************************************************************
 * 
 *
 * Overview:        
 *              
 *				
 *
 * PreCondition:    
 *              
 * Input:       
 *             
 * Output:      
 *				
 *
 * Side Effects:    
 *             
 ********************************************************************/
void ResetMemoryAdress(void)
{
	eeprom_write(ADDRflashVal1,0);
	eeprom_write(ADDRflashVal2,0);
	eeprom_write(ADDRflashVal3,0);
}
/*********************************************************************
 * 
 *
 * Overview:        
 *              
 *				
 *
 * PreCondition:    
 *              
 * Input:       
 *             
 * Output:      
 *				
 *
 * Side Effects:    
 *             
 ********************************************************************/
void saveDataToFlash()
{
	unsigned char value, addr1, addr2, addr3, seconds, minutes, hours, date, month, year;

	ReadClock(&seconds, &minutes, &hours, &date, &month, &year);

	value = year;
	ReadMemoryAdress(&addr3, &addr2, &addr1);
	read_write_flash_ram(
		0,
		1,
		addr3,
		addr2,
		addr1,
		&value);
	IncreaseMemoryAdress();	

	value = month;
	ReadMemoryAdress(&addr3, &addr2, &addr1);
	read_write_flash_ram(
		0,
		1,
		addr3,
		addr2,
		addr1,
		&value);
	IncreaseMemoryAdress();	

	value = date;
	ReadMemoryAdress(&addr3, &addr2, &addr1);
	read_write_flash_ram(
		0,
		1,
		addr3,
		addr2,
		addr1,
		&value);
	IncreaseMemoryAdress();	

	value = hours;
	ReadMemoryAdress(&addr3, &addr2, &addr1);
	read_write_flash_ram(
		0,
		1,
		addr3,
		addr2,
		addr1,
		&value);
	IncreaseMemoryAdress();	


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

}
/*********************************************************************
 * 
 *
 * Overview:        
 *              
 *				
 *
 * PreCondition:    
 *              
 * Input:       
 *             
 * Output:      
 *				
 *
 * Side Effects:    
 *             
 ********************************************************************/
void SendMemoryData()
{
	unsigned long address, finalAddress = 0;
	unsigned char addr3, addr2, addr1, value;

	ReadMemoryAdress(&addr3, &addr2, &addr1);

	finalAddress = (finalAddress << 8) | addr3;
	finalAddress = (finalAddress << 8) | addr2;
	finalAddress = (finalAddress << 8) | addr1;

	for(address = 0; address < finalAddress; address++)
	{

		addr1 = address & 0x00FF;
		addr2 = address >> 8 & 0x00FF;
		addr3 = address >> 16 & 0x00FF;


		read_write_flash_ram(
			1,
			1,
			addr3,
			addr2,
			addr1,
			&value);
		
		TransmittPacket(MEMVAL, value);
	}
	TransmittPacket(READMEMORY, DONE);


}
