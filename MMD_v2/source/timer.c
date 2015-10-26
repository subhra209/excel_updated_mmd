#include "timer.h"

typedef struct _TMR
{
	unsigned int reload;
	void (*func)(void);
}TMR;

TMR tmr[2] = { 0 , 0 };


UINT16 count = 0;
UINT16 heartBeatCount  =0 ;
UINT16 mmdTaskCount  =0 ;
UINT16 appUpdateCount = 0;

/*
*------------------------------------------------------------------------------
* void TMR0_ISR(void)
*
* Summary	: Timer0 ISR for periodi tick for schedular base
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
#pragma code
#pragma interrupt TMR0_ISR
void TMR0_ISR(void)
{
  	/*
   	* Clears the TMR0 interrupt flag to stop the program from processing the
   	* same interrupt multiple times.
   	*/
  	INTCONbits.TMR0IF = 0;
	// Reload value for 5ms overflow
	WriteTimer0(tmr[0].reload);

	if(tmr[0].func != 0 )
		(*(tmr[0].func))();

	++heartBeatCount;
	++mmdTaskCount;
	++appUpdateCount;
}
#pragma code

/*
*------------------------------------------------------------------------------
* void TMR1_ISR(void)
*
* Summary	: Timer1 ISR for periodi tick for multiplexed display refresh
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
#pragma code
#pragma interrupt TMR1_ISR
void TMR1_ISR(void)
{
  	/*
   	* Clears the TMR0 interrupt flag to stop the program from processing the
   	* same interrupt multiple times.
   	*/
  	PIR1bits.TMR1IF = 0;
	// Reload value for 1ms overflow
	WriteTimer1(tmr[1].reload);

	if(tmr[1].func != 0 )
		(*(tmr[1].func))();	
}
#pragma code

/*
*------------------------------------------------------------------------------
* void TMR0_init(void)

* Summary	: Initialize timer0 for schedular base
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
void TMR0_init(UINT16 reload , void (*func)(void))
{
	// Enable the TMR0 interrupt,16bit counter
	// with internal clock,No prescalar.
   	OpenTimer0(TIMER_INT_ON & T0_SOURCE_INT & T0_16BIT & T0_PS_1_1);
	// Reload value for 5ms overflow
	WriteTimer0(reload);
	
	tmr[0].reload = reload;
	tmr[0].func = func;
}

/*
*------------------------------------------------------------------------------
* void TMR1_init(void)

* Summary	: Initialize timer1 for display refresh
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
void TMR1_init(unsigned int reload , void (*func)())
{
	// Enagle TMR1 interrrupt,16 bit counter, with internal clock, No prescalar
	OpenTimer1(TIMER_INT_ON & T1_16BIT_RW & T1_SOURCE_INT & T1_PS_1_1 & T1_OSC1EN_OFF & T1_SYNC_EXT_OFF );
	// Reload value for 1ms overflow
	WriteTimer1(reload);
	// Make timer1 interrupt high priority
  	IPR1bits.TMR1IP = 1;

	tmr[1].reload = reload;
	tmr[1].func = func;

}
