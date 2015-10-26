#include "app.h"
#include "mmd.h"
#include "utilities.h"
#include "communication.h"
#include "string.h"




void resetSegment(void);

void copySrcToDst(const rom UINT8*src, UINT8* dst , UINT8 length);

#pragma udata app_data

UINT8 segmentBuffer[10];
MMD_Config mmdConfig;
UINT8 line[50] ="LINE "; 
UINT8 flag = 0;
#pragma udata

const UINT8 lED_MAP[11] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00};
UINT8 line1[30] ={'1',' ','2','2','0','N',' ','3','4','0',' ','1','2','3','4',' ','5','6','7',' ','1','2','3','4','5'}; 
UINT8 APP_comCallBack(UINT8* cmd_data, UINT8* respID,  UINT8** resp_data);

void APP_init(void)
{
	UINT8 i;
	COM_init(CMD_SOP , CMD_EOP ,RESP_SOP , RESP_EOP , APP_comCallBack);

	resetSegment();

	mmdConfig.startAddress = 0;
	mmdConfig.length = 6;
	mmdConfig.symbolCount = 5;
	mmdConfig.symbolBuffer = line;
	mmdConfig.scrollSpeed = SCROLL_SPEED_NONE;
			
	MMD_configSegment( 0 , &mmdConfig);

	for( i = 0; i < MODEL_TT_DATA_SIZE; i++ )
		WriteDataToDisplay(MODEL_TT_OFFSET+i, lED_MAP[i]);

	for( i = 0; i < DISP_PLQTY_DATA_SIZE; i++ )
		WriteDataToDisplay(DISP_PLQTY_OFFSET+i, ~lED_MAP[i]);

	for( i = 0; i < HOURLY_RATIO_DATA_SIZE; i++ )
		WriteDataToDisplay(HOURLY_RATIO_OFFSET+i, ~lED_MAP[i]);	

}


void APP_task(void)
{
	UINT8 i;
	UINT8 tempFlag = 0;

	if( flag == 0)
	{
		for( i = 0; i < MODEL_TT_DATA_SIZE; i++ )
			WriteDataToDisplay(MODEL_TT_OFFSET+i, ~lED_MAP[i]);
	
		for( i = 0; i < DISP_PLQTY_DATA_SIZE; i++ )
			WriteDataToDisplay(DISP_PLQTY_OFFSET+i, ~lED_MAP[i]);
	
		for( i = 0; i < DISP_ACTQTY_DATA_SIZE; i++ )
			WriteDataToDisplay(DISP_ACTQTY_OFFSET+i, ~lED_MAP[i]);	
	
		for( i = 0; i < HOURLY_RATIO_DATA_SIZE; i++ )
			WriteDataToDisplay(HOURLY_RATIO_OFFSET+i, ~lED_MAP[i]);	
	
		for( i = 0; i < DISP_RATIO_DATA_SIZE; i++ )
			WriteDataToDisplay(DISP_RATIO_OFFSET+i, ~lED_MAP[i]);
	}	
	else 
	{
		for( i = 0; i < MODEL_TT_DATA_SIZE; i++ )
		{
			if( line[7+i] == ' ' || tempFlag == 1 )
			{
				WriteDataToDisplay(MODEL_TT_OFFSET+i, ~lED_MAP[10]);
				tempFlag = 1;
			}
			else
				WriteDataToDisplay(MODEL_TT_OFFSET+i, ~lED_MAP[(line[7+i]) - '0']);

		}

		tempFlag = 0;	
		for( i = 0; i < DISP_PLQTY_DATA_SIZE; i++ )
		{
			if( line[11+i] == ' ' || tempFlag == 1 )
			{
				WriteDataToDisplay(DISP_PLQTY_OFFSET+i, ~lED_MAP[10]);
				tempFlag = 1;
			}
			else
				WriteDataToDisplay(DISP_PLQTY_OFFSET+i, ~lED_MAP[(line[11+i]) -'0']);

		}

		
		tempFlag = 0;
		for( i = 0; i < DISP_ACTQTY_DATA_SIZE; i++ )
		{
			if( line[16+i] == ' ' || tempFlag == 1 )
			{
				WriteDataToDisplay(DISP_ACTQTY_OFFSET+i, ~lED_MAP[10]);
				tempFlag = 1;
			}
			else
				WriteDataToDisplay(DISP_ACTQTY_OFFSET+i, ~lED_MAP[(line[16+i]) -'0']);	
		}
	
		tempFlag = 0;
		for( i = 0; i < HOURLY_RATIO_DATA_SIZE; i++ )
		{
			if( line[21+i] == ' ' || tempFlag == 1 )
			{
				WriteDataToDisplay(HOURLY_RATIO_OFFSET+i, ~lED_MAP[10]);
				tempFlag = 1;
			}
			else
				WriteDataToDisplay(HOURLY_RATIO_OFFSET+i, ~lED_MAP[(line[21+i]) -'0']);	//21
		}
	
		for( i = 0; i < DISP_RATIO_DATA_SIZE; i++ )
			WriteDataToDisplay(DISP_RATIO_OFFSET+i, ~lED_MAP[10]);//27
	}	


}

	
			


UINT8 APP_comCallBack( UINT8 *rxPacket, UINT8* txCode, UINT8** txPacket)
{
	UINT8 i;

	UINT8 rxCode = rxPacket[0];
	UINT8 mmdCode = rxPacket[1];
	UINT8 length = 0;

	switch( rxCode )
	{
		case CMD_GET_STATUS:
			
		break;
			

		case CMD_STATIC:
			mmdConfig.scrollSpeed = SCROLL_SPEED_NONE;
			MMD_configSegment( 0 , &mmdConfig);
		break;

		case CMD_SCROLL:
			mmdConfig.scrollSpeed = SCROLL_SPEED_HIGH;
			MMD_configSegment( 0 , &mmdConfig);
		break;

		case CMD_MESSAGE:
		{	
			UINT8 i;
			UINT8 tempBuff[6] = {0};
			resetSegment();
			strcpy(line,rxPacket+1);
		
			for( i = 0; i < 6; i++ )
				tempBuff[i] = line[i];

			mmdConfig.startAddress = 0;
			mmdConfig.length = 6;
			mmdConfig.symbolCount = strlen(tempBuff);
			mmdConfig.symbolBuffer = tempBuff;
			mmdConfig.scrollSpeed = SCROLL_SPEED_NONE;

			MMD_configSegment( 0 , &mmdConfig);

			flag = 1;
		}
		break;

		default:
			length = 0;
			*txCode = COM_RESP_INVALID_CMD;
		break;

	}

	return length;

}
	


void resetSegment()
{
	UINT8 i;

	for(i = 0;i < 28 ; i++)
	{
		segmentBuffer[i] = ' ';
	}

	mmdConfig.startAddress = 0;
	mmdConfig.length = MMD_MAX_CHARS;//28;
	mmdConfig.symbolCount = MMD_MAX_CHARS;//28;
	mmdConfig.symbolBuffer = segmentBuffer;
	mmdConfig.scrollSpeed = SCROLL_SPEED_NONE;
	
	MMD_configSegment( 0 , &mmdConfig);

}


void copySrcToDst(const rom UINT8*src, UINT8* dst , UINT8 length)
{
	UINT8 i;
	for( i = 0 ; i < length; i++)
	{
		dst[i] = src[i];
	}
}