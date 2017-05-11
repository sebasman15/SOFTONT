#include "stm32_ub_vga_screen.h"

#define Fill 1
#define Nofill 0

#define Up 0
#define Down 1
#define Right 2
#define Left 3



void line(uint16_t start_x_pos, uint16_t start_y_pos, uint16_t length, uint16_t direction, uint16_t color);
void square(uint16_t start_x_pos,uint16_t start_y_pos, uint16_t length, uint16_t width, uint16_t color, uint16_t thickness_lines, uint16_t fill);
void elipse (uint16_t start_x_pos, uint16_t start_y_pos, uint16_t height, uint16_t width,uint16_t line_thickness, uint16_t color, uint16_t fill);
void triangle(uint16_t start_x_pos, uint16_t start_y_pos, uint16_t height, uint16_t width, uint16_t color, uint16_t fill,uint16_t thickness_line, uint16_t direction, uint16_t position);
