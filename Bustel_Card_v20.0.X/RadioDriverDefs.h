 /*********************************************************************
 *
 *                  Radio Utility Driver specific defs.
 *
 *********************************************************************
 * FileName:        RadioDriverdefs.h
 * Dependencies:    None
 * Processor:       PIC18/PIC24
 * Complier:        MCC18 v1.00.50 or higher, C30 3.10 or higher
 *                  HITECH PICC-18 V8.10PL1 or higher
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the "Company") is intended and supplied to you, the Company's
 * customer, for use solely and exclusively with products manufactured
 * by the Company.
 *
 * The software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *****************************************************************************/

//#define CHIP_VERSION_A1   //Uncomment this for A1 version chipset and comment the next two lines
#define CHIP_VERSION_REV1	//Uncomment this for REV1 chips and comment out the remaining
//#define CHIP_VERSION_SX	//Uncomment this for SX chip (Internal) and comment out the remaining
#define BAUD_RATE 19200
//IEEE EUI - globally unique number
#define EUI_7 0x00
#define EUI_6 0x01
#define EUI_5 0x02
#define EUI_4 0x03
#define EUI_3 0x04
#define EUI_2 0x05
#define EUI_1 0x06
#define EUI_0 0x07
#define HOTKEYS	case 26:\
						goto MainMenu; \
				case 24:\
						ResetMRF89XA();\
						goto MainMenu;\
             	case 19:\
						PrintStatus();\
						break;	
				

#if defined(__18F87J11)
	#define CLOCK_FREQ 10000000
#else
	#define CLOCK_FREQ 8000000
#endif
	//Baud rate is set to 19200 for communication through UART

//MRF49XA Reset, INT, CS pin mappings and LED port connections for each demo board

#if defined (__18F87J11) //PIC18 Explorer board defines
       // Transceiver Configuration
#if defined(CHIP_VERSION_A1)
    #define PHY_IRQ1 INTCON3bits.INT3IF
    #define PHY_IRQ1_En INTCON3bits.INT3IE
	#define PHY_IRQ0 RB4
	#define PHY_IRQ0_TRIS TRISB4	//Since this is port connection, direction needs to be defined
	#define Config_nCS LATE3
    #define Config_nCS_TRIS TRISE3
	#define Data_nCS LATC2
	#define Data_nCS_TRIS TRISC2
	#define PHY_SDI RC5
	#define PHY_SDI_TRIS TRISC5
	#define PHY_SDO LATC5
	#define PHY_SDO_TRIS TRISC4
	#define PHY_SCK LATC3
	#define PHY_SCK_TRIS TRISC3
	#define LED_1 LATD0
   	#define LED_2 LATD1
   	#define LED_1_TRIS TRISD0
   	#define LED_2_TRIS TRISD1
	#define EE_nCS_TRIS TRISE2
	#define EE_nCS LATE2
	#define IRQ0_Received RB4
	#define PHY_IRQ0_En 1
#elif defined(CHIP_VERSION_REV1)
#define PHY_IRQ1 INTCON3bits.INT3IF
    #define PHY_IRQ1_En INTCON3bits.INT3IE
	#define PHY_IRQ0 RB4
	#define PHY_IRQ0_TRIS TRISB4	//Since this is port connection, direction needs to be defined
	#define Config_nCS LATE3
    #define Config_nCS_TRIS TRISE3
	#define Data_nCS LATC2
	#define Data_nCS_TRIS TRISC2
	#define PHY_SDI RC5
	#define PHY_SDI_TRIS TRISC5
	#define PHY_SDO LATC5
	#define PHY_SDO_TRIS TRISC4
	#define PHY_SCK LATC3
	#define PHY_SCK_TRIS TRISC3
	#define LED_1 LATD0
   	#define LED_2 LATD1
   	#define LED_1_TRIS TRISD0
   	#define LED_2_TRIS TRISD1
	#define EE_nCS_TRIS TRISE2
	#define EE_nCS LATE2
	#define IRQ0_Received RB4
	#define RESET_TRIS	TRISB5
	#define	RESET_PIN	LATB5
	#define PHY_IRQ0_En 1
#endif
#else			// Explorer 16 board defines
	#if defined(CHIP_VERSION_A1)
    #define PHY_IRQ0 IFS1bits.INT1IF
    #define PHY_IRQ0_En IEC1bits.INT1IE
	#define PHY_IRQ1 IFS1bits.INT2IF
	#define PHY_IRQ1_En IEC1bits.INT2IE	
	#define PHY_IRQ0_TRIS TRISE8
	#define PHY_IRQ1_TRIS TRISE9
	#define Config_nCS LATB1
    #define Config_nCS_TRIS TRISB1
	#define Data_nCS LATB2
	#define Data_nCS_TRIS TRISB2
	#define PHY_SDI RF8
	#define PHY_SDI_TRIS TRISF8
	#define PHY_SDO LATF7
	#define PHY_SDO_TRIS TRISF7
	#define PHY_SCK LATF6
	#define PHY_SCK_TRIS TRISF6
	#define LED_1 LATA7
    #define LED_2 LATA6
    #define LED_1_TRIS TRISA7
    #define LED_2_TRIS TRISA6
	#define EE_nCS_TRIS TRISF1
	#define EE_nCS LATF1
	#define DATA_PIN	LATG3
	#define DATA_PIN_TRIS	TRISG3
	#elif defined(CHIP_VERSION_SX)
	#define PHY_IRQ0 IFS1bits.INT1IF
    #define PHY_IRQ0_En IEC1bits.INT1IE
	#define PHY_IRQ1 IFS1bits.INT2IF
	#define PHY_IRQ1_En IEC1bits.INT2IE	
	#define PHY_IRQ0_TRIS TRISE8
	#define PHY_IRQ1_TRIS TRISE9
	#define Config_nCS LATF0
    #define Config_nCS_TRIS TRISF0
	#define Data_nCS LATG6
	#define Data_nCS_TRIS TRISG6
	#define PHY_SDI RF8
	#define PHY_SDI_TRIS TRISF8
	#define PHY_SDO LATF7
	#define PHY_SDO_TRIS TRISF7
	#define PHY_SCK LATF6
	#define PHY_SCK_TRIS TRISF6
	#define LED_1 LATA7
    #define LED_2 LATA6
    #define LED_1_TRIS TRISA7
    #define LED_2_TRIS TRISA6
	#define EE_nCS_TRIS TRISF1
	#define EE_nCS LATF1
	#elif defined(CHIP_VERSION_REV1)
	#define PHY_IRQ0 IFS1bits.INT1IF
    #define PHY_IRQ0_En IEC1bits.INT1IE
	#define PHY_IRQ1 IFS1bits.INT2IF
	#define PHY_IRQ1_En IEC1bits.INT2IE	
	#define PHY_IRQ0_TRIS TRISE8
	#define PHY_IRQ1_TRIS TRISE9
	#define Config_nCS LATB1
    #define Config_nCS_TRIS TRISB1
	#define Data_nCS LATB2
	#define Data_nCS_TRIS TRISB2
	#define PHY_SDI RF8
	#define PHY_SDI_TRIS TRISF8
	#define PHY_SDO LATF7
	#define PHY_SDO_TRIS TRISF7
	#define PHY_SCK LATF6
	#define PHY_SCK_TRIS TRISF6
	#define LED_1 LATA7
    #define LED_2 LATA6
    #define LED_1_TRIS TRISA7
    #define LED_2_TRIS TRISA6
	#define EE_nCS_TRIS TRISF1
	#define EE_nCS LATF1
	#define DATA_PIN	LATG3
	#define DATA_PIN_TRIS	TRISG3
	#define RESET_TRIS	TRISG2
	#define RESET_PIN	LATG2
	#endif
#endif
#define PACKET_LEN 64
#define PAYLOAD_LEN 64

//defines
#define VERSION "1.0"
