/*********************************************************************
 *
 *                  SPI specific defs.
 *
 *********************************************************************
 * FileName:        spi.h
 * Dependencies:    None
 * Processor:       PIC18/PIC24
 * Complier:        MCC18 v1.00.50 or higher, C30 3.10 or higher
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
#ifndef _SPI_H
#define _SPI_H

#include "Compiler.h"
#include "GenericTypeDefs.h"

#if defined (__18F87J11)
	#define	SSPSTAT	SSP1STAT
	#define SSPCON	SSP1CON1
	#define SSPBUF	SSP1BUF
	#define WCOL	SSP1CON1bits.WCOL
	#define SSPIE	PIE1bits.SSP1IE
	#define SSPIF	PIR1bits.SSP1IF
	#ifndef	GIE		
			#define GIE	INTCONbits.GIE
	#endif
	#ifndef PIE
			#define PIE	INTCONbits.PIE
	#endif

#endif
	//SPI functions
	#define SPIInit()   SSPIF = 1
	void SPIPut(BYTE);
	BYTE SPIGet(void);
#endif
