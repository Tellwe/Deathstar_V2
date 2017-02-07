/*
File containing all the variables used for communication between bustel-units but also with the test equipment


*/

#ifndef _bustel_communication_variables_
#define _bustel_communication_variables_
//Start and end chars
#define STARTCHAR		44
#define ENDCHAR			55

//Topics
#define FLASH			1
#define	BUSSIGNAL		2
#define LIGHTSENSVAL	3
#define POTVAL			4
#define PROGVAL			5
#define USEPOT			6
#define DARKCALC		7
#define READMEMORY		8
#define CLEARMEMORY		9


//Values
#define LED1			1
#define LED2			2
#define LED3			3
#define NODE1			1
#define NODE2			2
#define NODE3			3
#define NODE4			4
#define REQ				0
#define NO 				0
#define YES				1
#define DARK 			1
#define NOTDARK			2
#define DONE			1
#define NOTDONE			0


#endif