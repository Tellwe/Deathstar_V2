/*********************************************************************
 *
 *                  Console specific Defines
 *
 *********************************************************************
 * FileName:        Console.h
 * Dependencies:    None
 * Processor:       PIC18/PIC24
 * Complier:        MCC18 v1.00.50 or higher, C30 3.10 or higher
 *                  HITECH PICC-18 V8.10PL1 or higher
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
#ifndef _CONSOLE_H
#define _CONSOLE_H

#include "GenericTypeDefs.h"
#include "compiler.h"
#include "RadioDriverDefs.h"

#if defined(__18F87J11)

		#define TXSTA	TXSTA1
		#define RCSTA	RCSTA1
		#define BAUDCON	BAUDCON1
		#define BRGL	SPBRG1
		#define BRGH	SPBRGH1
		#define TRMT	TXSTA1bits.TRMT
		#define TXIF	PIR1bits.TX1IF
		#define BRG16	BAUDCON1bits.BRG16
		#define TXREG	TXREG1
		#define RXREG	RCREG1
		#define OERR	RCSTA1bits.OERR
		#define CREN	RCSTA1bits.CREN
		#ifndef	GIE		
			#define GIE	INTCONbits.GIE
		#endif
		#ifndef PEIE
			#define PEIE	INTCONbits.PEIE
		#endif
		#define RCIF	PIR1bits.RC1IF
		
 //declare console functions		
		void ConsoleInit(void);
		#define ConsoleIsPutReady()     (TRMT)
        void ConsolePut(BYTE c);
        void ConsolePutString(BYTE *s);
        void ConsolePutROMString(ROM char* str);
    
        #define ConsoleIsGetReady()     (RCIF)
        BYTE ConsoleGet(void);
        void PrintChar(BYTE);
        void PrintDigit(BYTE toPrint);
		void PrintDigitW(WORD toPrint);
        BYTE ConsoleInput(void);
#else
	//This case is for PIC24 and processors working with Explorer 16 board
        void ConsoleInit(void);
        #define ConsoleIsPutReady()     (U2STAbits.TRMT)
        void ConsolePut(BYTE c);
        void ConsolePutROMString(ROM char* str);
    
        #define ConsoleIsGetReady()     (IFS1bits.U2RXIF)
        BYTE ConsoleGet(void);
        BYTE ConsoleInput(void);
        void PrintChar(BYTE);
        void PrintDigit(BYTE toPrint);
		void PrintDigitW(WORD toPrint);
#endif
	#define nl() ConsolePutROMString((ROM char*)"\r\n");
	#define c(a) ConsolePutROMString((ROM char*)a);

#endif
