//////////////////////////////////////////////////////////////////////////////
// prog: uarttask.c
// comm: reads the uart for incoming commands
// auth: MSC
//////////////////////////////////////////////////////////////////////////////

#include <stdio.h>  // toupper
#include <ctype.h>
#include <string.h>

#include "includes.h"
#include "main.h"
#include "taskcreate.h" // for function prototypes
 
#define CR 13 // carriage return char
#define LF 10 // linefeed char

void setQMember(PC pc, char *msg, int nr);
void setQMember(PC pc, char *msg, int nr)
{
	// retrieve command from string
	pc->value       = 0;                  				// reset last value
	pc->text[0]     = '\0';               				// reset last string
	pc->command_nr  = nr+1;               				// update command counter
	pc->device_type = toupper((unsigned char)msg[0]);   // to capital, easier to handle
	if (pc->device_type=='D')             				// if output is display, get text
		strcpy(pc->text, &msg[2]);
	else						          				// otherwise, get value
		pc->value = atoi(&msg[2]);
}


// UartReadTask; task that: 
// 1. reads incoming chars from uart to string
// 2. posts command-string to right mailbox
// 3. waits for command to be exectuted by flag
// 4. before new command can be entered
void UartReadTask (void *pdata)
{
	INT8U         error;
	OS_FLAGS      nr_of_commands;
    char          buf[80];
    INT8U         q_nr=0;
	PC            pc; // pointer to C structure

	memset(dqueue, 0, sizeof(C)*QSIZE); // clear data array

    while(TRUE) 
    { 
		UART_puts("\n\rEnter command [device(l,d,b).value/text]: ");
		UART_gets(buf, TRUE);
		
		// the next one is special, for debugging purposes only
		// if firs char is 's', then check stackdata for tasks...
		switch (toupper((unsigned char)buf[0]))
		{
		default: // oops
		    UART_puts("Unknown command ignored\r\n");
			continue;

		case 'S': // stackdata
			displayAllStackData();
			continue;

		case 'E': // execute commands in queue
			UART_puts("Please wait: commands are being executed...");
  		    // send to distrib: go exec queue!
		    OSFlagPost(FlgHndl[FLAGEXECQ], 1, OS_FLAG_SET, &error);
		    // wait until execution finished
			nr_of_commands = OSFlagPend(FlgHndl[FLAGDIST],
		               0xff,
			           OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME,
					   WAIT_FOREVER,
					   &error);
		    UART_puts("Commands executed: "); UART_putint(nr_of_commands);
			UART_puts(" done, continue.\r\n");

			// reset dataqueue-stuff
			q_nr = 0; 							// reset counter
			memset(dqueue, 0, sizeof(C)*QSIZE); // clear data-area
			break;

		case 'L': // led-command
		case 'D': // display-command
		case 'B': // buzzer-command
		    pc = &dqueue[q_nr];             // determine right pos in array
		    setQMember(pc, buf, q_nr++);	// fill data array and incr pos
	        OSQPost(QHndl, (void *)pc);     // post pointer to queue
    		// now, wait for acknowledge of command in q
			break;
		}
	}
}
