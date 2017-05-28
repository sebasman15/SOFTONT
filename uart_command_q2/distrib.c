//////////////////////////////////////////////////////////////////////////////
// prog: distrib.c
// comm: receives command, fills it, and distributes it to other tasks
// auth: MSC
//////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include <stdlib.h> // atoi
#include <ctype.h>  // toupper

#include "includes.h"
#include "taskcreate.h" // for task function prototypes
#include "main.h"


void outCommand(PC);
void outCommand(PC pc)
{
	UART_puts("\n\r==============================\n\r");
	UART_puts("command structure: "); UART_putint((unsigned int)pc); UART_puts("\n\r");
	UART_puts("c.command count:   "); UART_putint(pc->command_nr); UART_puts("\n\r");
	UART_puts("c.device_type:     "); UART_putchar(pc->device_type); UART_puts("\n\r");
	UART_puts("c.value:           "); UART_putint(pc->value); UART_puts("\n\r");
	UART_puts("c.text:            "); UART_puts(pc->text); UART_puts("\n\r");
	UART_puts("==============================");
}

void outQ(void); // debug output
void outQ(void)
{
    INT8U       i;
	PC			pc;

	for (i=0; (pc=&dqueue[i]); i++)
		outCommand(pc);
}


// DistribTask: task that 
// 1. reads mbox with plain command
// 2. extracts data from buffer into structure Command
// 3. posts Command struct to right mbox-handle
void DistribTask (void *pdata)
{
    INT8U         error;
	OS_EVENT     *mbh;
	PC            pc; // pointer to C structure
    OS_Q_DATA     qd; // queue data structure

    while(TRUE) 
    {
	 	// wait for userinput to execute queue
		OSFlagPend(FlgHndl[FLAGEXECQ],
		               0xff,
			           OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME,
					   WAIT_FOREVER,
					   &error);

		OSQQuery(QHndl, &qd); // check size of queue
		while ((pc = (PC)OSQAccept(QHndl, &error))!=NULL)
		{
			switch(pc->device_type) // get the right mailboxhandle
			{
			case 'L': mbh = MbHndl[MBLEDS];    break;
			case 'D': mbh = MbHndl[MBDISPLAY]; break;
			case 'B':
			default : mbh = MbHndl[MBBUZZER];  break;
			}
			//outCommand(pc); // display data to uart
			OSTimeDly(10);
			OSMboxPost(mbh, (void*)pc); // post address of structure
		    // wait for response from output device
			OSFlagPend(FlgHndl[FLAGOUT],
	                    0xff,
					    OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME,
						WAIT_FOREVER,
						&error);
		}

		OSQFlush(QHndl); // if done, clear pqueue
		OSFlagPost(FlgHndl[FLAGDIST], qd.OSNMsgs , OS_FLAG_SET, &error);// post 'done'
	}
}
