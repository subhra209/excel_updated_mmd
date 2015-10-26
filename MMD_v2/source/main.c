
/*
*------------------------------------------------------------------------------
* main.c
*
*------------------------------------------------------------------------------
*/



/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/

#include <timers.h>				// Timer library functions
#include <delays.h>				// Dely library functions
#include <string.h>				// String library functions
#include "board.h"				// board setup
#include "timer.h"
#include "heartbeat.h"
#include "mmd.h"
#include "uart_driver.h"

#include "communication.h"
#include "app.h"
/*
*------------------------------------------------------------------------------
* Private Defines
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Processor config bits
*------------------------------------------------------------------------------
*/

#pragma config OSC      = HSPLL//INTIO67//
#pragma config FCMEN    = OFF
#pragma config IESO     = OFF
#pragma config PWRT     = OFF
#pragma config BOREN    = ON
#pragma config BORV     = 3
#pragma config WDT      = OFF
#pragma config WDTPS    = 512	
#pragma config MCLRE    = ON
#pragma config LPT1OSC  = OFF
#pragma config PBADEN   = OFF
#pragma config STVREN   = ON
#pragma config LVP      = OFF
//#pragma config ICPRT  = OFF       // Dedicated In-Circuit Debug/Programming
#pragma config XINST    = OFF       // Extended Instruction Set
#pragma config CP0      = OFF
#pragma config CP1      = OFF
#pragma config CP2      = OFF
#pragma config CP3      = OFF
#pragma config CPB      = OFF
#pragma config CPD      = OFF
#pragma config WRT0     = OFF
#pragma config WRT1     = OFF
#pragma config WRT2     = OFF
//#pragma config WRT3   = OFF
#pragma config WRTB     = OFF//N       // Boot Block Write Protection
#pragma config WRTC     = OFF
#pragma config WRTD     = OFF
#pragma config EBTR0    = OFF
#pragma config EBTR1    = OFF
#pragma config EBTR2    = OFF
#pragma config EBTR3    = OFF
#pragma config EBTRB    = OFF
/*
*------------------------------------------------------------------------------
* Private Macros
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Data Types
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Variables
*------------------------------------------------------------------------------
*/


/*
*------------------------------------------------------------------------------
* Private Variables (static)
*------------------------------------------------------------------------------
*/
extern UINT16 count;
extern UINT16 heartBeatCount;
extern UINT16 mmdTaskCount;
extern UINT16 appUpdateCount;

/*
*------------------------------------------------------------------------------
* Public Constants
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Constants (static)
*------------------------------------------------------------------------------
*/
#define TIMER0_RELOAD ( 65535 - 10000 )  // 1ms
/*
*------------------------------------------------------------------------------
* Private Function Prototypes (static)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Functions
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Functions
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* void main(void)

* Summary	: Application specifc main routine. Initializes all port and
*			: pheriperal and put the main task into an infinite loop.
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
const UINT8 LED_MAP[11] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00};
#define MMD_REFRESH_PERIOD	(65535 - 20000)

void main(void)
{
	UINT8 i,j;

	BRD_init();			//board initialization

	HB_init();			//initialize heart beat module

	MMD_init();			//initialize mmd module

	APP_init();

	TMR0_init(TIMER0_RELOAD,0);						//initialize timer0 

	TMR1_init(MMD_REFRESH_PERIOD,MMD_refreshDisplay);							//initialize timer1

	UART_init();

/*	SCH_AddTask(HB_task, 100,TASK_500MSEC_PERIOD);	//add heart beat task

	SCH_AddTask(MMD_task, 50,TASK_1000MSEC_PERIOD);	//add mmd task

	SCH_AddTask(COM_task, 50,TASK_10MSEC_PERIOD);	//add communication task

	SCH_AddTask(APP_task, 200,TASK_500MSEC_PERIOD);	//add application task 
*/

	EnableInterrupts( );

   	while( LOOP_FOREVER )
    {
		if( heartBeatCount == 500 )
		{
			HB_task( );
			heartBeatCount = 0;
		}

		if( mmdTaskCount == 100 )
		{
			MMD_task( );
			mmdTaskCount = 0;
		}
	
		if( appUpdateCount == 500 )
		{
			APP_task( );
			appUpdateCount = 0;
		}

		COM_task( );
		ClrWdt();				// Kick the dog
    }
}

/*
*  End of main.c
*/
