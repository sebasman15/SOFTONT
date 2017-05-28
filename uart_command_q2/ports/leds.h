/*
Author: 	W Pielage & E Helmond
Date:		2-10-2013
Revision:	2

    leds.h:
           LED-driver testboard

    pin-info:
    	   PB1	- LED_RCK
           PB8  - LED_SCK
           PA15 - LED_SER
*/


#define LED_SER  	GPIO_Pin_15
#define	P_LED_SER	GPIOA
#define	LED_SCK		GPIO_Pin_8
#define	P_LED_SCK	GPIOB
#define	LED_RCK	 	GPIO_Pin_1
#define	P_LED_RCK	GPIOB

void LED_init(void);
void LED_put(unsigned char led_byte);


