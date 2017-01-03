/*********************************************************************
 *
 *                  Processor/Compiler specific defs.
 *
 *********************************************************************
 * FileName:        Compiler.h
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
#ifndef COMPILER_H
#define COMPILER_H
#include "GenericTypeDefs.h"

#if defined (__18F87J11)
						//microchip c18 compiler
	#include <p18cxxx.h>
						//include the defines file
						//All pic18 processors come under this category
#elif defined (__PIC24F__)		
						//microchip c30 compiler
		#include <p24Fxxxx.h>
						//include the defines file
						//All pic24 processors come under this category
#endif
#include <stdlib.h>

//#if defined(WIN32)
//	#define ROM
//#endif

#if defined(__18F87J11)
	
	#define ROM rom
#else
	#define ROM const
#endif

						//include c library
	#define TRISA0              TRISAbits.TRISA0
    #define TRISA1              TRISAbits.TRISA1
    #define TRISA2              TRISAbits.TRISA2
    #define TRISA3              TRISAbits.TRISA3
    #define TRISA4              TRISAbits.TRISA4
    #define TRISA5              TRISAbits.TRISA5
    #define TRISA6              TRISAbits.TRISA6
    #define TRISA7              TRISAbits.TRISA7

    #define LATA0               LATAbits.LATA0
    #define LATA1               LATAbits.LATA1
    #define LATA2               LATAbits.LATA2
    #define LATA3               LATAbits.LATA3
    #define LATA4               LATAbits.LATA4
    #define LATA5               LATAbits.LATA5
    #define LATA6               LATAbits.LATA6
    #define LATA7               LATAbits.LATA7

	#define RA0                 PORTAbits.RA0
    #define RA1                 PORTAbits.RA1
    #define RA2                 PORTAbits.RA2
    #define RA3                 PORTAbits.RA3
    #define RA4                 PORTAbits.RA4
    #define RA5                 PORTAbits.RA5
    #define RA6                 PORTAbits.RA6
    #define RA7                 PORTAbits.RA7

//PORTB defines
	#define TRISB0              TRISBbits.TRISB0
    #define TRISB1              TRISBbits.TRISB1
    #define TRISB2              TRISBbits.TRISB2
    #define TRISB3              TRISBbits.TRISB3
    #define TRISB4              TRISBbits.TRISB4
    #define TRISB5              TRISBbits.TRISB5
    #define TRISB6              TRISBbits.TRISB6
    #define TRISB7              TRISBbits.TRISB7
    
	#define LATB0               LATBbits.LATB0
    #define LATB1               LATBbits.LATB1
    #define LATB2               LATBbits.LATB2
    #define LATB3               LATBbits.LATB3
    #define LATB4               LATBbits.LATB4
    #define LATB5               LATBbits.LATB5
    #define LATB6               LATBbits.LATB6
    #define LATB7               LATBbits.LATB7

	#define RB0                 PORTBbits.RB0
    #define RB1                 PORTBbits.RB1
    #define RB2                 PORTBbits.RB2
    #define RB3                 PORTBbits.RB3
    #define RB4                 PORTBbits.RB4
    #define RB5                 PORTBbits.RB5
    #define RB6                 PORTBbits.RB6
    #define RB7                 PORTBbits.RB7

//PORTC defines     
    #define LATC0               LATCbits.LATC0
    #define LATC1               LATCbits.LATC1
    #define LATC2               LATCbits.LATC2
    #define LATC3               LATCbits.LATC3
    #define LATC4               LATCbits.LATC4
    #define LATC5               LATCbits.LATC5
    #define LATC6               LATCbits.LATC6
    #define LATC7               LATCbits.LATC7

    #define TRISC0              TRISCbits.TRISC0
    #define TRISC1              TRISCbits.TRISC1
    #define TRISC2              TRISCbits.TRISC2
    #define TRISC3              TRISCbits.TRISC3
    #define TRISC4              TRISCbits.TRISC4
    #define TRISC5              TRISCbits.TRISC5
    #define TRISC6              TRISCbits.TRISC6
    #define TRISC7              TRISCbits.TRISC7

    #define RC0                 PORTCbits.RC0
    #define RC1                 PORTCbits.RC1
    #define RC2                 PORTCbits.RC2
    #define RC3                 PORTCbits.RC3
    #define RC4                 PORTCbits.RC4
    #define RC5                 PORTCbits.RC5
    #define RC6                 PORTCbits.RC6
    #define RC7                 PORTCbits.RC7

//PORTD defines
    #define TRISD7              TRISDbits.TRISD7
    #define TRISD6              TRISDbits.TRISD6
    #define TRISD5              TRISDbits.TRISD5
    #define TRISD4              TRISDbits.TRISD4
    #define TRISD3              TRISDbits.TRISD3
    #define TRISD2              TRISDbits.TRISD2
    #define TRISD1              TRISDbits.TRISD1
    #define TRISD0              TRISDbits.TRISD0

    #define LATD0               LATDbits.LATD0
    #define LATD1               LATDbits.LATD1
    #define LATD2               LATDbits.LATD2
    #define LATD3               LATDbits.LATD3
    #define LATD4               LATDbits.LATD4
    #define LATD5               LATDbits.LATD5
    #define LATD6               LATDbits.LATD6
    #define LATD7               LATDbits.LATD7

    #define RD7                 PORTDbits.RD7
    #define RD6                 PORTDbits.RD6
    #define RD5                 PORTDbits.RD5
    #define RD4                 PORTDbits.RD4
    #define RD3                 PORTDbits.RD3
    #define RD2                 PORTDbits.RD2
    #define RD1                 PORTDbits.RD1
    #define RD0                 PORTDbits.RD0

//PORTE defines
	#define TRISE9              TRISEbits.TRISE9
	#define TRISE8              TRISEbits.TRISE8
	#define TRISE7              TRISEbits.TRISE7
    #define TRISE6              TRISEbits.TRISE6
    #define TRISE5              TRISEbits.TRISE5
    #define TRISE4              TRISEbits.TRISE4
    #define TRISE3              TRISEbits.TRISE3
    #define TRISE2              TRISEbits.TRISE2
    #define TRISE1              TRISEbits.TRISE1
    #define TRISE0              TRISEbits.TRISE0

    #define LATE0               LATEbits.LATE0
    #define LATE1               LATEbits.LATE1
    #define LATE2               LATEbits.LATE2
    #define LATE3               LATEbits.LATE3
    #define LATE4               LATEbits.LATE4
    #define LATE5               LATEbits.LATE5
    #define LATE6               LATEbits.LATE6
    #define LATE7               LATEbits.LATE7
    #define LATE8               LATEbits.LATE8
    #define LATE9               LATEbits.LATE9

    #define RE0                 PORTEbits.RE0
    #define RE1                 PORTEbits.RE1
    #define RE2                 PORTEbits.RE2
    #define RE3                 PORTEbits.RE3
    #define RE4                 PORTEbits.RE4
    #define RE5                 PORTEbits.RE5
    #define RE6                 PORTEbits.RE6
    #define RE7                 PORTEbits.RE7
    #define RE8                 PORTEbits.RE8
	#define RE9                 PORTEbits.RE9

	#define LATG3				LATGbits.LATG3
	#define TRISG3				TRISGbits.TRISG3
	#define LATG2				LATGbits.LATG2
	#define TRISG2				TRISGbits.TRISG2
	#define RF8					PORTFbits.RF8
	#define RF7					PORTFbits.RF7
	#define LATF8				LATFbits.LATF8
	#define LATF7				LATFbits.LATF7
	#define LATF6				LATFbits.LATF6
	#define TRISF8				TRISFbits.TRISF8
	#define TRISF7				TRISFbits.TRISF7
	#define TRISF6				TRISFbits.TRISF6
	#define TRISF1				TRISFbits.TRISF1
	#define LATF1				LATFbits.LATF1
	#define LATF0				LATFbits.LATF0
	#define LATG6				LATGbits.LATG6
	#define TRISF0				TRISFbits.TRISF0
	#define TRISG6				TRISGbits.TRISG6

		
//other defines

	
#endif


