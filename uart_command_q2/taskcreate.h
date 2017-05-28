//////////////////////////////////////////////////////////////////////////////
// prog: taskcreate.h
// comm: external definitions and prototypes for tasks
// auth: MSC
//////////////////////////////////////////////////////////////////////////////

#define STACK_SIZE               100

// structure to simplify the creation of tasks
typedef struct TaskData
{
	void           (*taskname)(void*); // pointer to function
	OS_STK         *stack;             // stackaddress
	INT8U		    priority;          // priority = task-id
	INT32U		    stacksize;         // stacksize
	INT16U          option;            // to enable stackchecking
} TASKDATA, *PTASKDATA;	// name of struct, and pointer to struct


// task-priorities
enum 
{     
    UARTREAD_TASK_PRTY = 7,
    DISTRIB_TASK_PRTY,
    BUZZER_TASK_PRTY,
    LEDS_TASK_PRTY,
    DISPLAY_TASK_PRTY,
    DUMMY
};
 
// function prototypes of threads/tasks/processes to prevent compiler warnings
extern void UartReadTask  (void *pdata);
extern void DistribTask   (void *pdata);
extern void BuzzerTask    (void *pdata);
extern void LedsTask      (void *pdata);
extern void DisplayTask   (void *pdata);

extern void CreateTasks(void);
