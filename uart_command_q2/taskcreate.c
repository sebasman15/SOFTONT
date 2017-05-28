//////////////////////////////////////////////////////////////////////////////
// prog: taskcreate.c
// comm: task creation and stuff
// auth: MSC
//////////////////////////////////////////////////////////////////////////////

#include "includes.h"
#include "taskcreate.h"
#include "main.h"

void displayStackData(int);


// Create different stacks for the tasks
// give each thread/task/process its own stack with size
// stacks are allocated here statically, because malloc() is not supported
OS_STK UartReadTaskStk [STACK_SIZE];
OS_STK DistribTaskStk  [STACK_SIZE];
OS_STK BuzzerTaskStk   [STACK_SIZE];
OS_STK LedsTaskStk     [STACK_SIZE];
OS_STK DisplayTaskStk  [STACK_SIZE];

// allocation of array of structures
// note: the last structure is set to zero's, not necessary, but it simplifies
// 1. looping thru the array (until null), 2. copy-pasting with the last comma
TASKDATA tasks[] =
{
// name           stack             priority            stacksize   option
// -------------------------------------------------------------------------------------
{   UartReadTask,  UartReadTaskStk,  UARTREAD_TASK_PRTY, STACK_SIZE, OS_TASK_OPT_STK_CHK  },
{   DistribTask,   DistribTaskStk,   DISTRIB_TASK_PRTY,  STACK_SIZE, OS_TASK_OPT_STK_CHK  },
{   BuzzerTask,    BuzzerTaskStk,    BUZZER_TASK_PRTY,   STACK_SIZE, OS_TASK_OPT_STK_CHK  },
{   LedsTask,      LedsTaskStk,      LEDS_TASK_PRTY,     STACK_SIZE, OS_TASK_OPT_STK_CHK  },
{   DisplayTask,   DisplayTaskStk,   DISPLAY_TASK_PRTY,  STACK_SIZE, OS_TASK_OPT_STK_CHK  },
{   NULL,          NULL,             0,                  0,          0                    }
};

//////////////////////////////////////////////////////////////////////////////
// func: createTasks
// args: none
// comm: all tasks are created in a loop, using the TASKDATA array
// note: the stacks are allocated statically; dynamic allocation would be an
//       improvement, but malloc() is not supported.
//////////////////////////////////////////////////////////////////////////////
void CreateTasks()
{
	// using a pointer to a structure: pointer is incremented
	PTASKDATA ptd = tasks; // tasks == &tasks[0]: both are addresses of first struct
	for (; ptd->taskname != NULL; ptd++)
	{
		OSTaskCreateExt(
			ptd->taskname,	 					// taskname
			NULL, 								// not used
			ptd->stack + ptd->stacksize-1, 		// top of stack
			ptd->priority,						// priority
            ptd->priority, 						// not used
			ptd->stack, 						// bottom of stack
			ptd->stacksize, 					// size of stack
			NULL, 								// not used
			ptd->option);						// enable stackchecking
	}
	// or, by using an integer to get the right structure
	// this version is simpler to understand, but less elegant
	/*
	//int       nr_of_tasks = tasks/sizeof(TASKDATA) -1; // nice, but not used
	int i;
	for (i=0; tasks[i].taskname != NULL; i++)
	{
		OSTaskCreateExt(
			tasks[i].taskname, 					// taskname
			NULL, 								// not used
			&tasks[i].stack[tasks[i].stacksize-1], 	// top of stack
			tasks[i].priority,					// priority
            tasks[i].priority, 					// not used
			tasks[i].stack, 					// bottom of stack
			tasks[i].stacksize, 				// size of stack
			NULL, 								// not used
			tasks[i].option);					// enable stackchecking
	}
	*/
}


//////////////////////////////////////////////////////////////////////////////
// func: displayStackData
// args: priority (== process-id)
// comm: functions retrieves and displays StackData-information
//       the os-call is optional and set in the STACKDATA array
//////////////////////////////////////////////////////////////////////////////
void displayStackData(int priority)
{
    OS_STK_DATA stackdata; // allocate structure
	INT8U       error;

	if ((error = OSTaskStkChk(priority, &stackdata))) // get data onto this struct
	    return;

    UART_puts("\n\rStackData for process "); UART_putint(priority);
    UART_puts(", Usage=");     UART_putint((int)stackdata.OSUsed/4);
    UART_puts(" - Free=");	 UART_putint((int)stackdata.OSFree/4);
    UART_puts("\n\r");
}


void displayAllStackData()
{
	PTASKDATA ptd = tasks; // tasks == &tasks[0]: both are addresses of first struct
	for (; ptd->taskname != NULL; ptd++)
		displayStackData(ptd->priority);
}
