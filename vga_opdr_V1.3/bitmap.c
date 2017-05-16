#include "stm32_ub_vga_screen.h"
#include <bitmap.h>
//typedef enum richting{omhoog,omlaag,links,rechts};

void smiley(int x_loc, int y_loc)
{
	int yt;
	int xt;
	int i = 0;
	for(yt=0;yt<100;yt++)
	{
		for(xt=0;xt<100;xt++)
		{
			if (SMILEY[i] != 0)
			{
			  UB_VGA_SetPixel(xt+x_loc,yt+y_loc,SMILEY[i]);
			}
			i++;
		}
	}
}
void pijl(int x_loc, int y_loc, int richting)
{
	switch (richting) {
		case omhoog:
			pijl_up(x_loc, y_loc);
			break;
		case omlaag:
//			Pijl_down(x_loc, y_loc);
			break;
		case rechts:
//			pijl_right(x_loc,y_loc);
			break;
		case links:
//			pijl_left(x_loc,y_loc);
			break;
		default:
			break;
	}
}

void pijl_up(int x_loc, int y_loc, int colour)
{
	int yt;
	int xt;
	int i = 0;
	for(yt=0;yt<120;yt++)
	{
		for(xt=0;xt<65;xt++)
		{
			if (PIJL_UP[i] != 0)
			{
			  UB_VGA_SetPixel(xt+x_loc,yt+y_loc,colour);
			}
			i++;
		}
	}
}

void pijl_down(int x_loc, int y_loc, int colour)
{
	int yt;
	int xt;
	int i = 0;
	for(yt=0;yt<120;yt++)
	{
		for(xt=0;xt<65;xt++)
		{
			if (PIJL_DOWN[i] != 0)
			{
			  UB_VGA_SetPixel(xt+x_loc,yt+y_loc,colour);
			}
			i++;
		}
	}
}

void pijl_left(int x_loc, int y_loc, int colour)
{
	int yt;
	int xt;
	int i = 0;
	for(yt=0;yt<65;yt++)
	{
		for(xt=0;xt<120;xt++)
		{
			if (PIJL_LEFT[i] != 0)
			{
			  UB_VGA_SetPixel(xt+x_loc,yt+y_loc,colour);
			}
			i++;
		}
	}
}

void pijl_right(int x_loc, int y_loc, int colour)
{
	int yt;
	int xt;
	int i = 0;
	for(yt=0;yt<65;yt++)
	{
		for(xt=0;xt<120;xt++)
		{
			if (PIJL_RIGHT[i] != 0)
			{
			  UB_VGA_SetPixel(xt+x_loc,yt+y_loc,colour);
			}
			i++;
		}
	}
}

void Katja()
{
	int yt;
	int xt;
	int y_loc = 0;
	int x_loc = 0;
	int TA = 0;
	int val;
	int *p_bit;
	for(yt=0;yt<142;yt++)
	{
		for(xt=0;xt<100;xt++)
		{
			UB_VGA_SetPixel(xt+x_loc,yt+x_loc,KATJA[TA]);
			TA++;
		}
	}
}
