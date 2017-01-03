/*********************************************************************
 *
 *           Frequency Hopping Spread Spectrum API .H file
 *
 *********************************************************************
 * FileName:        FHSS.h
 * Dependencies:    None
 * Processor:       PIC18/PIC24
 * Complier:        MCC18/C30 compilers
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its PICmicro® Microcontroller is intended and
 * supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 ********************************************************************/
#ifndef _FHSS_H
	#define _FHSS_H
#include "Compiler.h"
#include "GenericTypeDefs.h"

typedef struct {
	BYTE R;
	BYTE P;
	BYTE S;
	BYTE MCParam_Band;
}freq_t;

void FHSS_Init(void);		//Initialize Frequency hopping - Timers
void FHSS_Exit(void);		//Exit frequency hopping mode
void FHSS_Timer_Init(WORD tvalue);	//Initialize the FHSS timer
void FHSS_Timer_Stop(void); //Stop FHSS timer
void FHSS_Sync_Start_Master(void);	//Start synchronization phase
void FHSS_Sync_Start_Slave(void);	
void FHSS_Sync_Timer_Init(WORD); //Start Sync Timer
void FHSS_Sync_Timer_Stop(void); //End Sync Timer
BOOL FHSS_Sync_Timer_Expired(void);	//gives the status whether the sync timer expired or not
void FHSS_FreqHop(void);		//Transmits a dialog frame and gets response back

#define SYNC_COMMAND 0x01
#define	DIALOG_COMMAND 0x02
#define RESPONSE_COMMAND 0x03

//End of the file
#endif
