/*
Author: 	W Pielage & E Helmond
	  :		M Scager J.F. van der Bent
Date:		13-6-2014
Revision:	2

    uart.c:
          UART2 driver for ARM-board V5

    pin-info:
           PA2 - TX
           PA3 - RX

To enable UART use the following line:
	UART_init();
To use UART on interrupt base use:
	UART_INT_init();

This file initialize the UART on the ARM-board v5.
To send data to the UART use:
	UART_printf(*string);

To read the UART without interrupt use:
	char = USART2->DR & 0xFF;

In the interrupt routine the char is send back to the terminal
07-07-2014 Aanpassen Uart routine om compatibel te zijn met void UART_put(char *c)

*/

/* Includes ------------------------------------------------------------------*/
#include "includes.h"


char string[100];
int charcounter = 0;

void UART_init(void)
{

  /* --------------------------- System Clocks Configuration -----------------*/
  /* USART2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

  /* GPIOA clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;

  /*-------------------------- GPIO Configuration ----------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Connect USART pins to AF */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);   // USART2_TX
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);  // USART2_RX

  USART_InitTypeDef USART_InitStructure;

/* USARTx configuration ------------------------------------------------------*/
/* USARTx configured as follow:
      - BaudRate = 115200 baud
      - Word Length = 8 Bits
      - One Stop Bit
      - No parity
      - Hardware flow control disabled (RTS and CTS signals)
      - Receive and transmit enabled
*/
USART_InitStructure.USART_BaudRate = 115200;
USART_InitStructure.USART_WordLength = USART_WordLength_8b;
USART_InitStructure.USART_StopBits = USART_StopBits_1;
USART_InitStructure.USART_Parity = USART_Parity_No;
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

USART_Init(USART2, &USART_InitStructure);
USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);

USART_Cmd(USART2, ENABLE);

}

void UART_putchar(char c)
{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); // Wait for Empty
		USART_SendData(USART2, c);

}

void UART_puts(char *s)
{
	volatile unsigned int i;
	for (i=0; s[i]; i++)
	{
		UART_putchar(s[i]);
		//while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); // Wait for Empty
		//USART_SendData(USART2, s[i]);
	}
}


void UART_INT_init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void USART2_IRQHandler(void)
{
	// check if the USART2 receive interrupt flag was set
	if( USART_GetITStatus(USART2, USART_IT_RXNE))
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		int i;
		char c = USART2->DR & 0xFF;
//		LCD_XY(10,1);
//		LCD_put("UART:   ");
//		LCD_XY(15,1);
//		LCD_putchar(c);

		if(c > 64 && c < 91)
			c += 32;
		if(c <= 13)
		{
			for(i=0;i<charcounter;i++)
			{
				USART_SendData(USART2, string[i]); // Echo Char
			}
			charcounter = 0;
			USART_SendData(USART2, c); // Echo Char
		}
		else
		{
			string[charcounter] = c;
			charcounter++;
		}


		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); // Wait for Empty

	}
}

signed int UART_printf(size_t length, const char *pFormat, ...)
{
    va_list    ap;
    signed int rc;
    char c[length];
    char *pString = c;

    va_start(ap, pFormat);
    rc = vsnprintf(pString, length, pFormat, ap);
    va_end(ap);

    __asm( "CPSID   I");
    volatile unsigned char i;
	for (i=0; pString[i]; i++)
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); // Wait for Empty
		USART_SendData(USART2, pString[i]);
	}
	__asm( "CPSIE   I");

    return rc;
}
// Stuurt meegegeven getal uit op de UART

void UART_putint(unsigned int num)
{
    UART_putnum(num, 10);
}

// Stuurt meegegeven getal uit op de UART in het aangegeven getallenstelsel
void UART_putnum(unsigned int num, unsigned char deel)
{
    static unsigned char chars[16] = "0123456789ABCDEF";
    unsigned int rest;
    signed char c[16];
    signed int i=15;

    // Zet de integer om naar een string
    if(num==0)
    {
        c[i]='0';
        i--;
    }
    else
    {
        while(num>0)
        {
            rest=num%deel;
            num/=deel;
            c[i]=chars[rest];
            i--;

            if(i==0) // it ends here
                num=0;
        }
    }


    // Stuur de string uit
    while(i<15)
    {
        i++;
        // Wacht tot de buffer leeg is
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); // Wait for Empty
        USART_SendData(USART2, c[i]);

    }
}

// Ontvang een karakter via de UART
// niet echt nodig als routine maar als wrapper voor compatabiliteit. Let op geen -1 als er geen char is ontvangen!

char UART_get(void)
{
    char uart_char = -1;
    if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE)== SET)  // check for data available
    	 uart_char= USART2->DR & 0xFF; // and read the data from peripheral
    return uart_char;
}


// void UART_gets
// args: char *readbuffer
//       int   echo, when TRUE, send read-char to UART
// remark: ARM sends -1 if buffer is empty
//         LF is cleared if set in terminal-program
void UART_gets(char *s, int echo)
{
	while (TRUE)
	{
	 	*s = UART_get();

	 	if (*s==-1)             // check for data available
	 		continue;

	 	if (*s==0xff || *s==LF) // if no data or LF, continue
			continue;

		if (echo)              // if output-flag set
			UART_putchar(*s);  // to read what u entered

		if (*s==CR)            // if enter pressed
		{
			*s = '\0';         // ignore char and close string
		    return;            // buf ready, exit loop
		}
		s++;
	}
}
