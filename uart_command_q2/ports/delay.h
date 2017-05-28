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

/****************Libraries******************************/
/* Libraries needed for ADC are (These are in main.h):
 * #include "stm32f4xx.h"
 * #include "stm32f4xx_conf.h"
 * #include "stm32f4xx_syscfg.h"
 */

/****************Defines********************************/

/****************Function Prototypes********************/
void DELAY_init(void);
void DELAY_us(unsigned int x);
void DELAY_ms(unsigned int x);
void DELAY_s(unsigned int x);
