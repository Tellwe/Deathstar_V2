

//IO of the card v10.0
//Inputs
#define iButton 		RB5
#define iPIR 			RA1
#define iPotentiometer	RA3 	//AN3
#define iLightSensor 	RA0 	//AN0
#define iBracket1 		RB0
#define iBracket2 		RB1
#define iBracket3 		RB2
//Outputs
#define oAnalogInputsON	RC0
#define oLEDBlink 		RC1
#define oLEDLight 		RC2
//Connections to the transiver
#define trRESET 		RA2 	//DO
#define trCSCON 		RA4 	//DO
#define trIRQ0 			RB3 	//DI
#define trIRQ1 			RB4 	//DI
#define trSDI 			RC5 	//DO
#define trSDO 			RC4 	//DI
#define trSCK 			RC3 	//DO
#define trCSDATA 		RA5 	//DO

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