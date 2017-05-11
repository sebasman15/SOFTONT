//--------------------------------------------------------------
// File     : main.c
// Datum    : 30.03.2016
// Version  : 1.0
// Autor    : UB
// mods by	: J.F. van der Bent
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.x
// Module   : CMSIS_BOOT, M4_CMSIS_CORE
// Function : VGA_core DMA LIB 320x240, 8bit color
//--------------------------------------------------------------

#include "main.h"
#include "stm32_ub_vga_screen.h"
#include <math.h>
#include <figures.h>
#include <read_write_pixel.h>




//void comic_sans (int kleur)
//{
//#define geen_kleur VGA_COL_BLACK
//	int a[5][5] = {{geen_kleur, geen_kleur, kleur, geen_kleur, geen_kleur},
//					{geen_kleur,kleur,geen_kleur, kleur,geen_kleur},
//					{kleur,kleur,kleur,kleur,kleur},
//					{kleur,geen_kleur,geen_kleur,geen_kleur,kleur},
//					{kleur,geen_kleur,geen_kleur,geen_kleur,kleur}};
//}

int main(void)
{

	//  uint32_t n;

	SystemInit(); // System speed to 168MHz

	UB_VGA_Screen_Init(); // Init VGA-Screen

  UB_VGA_FillScreen(VGA_COL_BLACK);
  //UB_VGA_SetPixel(10,10,10);




  while(1)
  {

	  //square(40,40,40,20,VGA_COL_YELLOW,2,Nofill);
	  //line(100,100,10,Right,VGA_COL_YELLOW);
	  //elipse(100,100,40,40,1,VGA_COL_YELLOW,Nofill);
	  //UB_VGA_SetPixel(100,100,VGA_COL_YELLOW);
	  //triangle(0,0,100,100,VGA_COL_YELLOW,Nofill,2,Left,Down);
	  read_and_write(30,40,VGA_COL_YELLOW,write);
  }
}

