#include "board.h"



//#define		DISP_TIME_OFFSET			(0)
//#define		DISP_TIME_DATA_SIZE			(6)

#define 	MODEL_TT_OFFSET				(6)
#define 	MODEL_TT_DATA_SIZE			(3)

#define		DISP_PLQTY_OFFSET			(9)
#define		DISP_PLQTY_DATA_SIZE		(4)

#define		DISP_ACTQTY_OFFSET			(13)
#define		DISP_ACTQTY_DATA_SIZE		(4)

#define		HOURLY_RATIO_OFFSET			(17)
#define		HOURLY_RATIO_DATA_SIZE		(5)

#define		DISP_RATIO_OFFSET			(22)
#define		DISP_RATIO_DATA_SIZE		(5)



typedef enum
{
	APP_STATE_NONE = 0,
	APP_STATE_ACTIVE = 1,
	APP_STATE_INACTIVE = 2
}APP_STATE;		



typedef enum
{
	ACTIVITY_NONE = 0,
	ACTIVITY_PICKING = 1,
	ACTIVITY_STAGING = 2,
	ACTIVITY_LOADING = 3
}ACTIVITY; 


typedef enum
{
	RESET = 0,
	ON_TIME = 1,
	DELAYED = 2
}STATUS;


typedef enum
{
	CMD_GET_STATUS  = 0x80,
	CMD_STATIC      = 0X81,
	CMD_SCROLL		= 0x82,
	CMD_MESSAGE		= 0X83
};





	

void APP_init(void);
void APP_task(void);


