/*
Author: 	W Pielage & E Helmond
Date:		13-9-2013
Revision:	2

    buzzer.c:
          Buzzer-driver for ARM-board v5

    pin-info:
           PC8 - Buzzer

*/

#include "includes.h"

void BUZZER_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOC clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void BUZZER_set(int counter)
{
	while(counter--)
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_8);
		DELAY_us(280);
		GPIO_SetBits(GPIOC, GPIO_Pin_8);
		DELAY_us(280);
	}
}

void Buzzer_put(int len)
{
	BUZZER_set(len);
	DELAY_ms(150);
}

// Maak een piepje met meegegeven lengte
// toevoeging om compatibel te blijven met ARM v4.2
void BUZZER_put(unsigned int time)
{
    volatile unsigned int i;
    volatile unsigned int k=0;

	while (k++<100)
    {
		GPIO_ResetBits(GPIOC, GPIO_Pin_8);
		for (i=0;i<(time+k);i++);
		GPIO_SetBits(GPIOC, GPIO_Pin_8);
		for (i=0;(i<time+k);i++);
	}
}

