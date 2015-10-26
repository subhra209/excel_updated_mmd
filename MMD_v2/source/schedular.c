/*
*------------------------------------------------------------------------------
* schedular.c
*


/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/

#include "board.h"
#include "config.h"
#include "schedular.h"


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
#pragma udata sch_data

// The array of tasks
stTask SchedularTaks[SCH_MAX_TASKS];

// Used to display the error code
UINT8 ErrorCode;
#pragma udata
/*
*------------------------------------------------------------------------------
* Private Variables (static)
*------------------------------------------------------------------------------
*/

// Keeps track of time since last error was recorded
static UINT16 ErrorTickCount;

// The code of the last error (reset after ~1 minute)
static UINT8 LastErrorCode;

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

static void SCH_GoToSleep(void);
static void SCH_ReportStatus(void);

/*
*------------------------------------------------------------------------------
* Public Functions
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* void SCH_init(void)

* Summary	: Scheduler initialisation function.  Prepares scheduler
*  			  data structures and sets up timer interrupts at required rate.
* 			  Must call this function before using the scheduler.
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
void SCH_init(void)
{
	UINT8 taskIndex;

   	for (taskIndex = 0; taskIndex < SCH_MAX_TASKS; taskIndex++)
    {
      	SCH_DeleteTask(taskIndex);
 	}

	ErrorCode = 0;			//Reset the global error variable

}

/*
*------------------------------------------------------------------------------
* void SCH_Update(void)

* Summary	: This is the scheduler routine.  It is called at a rate
*  			  determined by the timer settings in InitSchedularTimer().
*			  This is triggered by Timer 0 interrupts.
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
void SCH_Update(void)
{
	UINT8 Index;
	// NOTE: calculations are in *TICKS* (not milliseconds)
	for (Index = 0; Index < SCH_MAX_TASKS; Index++)
	{
		// Check if there is a task at this location
		if (SchedularTaks[Index].mpTaskPointer)
		{
			if (SchedularTaks[Index].mStartupDelay == 0)
			{
				// The task is due to run
				SchedularTaks[Index].mRunMe += 1;  // Inc. the 'Run Me' flag

				if (SchedularTaks[Index].mTaskFrequency)
				{
					// Schedule periodic tasks to run again
					SchedularTaks[Index].mStartupDelay = SchedularTaks[Index].mTaskFrequency;
				}
			}
			else
			{
				// Not yet ready to run: just decrement the mStartupDelay
				SchedularTaks[Index].mStartupDelay -= 1;
			}
		}
	}
}

/*
*------------------------------------------------------------------------------
* void SCH_Start(void)

* Summary	: Starts the scheduler, by enabling interrupts.
*             to keep the tasks synchronised.
*
* Input		: None
*
* Output	: None
*
* Note		: Usually called after all regular tasks are added.
*			  ONLY THE SCHEDULER INTERRUPT SHOULD BE ENABLED!
*
*------------------------------------------------------------------------------
*/
void SCH_Start(void)
{
	// Enable interrupt priority
  	RCONbits.IPEN = 1;
 	// Enable all high priority interrupts
  	INTCONbits.GIEH = 1;

	EXIT_CRITICAL_SECTION();
}

/*
*------------------------------------------------------------------------------
* void SCH_DispatchTasks(void)

* Summary	: This is the 'dispatcher' function.  When a task (function)
*		      is due to run, SCH_DispatchTasks() will run it.
*		      This function must be called (repeatedly) from the main loop.
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
void SCH_DispatchTasks(void)
{
	UINT8 index;

   	// Dispatches (runs) the next task (if one is ready)
   	for (index = 0; index < SCH_MAX_TASKS; index++)
    {
    	if (SchedularTaks[index].mRunMe > 0)
        {
         	(*SchedularTaks[index].mpTaskPointer)();  // Run the task

         	SchedularTaks[index].mRunMe -= 1;   // Reset / reduce mRunMe flag

         	// Periodic tasks will automatically run again
         	// if this is a 'one shot' task, remove it from the array
         	if (SchedularTaks[index].mTaskFrequency == 0)
            {
            	SCH_DeleteTask(index);
            }
         }
      }
	// Report system status
   	SCH_ReportStatus();
   	// The scheduler enters idle mode at this point
   	SCH_GoToSleep();
   }

/*
*------------------------------------------------------------------------------
* UINT8 SCH_AddTask(void (far rom void * pFunction)(),
*                   const UINT16 Delay,
*                   const UINT16 Period)
*
* Summary	: Causes a task (function) to be executed at regular intervals
*  			  or after a user-defined delay.
*
* Input		: pFunction - The name of the function which is to be scheduled.
*             NOTE: All scheduled functions must be 'void,that is, they must
*			  take no parameters, and have a void return type.
*
*             Delay  - The interval (TICKS) before the task is first executed
*
* 			  Period - If 'Period' is 0, the function is only called once,
*             at the time determined by 'Delay'.  If Period is non-zero,
*             then the function is called repeatedly at an interval
*             determined by the value of mTaskFrequency (see below for examples
*             which should help clarify this).
*
*
* Output	: Returns the position in the task array at which the task has been
*  			  added.  If the return value is SCH_MAX_TASKS then the task could
*  			  not be added to the array (there was insufficient space).  If the
*    		  return value is < SCH_MAX_TASKS, then the task was added
*  			  successfully.
*
* Note		: this return value may be required, if a task is
*  			  to be subsequently deleted - see SCH_DeleteTask().
*
* Examples	: Task_ID = SCH_AddTask(Do_X,1000,0);
*  			  Causes the function Do_X() to be executed once after 1000 sch ticks.
*
*  			  Task_ID = SCH_AddTask(Do_X,0,1000);
*  			  Causes the function Do_X() to be executed regularly, every 1000 sch ticks.
*
*  			  Task_ID = SCH_AddTask(Do_X,300,1000);
*  			  Causes the function Do_X() to be executed regularly, every 1000 ticks.
*  			  Task will be first executed at T = 300 ticks, then 1300, 2300, etc.
*
*------------------------------------------------------------------------------
*/
UINT8 SCH_AddTask(rom void (*pFunction)(),
                  const UINT16 Delay,
                  const UINT16 Period)
{
	UINT8 Index = 0;

   	// First find a gap in the array (if there is one)
   	while ((SchedularTaks[Index].mpTaskPointer != 0) && (Index < SCH_MAX_TASKS))
    {
    	Index++;
    }

   	// Have we reached the end of the list?
   	if (Index == SCH_MAX_TASKS)
    {
    	// Task list is full
      	// Set the global error variable
      	ErrorCode = ERROR_SCH_TOO_MANY_TASKS;

      	// Also return an error code
      	return SCH_MAX_TASKS;
   	}

   	// If we're here, there is a space in the task array
   	SchedularTaks[Index].mpTaskPointer  = pFunction;

   	SchedularTaks[Index].mStartupDelay  = Delay;
   	SchedularTaks[Index].mTaskFrequency = Period;

   	SchedularTaks[Index].mRunMe  = 0;
	// return position of task (to allow later deletion)
   	return Index;
}

/*
*------------------------------------------------------------------------------
* BOOL SCH_DeleteTask(const UINT8 taskIndex)

* Summary	: Removes a task from the scheduler. Note that this does
*			  not delete the associated function from memory:
*  			  it simply means that it is no longer called by the scheduler
*
* Input		: const UINT8 taskIndex- The task index.  Provided by SCH_AddTask().
*
* Output	: BOOL - RETURN_ERROR or RETURN_NORMAL
*
*------------------------------------------------------------------------------
*/
BOOL SCH_DeleteTask(const UINT8 taskIndex)
{
   	BOOL Return_code;

   	if (SchedularTaks[taskIndex].mpTaskPointer == 0)
    {
    	// No task at this location...
        // Set the global error variable
      	ErrorCode = ERROR_SCH_CANNOT_DELETE_TASK;

      	// also return an error code
      	Return_code = RETURN_ERROR;
     }
   	else
    {
    	Return_code = RETURN_NORMAL;
    }

   	SchedularTaks[taskIndex].mpTaskPointer   = 0x0000;
   	SchedularTaks[taskIndex].mStartupDelay   = 0;
   	SchedularTaks[taskIndex].mTaskFrequency  = 0;

   	SchedularTaks[taskIndex].mRunMe   = 0;

   	return Return_code;       // return status
}

/*
*------------------------------------------------------------------------------
* Private Functions
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* void SCH_ReportStatus(void)

* Summary	: Simple function to display error codes.This displays code on a
*			  port with attached LEDs adapt, if required, to report errors
*			  over serial link, etc.Errors are only displayed for a limited
*		      Period (60000 ticks = 1 minute at 1ms tick interval).
*			  After this the the error code is reset to 0.This code may be
*			  easily adapted to display the last error 'for ever'.
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
void SCH_ReportStatus(void)
{
#ifdef SCH_REPORT_ERRORS
	// Only applies if we are reporting errors.
   	// Check for a new error code
   	if (ErrorCode != LastErrorCode)
    {
    	// Negative logic on LEDs assumed
      	Error_port = 255 - ErrorCode;

      	LastErrorCode = ErrorCode;

      	if (ErrorCode != 0)
        {
         	ErrorTickCount = 60000;
        }
      	else
        {
         	ErrorTickCount = 0;
        }
  	}
   	else
    {
    	if (ErrorTickCount != 0)
        {
        	if (--ErrorTickCount == 0)
            {
            	ErrorCode = 0; // Reset error code
            }
         }
   	}
#endif
}

/*
*------------------------------------------------------------------------------
* void SCH_GoToSleep(void)

* Summary	: This scheduler enters 'idle mode' between clock ticks to save power.
*			  The next clock tick will return the processor to the normal
*			  operating state.
*
* Input		: None
*
* Output	: None
*
* Note		: A slight performance improvement is possible if this function is
*		      implemented as a macro, or if the code here is simply pasted into
*			  the 'dispatch' function. However, by making this a function call,
*			  it becomes easier - during development - to assess the performance
*			  of the scheduler, using the 'performance analyser'. May wish to
*			  disable this if using a watchdog.
*
*------------------------------------------------------------------------------
*/
void SCH_GoToSleep(void)
{
	//PCON |= 0x01;    // Enter idle mode (generic 8051 version)

   	// Entering idle mode requires TWO consecutive instructions
   	// on 80c515 / 80c505 - to avoid accidental triggering
   	//PCON |= 0x01;    // Enter idle mode (#1)
   	//PCON |= 0x20;    // Enter idle mode (#2)
}

/*
*  End of schedular.c
*/