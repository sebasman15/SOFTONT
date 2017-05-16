/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                             (c) Copyright 2013; Micrium, Inc.; Weston, FL
*
*                   All rights reserved.  Protected by international copyright laws.
*                   Knowledge of the source code may not be used to write a similar
*                   product.  This file may only be used in accordance with a license
*                   and should not be redistributed in any way.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                           MASTER INCLUDES
*
*                                     ST Microelectronics STM32
*                                              with the
*
*                                           STM3240G-EVAL
*                                         Evaluation Board
*
* Filename      : includes.h
* Version       : V1.00
* Programmer(s) : van der Bent
* 				: COOcox
*
*  07-07-2014 toevoeging van false en true defines
*********************************************************************************************************
*/

#ifndef  INCLUDES_PRESENT
#define  INCLUDES_PRESENT
#define OS_MASTER_FILE
/*
*********************************************************************************************************
*                                         STANDARD LIBRARIES
*********************************************************************************************************
*/

#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <math.h>

/*
*********************************************************************************************************
*                                              LIBRARIES
*********************************************************************************************************
*/

#include  "os_cpu.h"
#include  "os_cfg.h"

/*
*********************************************************************************************************
*                                                 OS
*********************************************************************************************************
*/

#include  "ucos_ii.h"

#include "misc.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"

/*
*********************************************************************************************************
*                                            INCLUDES END
*********************************************************************************************************
*/

#include "uart.h"
//#include "lcd.h"
//#include "delay.h"
//#include "leds.h"
//#include "buzzer.h"
//#include "keys.h"

#define TRUE		1
#define FALSE		0


#endif

