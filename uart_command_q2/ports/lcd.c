/*
Date:		13-9-2013
Revision:	3

    lcd.c:
          LCD-driver ARM-board v5

    pin-info:
           PE2  - DB4   --->> PE3
           PE4  - DB5
           PE5  - DB6
           PE6  - DB7
           PC13 - E:  start data
           PC15 - RS: register-select
           PC14 - RW: read/write

    opmerkingen:
                -Alle instructies voor het display staan achterstevoren! (maakt
                 de code wat eenvoudiger)
                -Zorg ervoor dat de delay-code ook wordt meegecompileerd, anders
                 gaat het hier mis...
                -Deze code is gebaseerd op
                 "LCD routines for OLIMEX LPC-MT-2106,16x2 Character LCD 1602K,
                  4-bit mode operation",
                 Copyright (c) 2004 senz at arm.dreamislife.com
                 15-07-2014 aanpassing voor 1x16 display define eenregel bepaald type display J.F. van der Bent
*/

#include "includes.h"

static void LCD_writenibble(unsigned char data);
static void LCD_writebyte(unsigned char data);

unsigned char curpos = 0; // remember cursorposition

void ClearBits(void)
{
	GPIO_ResetBits(LCD_D4);
	GPIO_ResetBits(LCD_D5);
	GPIO_ResetBits(LCD_D6);
	GPIO_ResetBits(LCD_D7);
}

void LCD_cursor_home(void)
{
   LCD_writecontrol(0x02); // cursur home
   curpos=0;               // reset position
}

void LCD_clear(void)
{
   LCD_writecontrol(0x01); // clearscreen
   curpos=0;               // reset position
   DELAY_ms(5);
   busyflag();

}

void LCD_XY(unsigned int x, unsigned int y)
{
	unsigned char data;

	switch(y)
	{
	case 0:  data = (1<<7)+0x00+x; break;
	case 1:  data = (1<<7)+0x40+x; break;
	case 2:  data = (1<<7)+0x14+x; break;
	default: data = (1<<7)+0x54+x; break;
	}
	LCD_writecontrol(data);
}


// init LCD
// Display wordt gebruikt in 4bits modus,2 regels, 5x7 dots font.
void LCD_init(void)
{
	GPIO_InitTypeDef gpio;	// GPIO init structure
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); // Enable Clock for PortE
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_Init(GPIOE, &gpio);

	gpio.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_13;
	GPIO_Init(GPIOC, &gpio);

	gpio.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOD, &gpio);

//	GPIOE -> MODER |= ( 1 << 4 );
//	GPIOE -> MODER |= ( 1 << 8 );
//	GPIOE -> MODER |= ( 1 << 10 );
//	GPIOE -> MODER |= ( 1 << 12 );
//	GPIOC -> MODER |= ( 1 << 26 );
//	GPIOC -> MODER |= ( 1 << 30 );
//	GPIOC -> MODER |= ( 1 << 28 );

	GPIO_ResetBits(LCD_RS);
	GPIO_ResetBits(LCD_EN);
	GPIO_ResetBits(LCD_RW);

	DELAY_ms(15);//15

    LCD_writebyte(0x30); // function-set boot
    DELAY_ms(50);
    LCD_writebyte(0x30);
    LCD_writebyte(0x30);

     // interface op 4 bits
    LCD_writebyte(0x02);
    DELAY_ms(5);

    LCD_writebyte(0x2C);  // function set
    DELAY_ms(15);
    LCD_writebyte(0x0E);  // lcd aan
    DELAY_ms(15);
    LCD_writebyte(0x01);  // clear screen
    DELAY_ms(15);
    LCD_writebyte(0x06);  // entry mode set
    DELAY_ms(15);
}

// Zet meegegeven karakter op het scherm
void LCD_putchar(char c)
{
    GPIO_SetBits(LCD_RS);
    LCD_writebyte(c);
    if (++curpos==32) // remember cursorpos
    	curpos=0;
}

// Zet meegegeven string op het scherm
void LCD_put(char *string)
{
    unsigned char k,j;
	// vaag display; teken 8 tot 31 is niet uitgevoerd op het scherm
    for (k=0; string[k]; k++)
	{

#ifdef eenregel
    	// vaag display; teken 8 tot 31 is niet uitgevoerd op het scherm
    	if(curpos==8) // at 8, goto next valid lcd-position
        for (j=0; j<32; j++) LCD_cursor_right();
#endif
        LCD_putchar(string[k]);
    }
}

void LCD_puts(char *c)
{
    LCD_clear();
    LCD_put(c);
}

// Zet het meegegeven positief getal op het scherm
void LCD_putint(unsigned int num)
{
    unsigned int rest;
    static unsigned char chars[10] = "0123456789"; // possible chars
    char s[17]; // display has 16 chars [0..15] + 1 closing null
	s[16]='\0'; // closing null set
    int i=15;   // startposition at last display char

    // kinda function itoa, integer to ascii
    if(num==0)
        s[i--]='0'; // '0' at 14
    else
	{
        while(num>0 && i)
		{
            rest = num%10;
            num /= 10;
            s[i--]=chars[rest];
        }
    }
	LCD_put(&s[i+1]); // first, set i to starting pos, undo one i-- too much
}

// Stuurt een 4-bits commando naar het display
static void LCD_writenibble(unsigned char data)
{
    ClearBits();

    if (data & 0x01)
		GPIO_SetBits(LCD_D7);

	if (data & 0x02)
		GPIO_SetBits(LCD_D6);

	if (data & 0x04)
		GPIO_SetBits(LCD_D5);

	if (data & 0x08)
		GPIO_SetBits(LCD_D4);

}

// Stuurt een 8-bits commando naar het display
static void LCD_writebyte(unsigned char data)
{
    /* hoogste 4 bits */
    GPIO_SetBits(LCD_EN);
    LCD_writenibble((data>>4)&0x0F);
    GPIO_ResetBits(LCD_EN);

    busyflag();

    /* laagste 4 bits */
    GPIO_SetBits(LCD_EN);
    LCD_writenibble(data&0x0F);
    GPIO_ResetBits(LCD_EN);

    busyflag();
}

// Stuurt een commando naar het display
void LCD_writecontrol(unsigned char data)
{
    GPIO_ResetBits(LCD_RS);
    LCD_writebyte(data);
}

//  Vertraging in milliseconden.
void delay_LCD(unsigned int time)
{
	    time *= 2000; //3360
	    while(time--)
	    	__NOP();
}

void busyflag(void)
{
	uint8_t bitstatus = 0x00;
	uint8_t bitstatus1 = Bit_SET;
	GPIO_InitTypeDef gpio;

	while(bitstatus == bitstatus1)
	{
		gpio.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
		gpio.GPIO_Mode = GPIO_Mode_IN;
		gpio.GPIO_OType = GPIO_OType_PP;
		gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
		gpio.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOE, &gpio);

		GPIO_ResetBits(LCD_RS);
		GPIO_SetBits(LCD_RW);

		DELAY_ms(1);
		GPIO_SetBits(LCD_EN);
		DELAY_ms(1);

		bitstatus = GPIO_ReadInputDataBit(LCD_D7);
		GPIO_ResetBits(LCD_EN);
		DELAY_ms(1);
	}

	gpio.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &gpio);

	DELAY_ms(1);
}
