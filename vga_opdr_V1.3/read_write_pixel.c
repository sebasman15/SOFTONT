#include <stm32_ub_vga_screen.h>

typedef enum{read,write};


void read_and_write(int start_x_pos, int start_y_pos, int color, int r_w)
{
	if (r_w == write)
	UB_VGA_SetPixel(start_x_pos,start_y_pos,color);
	if (r_w == read)
	VGA_RAM1[start_x_pos*(start_y_pos+1)];
}
