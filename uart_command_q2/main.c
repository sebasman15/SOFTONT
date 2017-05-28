/******************************************************************************
    ARM IP interface - uC/OS-II
    main.c: this code demonstrates the usage of some uC/osII-API-functions on an 
            ARM-board:
            - mutual exclusion semapores
            - mailboxing
            Next to that it demonstrates some c-features:
            - enums
            - arrays of structs
            - pointerhandling
    remarks:
    author: MSC        
******************************************************************************/

//#include <lcd.h>
//#include <buzzer.h>

#include "includes.h"
#include "taskcreate.h"
#include "main.h"

// handles, used for q, mailboxes and flags
// defined as a pointer; the os will allocate an OS_EVENT on it
OS_EVENT           *QHndl;
OS_EVENT           *MbHndl[4];
OS_FLAG_GRP        *FlgHndl[2];
OS_STK              InitTaskStk[STACK_SIZE];

C     dqueue[QSIZE]; // data-array
void *pqueue[QSIZE]; // queue array, holds pointers to data-array
char *version = "UART_COMMAND_Q2";

char *functionality =
"UART_COMMAND_Q2 is een queue-versie van UART_COMMAND. Het \
demonstreert queueing, mailboxing, flagging, hoe je makkelijk tasks \
creeert (taskcreate.c) en de bidirectionele omgang met de UART-comport \
(uarttask.c). Om te beginnen typt de gebruiker commando's in \
via een terminal-programma, bv. terminal.exe.\r \
Uarttask:\r\
1. leest inkomende characters van de uart. Dat is een commando, dat de gebruiker \
via een terminal-programma intoetst, bv l.255, dat alle leds aanzet.\r\
2. dit commando wordt gelezen, in een struct gezet en naar de queue 'gestuurd'.\r\
3. na 'e' wordt een go-flag gestuurd naar \r\
DistribTask:\r\
4. dat de commando's uit de queue een voor een uitvoert via mailboxing, \
door het commando naar de juiste output (led, display of buzzer) te sturen. \
Elk outputje heeft zijn eigen task die op een mailbox wacht.\r\
5. als een commando uitgevoerd is stuurt de betreffende output-task een flag \
terug om te laten weten dat het commando uitgevoerd is. \
6. als alle commando's uitgevoerd zijn wordt de queue opgeschoond en een \
flag naar UartTask gestuurd, zodat nieuwe commando's ingetikt kunnen worden.\r\
Michiel Scager\n\r";


// put some debug-output to uart
// put some debug-output to uart
void DisplayOSData(void)
{
    UART_puts("uC/OS-II versie ");
    UART_putint(OSVersion());
    UART_puts(" voor de CORTEX M4\n\r");
    UART_puts("processor op ");
    UART_putint(SystemCoreClock/1000000);
    UART_puts(" MHz, met perhiperals op ");
    UART_putint(SystemCoreClock/1000000);
    UART_puts(" MHz\n\r");
    UART_puts(functionality);
}

// create all handles 
void CreateHandles()
{
	INT8U error;
	int   i;
	
	// create the queue
	QHndl = OSQCreate(&pqueue[0], QSIZE);

	// create all mailboxes
	for (i=0; i<MBOX_TOTAL; i++)
		MbHndl[i] = OSMboxCreate(NULL);

   	// create all flags
	for (i=0; i<FLAGS_TOTAL; i++)
		FlgHndl[i] = OSFlagCreate(0x00, &error);
}	
   

//////////////////////////////////////////////////////////////////////////////
// func: InitTask
// args: void *pdata, needed by os
// comm: This is the initialisation-task. It's the first task to run, and 
//       it will peform some tasks that need to be done after uC/OS-II 
//       has been started. Most important task here is starting the timer.
// note: this task deletes itself when finished     
//////////////////////////////////////////////////////////////////////////////
void InitTask(void *pdata)
{
	__asm("CPSID   I");

	char *version = "UART_COMMAND_Q2";
   	LCD_puts(version);
	UART_puts(version); UART_puts("\n\r");

	DisplayOSData();  // output to uart of some data

	CreateHandles();  // create all handles
	CreateTasks();    // start all threads/tasks

	OS_CPU_SysTickInit(840000);	// 84000 = 1kHz AHB divider staat op 2!

	__asm( "CPSIE   I");

   	OSTaskDel(OS_PRIO_SELF); // This task is no longer needed; delete it
}


int main (void)
{
	SystemInit();	// Set SystemCLK

	DELAY_init();
	UART_init();
	LCD_init();
	LED_init();
	BUZZER_init();
    KEYS_init();

    OSInit();

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIOD -> MODER |= ( 1 << 24 );
	GPIOD -> MODER |= ( 1 << 26 );
	GPIOD -> MODER |= ( 1 << 28 );
	GPIOD -> MODER |= ( 1 << 30 );

	__asm("CPSID	I");	// Interrupts disable!

    OSTaskCreate(InitTask, NULL, &InitTaskStk[STACK_SIZE-1], 5);

	OSStart();

	// Nothing comes beyond this point
    while(1);
}

