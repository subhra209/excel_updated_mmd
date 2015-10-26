
/*
*------------------------------------------------------------------------------
* device.c
*
* Board specipic drivers module(BSP)
*
* (C)2009 Sam's Logic.
*
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* File				: device.c
* Created by		: Sam
* Last changed by	: Sam
* Last changed		: 11/12/2010 
*------------------------------------------------------------------------------
* Revision 1.3 11/12/2010  Sam
* Demo Release
* Revision 1.2 15/11/2010  Sam
* Updated according to the current project requirement
* Revision 1.1 07/07/2010 Sam
* First Release
* Revision 1.0 14/07/2009 Sam
* Initial revision
*
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/


#include "board.h"
#include "config.h"
#include "typedefs.h"




/*
*------------------------------------------------------------------------------
* Private Defines
*------------------------------------------------------------------------------
*/

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

static BOOL ledState;

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
* void InitializeBoard(void)

* Summary	: This function configures all i/o pin directions
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
void BRD_init(void)
{

//	OSCCON = 0X70; // Internal oscillator at 8MHz
//	OSCTUNEbits.PLLEN = 1; 	// PLL is enabled

	// set all anolog channels as Digital I/O
	ADCON1 = 0x0F;


	// Configure heart beat LED output
	HEART_BEAT_DIR 	= PORT_OUT;
	HEART_BEAT 		= ~SWITCH_OFF;

	// Rs485 Direction Controle
	TX_EN_DIR 		= PORT_OUT;
	TX_EN 			= SWITCH_OFF;

	// Configure Serial port
	SER_TX_DIR 		=  PORT_OUT;
	SER_RX_DIR 		=  PORT_IN;

	// Configure I2C port
	I2C_SDA_DIR 	=  PORT_OUT;
	I2C_SCL_DIR	 	=  PORT_OUT;



/*	DISPLAY_CONTROL_DIRECTION = PORT_OUT;
	DISPLAY_CONTROL			  = DISPLAY_DISABLE;


	// Display Data port
	DATA_PORT_DIR = 0x00;
	DIGIT_PORT_DIR = 0x00;
	DIGIT_PORT = 0x00;
	

	DIGIT_SEL_A_DIR = PORT_OUT;
	DIGIT_SEL_A = SWITCH_OFF;
	DIGIT_SEL_B_DIR = PORT_OUT;
	DIGIT_SEL_B = SWITCH_OFF;
	DIGIT_SEL_C_DIR = PORT_OUT;
	DIGIT_SEL_C = SWITCH_OFF;
	DIGIT_SEL_D_DIR = PORT_OUT;
	DIGIT_SEL_D = SWITCH_OFF;
	DIGIT_SEL_E_DIR = PORT_OUT;
	DIGIT_SEL_E = SWITCH_OFF;
	DIGIT_SEL_F_DIR = PORT_OUT;
	DIGIT_SEL_F = SWITCH_OFF;
	DIGIT_SEL_G_DIR = PORT_OUT;
	DIGIT_SEL_G = SWITCH_OFF;


	MMD_CONTROL_DIR = PORT_OUT;
	MMD_CONTROL = SWITCH_OFF;
*/

	DISPLAY_CONTROL_DIRECTION = PORT_OUT;
	DISPLAY_CONTROL			  = DISPLAY_DISABLE;


	// Display Data port
	DATA_PORT_DIR = 0x00;
	DIGIT_PORT_DIR = 0x00;
	DIGIT_PORT = 0x00;
	

	DIGIT_SEL_A_DIR = PORT_OUT;
	DIGIT_SEL_A = SWITCH_OFF;
	DIGIT_SEL_B_DIR = PORT_OUT;
	DIGIT_SEL_B = SWITCH_OFF;
	DIGIT_SEL_C_DIR = PORT_OUT;
	DIGIT_SEL_C = SWITCH_OFF;

	BOARD_SEL_A_DIR = PORT_OUT;
	BOARD_SEL_A = SWITCH_OFF;
	BOARD_SEL_B_DIR = PORT_OUT;
	BOARD_SEL_B = SWITCH_OFF;
	BOARD_SEL_C_DIR	= PORT_OUT;
	BOARD_SEL_C = SWITCH_OFF;
	BOARD_SEL_D_DIR = PORT_OUT;
	BOARD_SEL_D = SWITCH_OFF;
	BOARD_SEL_E_DIR = PORT_OUT;
	BOARD_SEL_E = SWITCH_OFF;


	// Configure Relay output
	MMD_CONTROL_DIR = PORT_OUT;
	MMD_CONTROL = SWITCH_OFF;


}



/*
*------------------------------------------------------------------------------
* Private Functions
*------------------------------------------------------------------------------
*/

/*
*  End of device.c
*/
