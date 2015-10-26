#ifndef SCHEDULAR_H
#define SCHEDULAR_H

/*
*------------------------------------------------------------------------------
* schedular.h
*
* Include file for schedular module.
*
* *
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* File				: schedular.h
* 
*------------------------------------------------------------------------------
*
* 
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/

#include "board.h"

/*
*------------------------------------------------------------------------------
* Public Defines
*------------------------------------------------------------------------------
*/

// Store in DATA area, if possible, for rapid access
// Total memory per task is 7 bytes
typedef struct
{
	// Pointer to the task (must be a 'void (void)' function)
   	rom void (*mpTaskPointer)(void);

   	// mStartupDelay (ticks) until the function will (next) be run
   	// see SCH_AddTask() for further details
   	UINT16 mStartupDelay;

   	// Interval (ticks) between subsequent runs.
   	// see SCH_AddTask() for further details
   	UINT16 mTaskFrequency;

   	// Incremented (by scheduler) when task is due to execute
   	UINT8 mRunMe;
}stTask;

// Add the entry here when a new task is added, so that SCH_MAX_TASKS
// automaticaly pdated.
enum TASKS_ADDED
{
	SCH_MAX_TASKS = 8	//This must be last
};

/*
*------------------------------------------------------------------------------
* Public Macros
*------------------------------------------------------------------------------
*/

// Comment this line out if error reporting is NOT required
//#define SCH_REPORT_ERRORS

#ifdef SCH_REPORT_ERRORS
// The port on which error codes will be displayed
#define Error_port P2
#endif

#ifndef TRUE
#define FALSE 											(0)
#define TRUE 											(!FALSE)
#endif

#define RETURN_NORMAL 									(0)
#define RETURN_ERROR 									(1)


// Error codes
#define ERROR_SCH_TOO_MANY_TASKS 						(1)
#define ERROR_SCH_CANNOT_DELETE_TASK 					(2)

#define ERROR_SCH_WAITING_FOR_SLAVE_TO_ACK 				(3)
#define ERROR_SCH_WAITING_FOR_START_COMMAND_FROM_MASTER (3)

#define ERROR_SCH_ONE_OR_MORE_SLAVES_DID_NOT_START 		(4)
#define ERROR_SCH_LOST_SLAVE 							(5)

#define ERROR_SCH_CAN_BUS_ERROR 						(6)

#define ERROR_I2C_WRITE_BYTE 							(10)
#define ERROR_I2C_READ_BYTE 							(11)
#define ERROR_I2C_WRITE_BYTE_AT24C64 					(12)
#define ERROR_I2C_READ_BYTE_AT24C64 					(13)
#define ERROR_I2C_DS1621 								(14)

#define ERROR_USART_TI 									(21)
#define ERROR_USART_WRITE_CHAR 							(22)

#define ERROR_SPI_EXCHANGE_BYTES_TIMEOUT 				(31)
#define ERROR_SPI_X25_TIMEOUT 							(32)
#define ERROR_SPI_MAX1110_TIMEOUT 						(33)

#define ERROR_ADC_MAX150_TIMEOUT 						(44)


// For 1ms schedular interrupt
// Tcy = 4/Fosc
// 1ms = (4/48000000Hz)*12000
// 2ms = (4/48000000Hz)*24000
//#define SCH_TICK_PERIOD	(65535 - 24000)
// 2ms = (4/20000000Hz) * 10000
//#define SCH_TICK_PERIOD	(65535 - 10000)
// 2ms = (4/40000000Hz) * 20000
#define SCH_TICK_PERIOD	(65535 - 10000)

#define OS_1MSEC_TICK_VAL  								(1)
#define OS_2MSEC_TICK_VAL  								(2)
#define OS_4MSEC_TICK_VAL  								(4)

// Setup the operating system tick time to use for all tasks.
#define OS_TICK_VAL        								(OS_1MSEC_TICK_VAL)

// Define times for use when setting up various periodic tasks.
#define TASK_2MSEC_PERIOD     							(2    / OS_TICK_VAL)
#define TASK_4MSEC_PERIOD     							(4    / OS_TICK_VAL)
#define TASK_6MSEC_PERIOD   							(6    / OS_TICK_VAL)
#define TASK_8MSEC_PERIOD     							(8    / OS_TICK_VAL)
#define TASK_10MSEC_PERIOD    							(10   / OS_TICK_VAL)
#define TASK_12MSEC_PERIOD    							(12   / OS_TICK_VAL)
#define TASK_24MSEC_PERIOD    							(24   / OS_TICK_VAL)
#define TASK_50MSEC_PERIOD    							(50   / OS_TICK_VAL)
#define TASK_76MSEC_PERIOD    							(76   / OS_TICK_VAL)
#define TASK_100MSEC_PERIOD   							(100  / OS_TICK_VAL)
#define TASK_150MSEC_PERIOD   							(150  / OS_TICK_VAL)
#define TASK_200MSEC_PERIOD   							(200  / OS_TICK_VAL)
#define TASK_250MSEC_PERIOD   							(250  / OS_TICK_VAL)
#define TASK_300MSEC_PERIOD   							(300  / OS_TICK_VAL)
#define TASK_500MSEC_PERIOD   							(500  / OS_TICK_VAL)
#define TASK_750MSEC_PERIOD   							(750  / OS_TICK_VAL)
#define TASK_1000MSEC_PERIOD 							(1000 / OS_TICK_VAL)

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
extern void SCH_Start(void);
extern void SCH_init(void);
extern void SCH_Update(void);
extern BOOL SCH_DeleteTask(const UINT8 taskIndex);
extern void SCH_DispatchTasks(void);
extern UINT8 SCH_AddTask(rom void (*pFunction)(),
                         const UINT16 Delay,
                         const UINT16 Period);

#endif
/*
*  End of schedular.h
*/

