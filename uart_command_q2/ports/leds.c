/*
    ARM IP interface

    leds.c:
           LED-driver testboard

    pin-info:
           PA16 - RCK: Storage register clock
           PA8  - SCK: Shift register clock
           PA15 - SER: Data input

    opmerkingen:
                -pin 4 wordt door de SPI, de toetsen en de leds gebruikt. Pin 6
                 wordt door de SPI en de leds gebruikt.

*/
#include "includes.h"

// Stelt gebruikte I/O pinnen in.
void LED_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef gpio;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_Mode = GPIO_Mode_OUT;

	gpio.GPIO_Pin = LED_SER;
	GPIO_Init(P_LED_SER, &gpio);
	gpio.GPIO_Pin = LED_SCK;
	GPIO_Init(P_LED_SCK, &gpio);
	gpio.GPIO_Pin = LED_RCK;
	GPIO_Init(P_LED_RCK, &gpio);

	GPIO_ResetBits(P_LED_SER, LED_SER);
    GPIO_ResetBits(P_LED_SCK, LED_SCK);
    GPIO_SetBits(P_LED_RCK, LED_RCK);

    LED_put(0); // all leds off
}

// Zet de meegegeven waarde op de LED's
void LED_put(unsigned char led_byte)
{
     unsigned short i;

     // Data uitklokken (wordt in buffer van schuifregister geplaatst)
     for(i=128; i>=1; i>>=1)
     {
          if(led_byte & i)
        	  GPIO_SetBits(P_LED_SER, LED_SER);
          else
        	  GPIO_ResetBits(P_LED_SER, LED_SER);
          GPIO_SetBits(P_LED_SCK, LED_SCK);
          GPIO_ResetBits(P_LED_SCK, LED_SCK);
     }
     GPIO_SetBits(P_LED_RCK, LED_RCK);
     GPIO_ResetBits(P_LED_RCK, LED_RCK);
}
