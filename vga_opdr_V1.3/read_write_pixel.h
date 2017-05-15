#include <stm32_ub_vga_screen.h>

typedef enum{read,write};

void read_and_write(int start_x_pos, int start_y_pos, int color, int r_w);
