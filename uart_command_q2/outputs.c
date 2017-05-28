//////////////////////////////////////////////////////////////////////////////
// prog: outputs.c
// comm: output tasks, for every single output a single task that reads
//       data thru mbox and writes it to output device
// auth: MSC
//////////////////////////////////////////////////////////////////////////////

#include "includes.h"
#include "taskcreate.h" // for task function prototypes
#include "main.h"


// a command wants to write to leds
void LedsTask (void *pdata)
{
	INT8U    error;
	PC       pcomm;
	
	while (TRUE)
	{
		pcomm = (PC)OSMboxPend(MbHndl[MBLEDS], WAIT_FOREVER, &error);
		
		LED_put((INT8U)pcomm->value); // set leds
		OSFlagPost(FlgHndl[FLAGOUT], pcomm->command_nr, OS_FLAG_SET, &error);
	}
}

// a command wants to write to buzzer
void BuzzerTask (void *pdata)
{
	INT8U    error;
	PC       pcomm;
	
	while (TRUE)
	{
		pcomm = (PC)OSMboxPend(MbHndl[MBBUZZER], WAIT_FOREVER, &error);
		
		BUZZER_put(pcomm->value);
		OSFlagPost(FlgHndl[FLAGOUT], pcomm->command_nr, OS_FLAG_SET, &error);
	}
}

// a command wants to write to display
void DisplayTask (void *pdata)
{
	INT8U    error;
	PC       pcomm;

	while (TRUE)
	{
		pcomm = (PC)OSMboxPend(MbHndl[MBDISPLAY], WAIT_FOREVER, &error);
		
	    LCD_clear(); LCD_put(pcomm->text); LCD_cursor_home();
		OSFlagPost(FlgHndl[FLAGOUT], pcomm->command_nr, OS_FLAG_SET, &error);
	}
}
