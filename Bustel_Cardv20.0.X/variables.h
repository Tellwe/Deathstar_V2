
#ifndef _variables_
#define _variables_
//IO of the card v11.0
//Inputs
#define iButton 		RB5
#define iPIR 			RA1
#define iPotentiometer	RA3 	//AN3
#define iLightSensor 	RA0 	//AN0
#define iBracket1 		RD0
#define iBracket2 		RD1
#define iBracket3 		RD2
#define iSW1			RB1	//RB0 according to electrical scheme		
//Outputs
#define oAnalogInputsON	RC0	//DOES NOT EXIST
#define oLEDBlink 		RC1
#define oLEDLight 		RC2
#define oOnBoardLED		RB2 //RB1 according to electrical scheme

//SPI connections
#define trSDI 			RC5 	//DO
#define trSDO 			RC4 	//DI
#define trSCK 			RC3 	//DO

//Connections to the transiver (MRF89XAM8A)
#define trRESET 		RA2 	//DO
#define trCSCON 		RA4 	//DO
#define trIRQ0 			RB3 	//DI
#define trIRQ1 			RB4 	//DI
#define trCSDATA 		RA5 	//DO

//Connections to Real time clock (MCP79510-I/MS)
#define clCS			RD4		//Chip select clock

//Connections to flash memory (M25P16-VMN6P)
#define memCE			RD3		//chip select memory

//Analog inputs for measurements
#define VOLT_BATT		RB3		//AN8	Voltage Battery //RB2 according to electrical scheme
#define AMP_BATT		RE1		//AN6	Ampere Battery
#define VOLT_SOL		RE2		//AN7	Volt Solarpanel
#define AMP_SOL			RE0		//AN5	Ampere Solarpanel


//Definition needed to be able to use the delay-function from the htc.h
#define _XTAL_FREQ 4000000 


//Global variables
unsigned char ReceivedData[40];
int intBlinkCycle;
int intBlinkCounter;
int intHalfSecondCounter;			//Counts half seconds in interruptroutine
int intSecondCounter;				//Counts seconds in interruptroutine
int intMinuteCounter;				//Counts minutes in interruptroutine
BOOL bDark;							//Bit is set when it's dark
int intDarkCheckIntervall;			//The set time beween controls of the light sensor in minutes
int intClockTimer[5];				//Timers made by Seb
BOOL bTimerComplete[5];				//Flag for timer complete by Seb
int intNumberOfBlinks = 600;		//Number of blinks for a blinkcycle
BOOL bTransiverModeReceive = FALSE;	//Active state of the transiver for receive
	BOOL bValueFromPot = TRUE;			//TODO
int addressDarknessValue = 0;

//Used in function for channel find
//Values in arrays calculated from matlab for 868,32MHZ
const unsigned char ChannelR[13] = {79,89,99,109,119,149,159,159,169,169,69,129,139};
const unsigned char ChannelP[13] = {63,71,79,87,95,119,126,127,134,135,55,103,111};
const unsigned char ChannelS[13] = {24,27,30,33,36,45,123,48,126,51,21,39,42};

BOOL bChannelFound = 0;				//Used in the function FindChannel, if the PLL-lock is ok the bit will be set
int channelFound;					//Used in the function FindChannel, if the channel is OK the variable indicates the array position.

#endif
