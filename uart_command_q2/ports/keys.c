/*
Author: 	J.F. van der Bent
Date:		7-7-2014
Revision:	3

    keys.c:
          Keyboard-driver for SpARM-board v2

    pin-info:
           PD1  - R2
           PD2  - R3
           PD3  - R4
           PD6  - K1
           PD7  - K2
           PD8  - K3
           PD9  - K4
           PD11 - R1

	Key B1 = 1  ----- key B16 = 16



To use the keyboard first initialize the ports:
	KEYS_init();
After that, you can use polling to read a key with:
	KEYS_read();
*/

#include "includes.h"

void KEYS_init(void)
/* Keys initialize
 * In this function the ports are set for the keyboard.
 * The rows are outputs and the columns are input.
 */
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef gpio;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;


	gpio.GPIO_Pin = KEY_ROW;
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(PKEY_ROW, &gpio);

	gpio.GPIO_Pin = KEY_COL;
	gpio.GPIO_Mode = GPIO_Mode_IN;
	gpio.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(PKEY_COL, &gpio);
	GPIO_ResetBits(PKEY_ROW, KEY_ROW);
}

unsigned int KEYS_read(void)
/* Keys read
 * This function reads which key is pressed.
 * It does so by making a row high and check if there is a connection with a column.
 * If there is a connection the value is returned.
 */
{
	unsigned int key = 0;
	unsigned out = 0;

	GPIO_ResetBits(PKEY_ROW, KEY_R1);
	GPIO_SetBits(PKEY_ROW, KEY_R4);
	key = KEYS_kolom();
	if( key != 0 ) out= 12+key;

	GPIO_ResetBits(PKEY_ROW, KEY_R4);
	GPIO_SetBits(PKEY_ROW, KEY_R3);
	key = KEYS_kolom();
	if( key != 0 ) out = 8+key;

	GPIO_ResetBits(PKEY_ROW, KEY_R3);
	GPIO_SetBits(PKEY_ROW, KEY_R2);
	key = KEYS_kolom();
	if( key != 0 ) out = 4+key;

	GPIO_ResetBits(PKEY_ROW, KEY_R2);
	GPIO_SetBits(PKEY_ROW, KEY_R1);
	key = KEYS_kolom();
	if( key != 0 ) out =key;

	return out;
}

unsigned int KEYS_kolom(void)
/* Keys Column
 * In this function the columns are checked if there is a connection.
 * If a column has a connection the return value is the column number.
 */
{
	unsigned int key = 0;
	if (GPIO_ReadInputDataBit(PKEY_COL, KEY_K1) == (uint8_t)Bit_SET) key = 1;
	if (GPIO_ReadInputDataBit(PKEY_COL, KEY_K2) == (uint8_t)Bit_SET) key = 2;
	if (GPIO_ReadInputDataBit(PKEY_COL, KEY_K3) == (uint8_t)Bit_SET) key = 3;
	if (GPIO_ReadInputDataBit(PKEY_COL, KEY_K4) == (uint8_t)Bit_SET) key = 4;
	return key;
}
