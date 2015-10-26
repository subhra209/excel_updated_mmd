#ifndef APP_H
#define APP_H

#include "board.h"



#define MAX_ENTRIES 50
#define	MAX_LENGTH_OF_ENTRY 5 

enum
{
	CMD_GET_LOG_STATUS = 0X80
};

void APP_init(void);
void APP_task(void);

#endif