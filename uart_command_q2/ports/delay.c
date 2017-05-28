/*
Author: 	W Pielage & E Helmond
Date:		13-9-2013
Revision:	2

    delay.c:
          delays for ARM-board v5

    pin-info:
           --

To use this delays. Initialize first. In your main add:
	DELAY_init();

This reads the systemclock and calculates 1 sec/1 ms/1 us.

After that you can use DELAY_s(number of seconds to wait);
*/

#include "includes.h"

uint32_t G_CLK;
uint32_t D_uS; // Global variable (us)
uint32_t D_mS; // Global variable (ms)
uint32_t D_S; // Global variable (s)


void DELAY_init(void)
{
	RCC_ClocksTypeDef Clocks;
	RCC_GetClocksFreq(&Clocks);
	G_CLK = Clocks.SYSCLK_Frequency;	// Read the systemclock
	D_S  = (G_CLK*1.25)/9/2;	// Number of instructions in one second
	D_mS = (G_CLK*1.25)/9000/2; // Number of instructions in one millisecond
	D_uS = (G_CLK*1.25)/9000000/2; // Number of instructions in one microsecond, largest rounding error
}

void DELAY_us(volatile unsigned int time)
{
    volatile unsigned int i;

    while(time>0)		// Run x times 1 microsecond
    {
        for(i=0;i<D_uS;i++);
        time--;
    }
}

void DELAY_ms(volatile unsigned int time)
{
    volatile unsigned int i;

    while(time>0)		// Run x times 1 millisecond
    {
        for(i=0;i<D_mS;i++);
        time--;
    }
}

void DELAY_s(volatile unsigned int time)
{
    volatile unsigned int i;

    while(time>0)		// Run x times 1 second
    {
        for(i=0;i<D_S;i++);
        time--;
    }
}
