#include "app.h"
#include "communication.h"
#include "config.h"
#include "uart.h"

/*
*------------------------------------------------------------------------------
* Structures
*------------------------------------------------------------------------------
*/

typedef struct _EVENT_LOG
{
	UINT8 writeIndex;
	UINT8 readIndex;
	UINT8 logBuffer[MAX_ENTRIES][MAX_LENGTH_OF_ENTRY];
}EVENT_LOG;


/*
*------------------------------------------------------------------------------
* Variables
*------------------------------------------------------------------------------
*/

#pragma idata LOG_DATA
EVENT_LOG log = {0};
#pragma idata


/*------------------------------------------------------------------------------
* Private Functions
*------------------------------------------------------------------------------
*/

void logWrite( far UINT8 **data, UINT8 length );
UINT8 logRead( far UINT8 **logBuff );
UINT8 APP_comCallBack( far UINT8 *rxPacket, far UINT8* txCode,far UINT8** txPacket);
/*
*------------------------------------------------------------------------------
* void APP-init(void)
*------------------------------------------------------------------------------
*/

void APP_init(void)
{
	//Modbus
	UINT16 sbaudrate = 9600, saddress = 1;


	COM_init(CMD_SOP , CMD_EOP ,RESP_SOP , RESP_EOP , APP_comCallBack);
}


/*
*------------------------------------------------------------------------------
* void APP_task(void)
*------------------------------------------------------------------------------
*/

void APP_task(void)
{
	UINT8 data = 0;

	if( UART_hasData() )
	{
		data = UART_read();
		UART_write( data );
	}


}


/*---------------------------------------------------------------------------------------------------------------
*	void updateLog(void)
*----------------------------------------------------------------------------------------------------------------
*/
void logWrite( far UINT8 **data, UINT8 length )
{
	UINT8 i, j = 0;
	UINT8 temp = 0;
 
	length = length * 2;   //length multiplied by number of bytes

	//Store data in the buffer
	for( i = 0, j = 0; i < length; i++ )
	{
		temp = **data ;
		temp &= 0XF0;
		temp >>= 4;

		log.logBuffer[log.writeIndex][j++] = (char) (temp + '0');

		temp = **data ;
		temp &= 0X0F;

		log.logBuffer[log.writeIndex][j++] = (char) (temp + '0');
		
		(*data)++;
	}

	log.logBuffer[log.writeIndex][j] = '\0'; //store null character to indicate end of the packet

	log.writeIndex++;
	if( log.writeIndex >= MAX_ENTRIES)
		log.writeIndex = 0;


}

/*---------------------------------------------------------------------------------------------------------------
*	UINT8 logRead(void)
*----------------------------------------------------------------------------------------------------------------
*/
UINT8 logRead( far UINT8** logBuff )
{
	UINT8 length = 0;

	if( log.writeIndex == log.readIndex )
	{
		*logBuff = 0;
	}
	else
	{
		*logBuff = log.logBuffer[log.readIndex];
		//length = strlen( log.logBuffer[log.readIndex] );
		length = 4;
		
		log.readIndex++;
		if( log.readIndex >= MAX_ENTRIES)
			log.readIndex = 0;	
	}

	return length;
			
}


/*---------------------------------------------------------------------------------------------------------------
*	APP CALLBACK
*----------------------------------------------------------------------------------------------------------------
*/
UINT8 APP_comCallBack( far UINT8 *rxPacket, far UINT8* txCode,far UINT8** txPacket)
{

	UINT8 i;

	UINT8 rxCode = rxPacket[0];
	volatile UINT8 length = 0;

	switch( rxCode )
	{
		case CMD_GET_LOG_STATUS:
			length = (UINT8) logRead(txPacket);
			*txCode = CMD_GET_LOG_STATUS;
		break;

		default:
			length = 0;
			
		break;

	}

	return length;

}