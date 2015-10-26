#ifndef DEVICE_H
#define DEVICE_H

/*
*------------------------------------------------------------------------------
* device.h
*
* Include file for port pin assignments
*

*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* File				: device.h
*------------------------------------------------------------------------------
*

*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/

#include <p18f4620.h>
#include <delays.h>
#include <timers.h>
#include "typedefs.h"
#include "config.h"


/*
*------------------------------------------------------------------------------
* Hardware Port Allocation
*------------------------------------------------------------------------------
*/



// Heart Beat



// I2C Port
#define		I2C_SDA					PORTCbits.RC4			// serial data
#define		I2C_SDA_DIR				TRISCbits.TRISC4
#define		I2C_SCL					PORTCbits.RC3			// serial clock
#define		I2C_SCL_DIR				TRISCbits.TRISC3

// Rs485 / RS232 Serial commnunicaton port
#define		TX_EN					PORTCbits.RC5			// TX control for RS485 communication
#define		TX_EN_DIR				TRISCbits.TRISC5
#define 	SER_TX					PORTCbits.RC6   		// serial transmit
#define		SER_TX_DIR				TRISCbits.TRISC6
#define 	SER_RX					PORTCbits.RC7			// serial receive
#define		SER_RX_DIR				TRISCbits.TRISC7



#define		HEART_BEAT				LATEbits.LATE2			// high - OFF, Low - ON
#define		HEART_BEAT_DIR			TRISEbits.TRISE2

/*
#define 	DISPLAY_CONTROL				PORTCbits.RC0
#define 	DISPLAY_CONTROL_DIRECTION	TRISCbits.TRISC0

#define		MMD_CONTROL				LATCbits.LATC1			// high - ON, Low - OFF
#define		MMD_CONTROL_DIR			TRISCbits.TRISC1


// Column Data port
#define		DATA_PORT				PORTD	//LATD	 				// 7seg display data (DB0-DB7)
#define 	DATA_PORT_DIR			TRISD

// Address port
#define		DIGIT_PORT				PORTB	//LATB	 				// digit drivers(upto 128 digits)
#define		DIGIT_PORT_DIR			TRISB	//LATB	 				// digit drivers(upto 128 digits)




// Row Control Lines
#define		DIGIT_SEL_A				PORTAbits.RA0 			// demux digit sel A
#define		DIGIT_SEL_A_DIR			TRISAbits.TRISA0
#define		DIGIT_SEL_B				PORTAbits.RA1			// demux digit sel B
#define		DIGIT_SEL_B_DIR			TRISAbits.TRISA1
#define		DIGIT_SEL_C				PORTAbits.RA2			// demux digit sel C
#define		DIGIT_SEL_C_DIR			TRISAbits.TRISA2
#define		DIGIT_SEL_D				PORTAbits.RA3			// demux digit sel D
#define		DIGIT_SEL_D_DIR			TRISAbits.TRISA3
#define		DIGIT_SEL_E				PORTAbits.RA4			// demux digit sel E
#define		DIGIT_SEL_E_DIR			TRISAbits.TRISA4
#define		DIGIT_SEL_F				PORTAbits.RA5			// demux digit sel F
#define		DIGIT_SEL_F_DIR			TRISAbits.TRISA5
#define		DIGIT_SEL_G				PORTEbits.RE0			// demux digit sel G
#define		DIGIT_SEL_G_DIR			TRISEbits.TRISE0
#define		DIGIT_SEL_H				PORTEbits.RE1			// demux digit sel H
#define		DIGIT_SEL_H_DIR			TRISEbits.TRISE1
*/

//Display control 

#define 	DISPLAY_CONTROL				PORTEbits.RE0
#define 	DISPLAY_CONTROL_DIRECTION	TRISEbits.TRISE0

// Relay Drive O/P
#define		MMD_CONTROL				LATAbits.LATA2			// high - ON, Low - OFF
#define		MMD_CONTROL_DIR			TRISAbits.TRISA2


// Display Data port
#define		DATA_PORT				PORTD	//LATD	 				// 7seg display data (DB0-DB7)
#define 	DATA_PORT_DIR			TRISD
#define		DIGIT_PORT				PORTB	//LATB	 				// digit drivers(upto 128 digits)
#define		DIGIT_PORT_DIR			TRISB	//LATB	 				// digit drivers(upto 128 digits)



// Display digit select lines
#define		DIGIT_SEL_A				PORTCbits.RC0 			// demux digit sel A
#define		DIGIT_SEL_A_DIR			TRISCbits.TRISC0
#define		DIGIT_SEL_B				PORTCbits.RC1			// demux digit sel B
#define		DIGIT_SEL_B_DIR			TRISCbits.TRISC1
#define		DIGIT_SEL_C				PORTCbits.RC2			// demux digit sel C
#define		DIGIT_SEL_C_DIR			TRISCbits.TRISC2

// Display board select lines
#define		BOARD_SEL_A				PORTBbits.RB3			// demux board sel A
#define		BOARD_SEL_A_DIR			TRISBbits.TRISB3
#define		BOARD_SEL_B				PORTBbits.RB4			// demux board sel B
#define		BOARD_SEL_B_DIR			TRISBbits.TRISB4
#define		BOARD_SEL_C				PORTBbits.RB5			// demux board sel C
#define		BOARD_SEL_C_DIR			TRISBbits.TRISB5
#define		BOARD_SEL_D				PORTBbits.RB6			// demux board sel D
#define		BOARD_SEL_D_DIR			TRISBbits.TRISB6
#define		BOARD_SEL_E				PORTBbits.RB7			// demux board sel E
#define		BOARD_SEL_E_DIR			TRISBbits.TRISB7

/*------------------------------------------------------------------------------
* Public Defines
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Macros
*------------------------------------------------------------------------------
*/

#define DEVICE_ADDRESS			(0x01)


//#define SYSTEM_CLOCK			(32000000UL)	// Hz		// 8MHz internal crystal with x4 PLL

#define SYSTEM_CLOCK			(40000000UL)	// Hz		// 10MHz external crystal x4 PLL

#define PERIPHERA_CLOCK			(SYSTEM_CLOCK / 4)UL

#define PERIPHERA_CLOCK_PERIOD 	(1 / PERIPHERA_CLOCK)UL



// Direction controle bit is processor specific ,
#define PORT_OUT				(BOOL)(0)
#define PORT_IN					(BOOL)(1)

#define OFF_FOREVER				(BOOL)(0)
#define LOOP_FOREVER			(BOOL)(1)

#define SWITCH_OFF				(BOOL)(0)
#define SWITCH_ON				(BOOL)(1)


#define DISPLAY_DISABLE			(BOOL)(1)
#define DISPLAY_ENABLE			(BOOL)(0)


/*
#ifdef __74LS374__
#undef DISPLAY_DISABLE
#undef DISPLAY_ENABLE

#define DISPLAY_DISABLE			(BOOL)(0)
#define DISPLAY_ENABLE			(BOOL)(1)

#endif 			//__74LS374__
*/


#define GetSystemClock()		(SYSTEM_CLOCK)      // Hz
#define GetInstructionClock()	(GetSystemClock()/4)
#define GetPeripheralClock()	GetInstructionClock()


#define BUZZER_ON()			BUZ_OP = SWITCH_ON;
#define BUZZER_OFF()		BUZ_OP = SWITCH_OFF;

// Disable global interrupt bit.
#define ENTER_CRITICAL_SECTION()	INTCONbits.GIE = 0;

// Enable global interrupt bit.
#define EXIT_CRITICAL_SECTION()		INTCONbits.GIE = 1;

#define ENABLE_GLOBAL_INT()			EXIT_CRITICAL_SECTION()


#define DISABLE_INT0_INTERRUPT()	INTCONbits.INT0IE = 0
#define ENABLE_INT0_INTERRUPT()		INTCONbits.INT0IE = 1
#define CLEAR_INTO_INTERRUPT()		INTCONbits.INT0IF = 0


#define DISABLE_TMR0_INTERRUPT()	INTCONbits.TMR0IE = 0
#define ENABLE_TMR0_INTERRUPT()		INTCONbits.TMR0IE = 1

#define DISABLE_TMR1_INTERRUPT()	PIE1bits.TMR1IE = 0
#define ENABLE_TMR1_INTERRUPT()		PIE1bits.TMR1IE = 1


#define HOLD_ATKBD_CLOCK()		    ATKBD_CLK_DIR= PORT_OUT;\
									ATKBD_CLK = 0;

#define RELEASE_ATKBD_CLOCK()	  	ATKBD_CLK_DIR = PORT_IN;\
									ATKBD_CLK = 1;

#define SET_INT0_FALLING_EDGE_TRIGGER()	INTCON2bits.INTEDG0 = 0

#define	ATKBD_INTERRUPT_EDGE	INTCON2.INTEDG0		

#define ENB_485_TX()	TX_EN = 1;
#define ENB_485_RX()	TX_EN = 0

#define DISABLE_UART_RX_INTERRUPT()	PIE1bits.RCIE = 0
#define ENABLE_UART_RX_INTERRUPT()	PIE1bits.RCIE = 1

#define Delay10us(us)		Delay10TCYx(((GetInstructionClock()/1000000)*(us)))
#define DelayMs(ms)												\
	do																\
	{																\
		unsigned int _iTemp = (ms); 								\
		while(_iTemp--)												\
			Delay1KTCYx((GetInstructionClock()+999999)/1000000);	\
	} while(0)

/*
*------------------------------------------------------------------------------
* Public Data Types
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Variables (extern)
*------------------------------------------------------------------------------
*/


/*
*------------------------------------------------------------------------------
* Public Constants (extern)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Function Prototypes (extern)
*------------------------------------------------------------------------------
*/

extern void BRD_init(void);

#endif
/*
*  End of device.h
*/