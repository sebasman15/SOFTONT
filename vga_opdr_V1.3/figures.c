#include "stm32_ub_vga_screen.h"

typedef enum direction{Up,Down,Right,Left};
typedef enum fill{Nofill,Fill};



void line(uint16_t start_x_pos, uint16_t start_y_pos, uint16_t length, uint16_t direction, uint16_t color)
{

int x;
int y;

	if (direction == Up)
	{
		UB_VGA_SetPixel(start_x_pos,start_y_pos,color);
		for (y = start_y_pos-length; y < start_y_pos; ++y)
		{
			UB_VGA_SetPixel(start_x_pos,y,color);
		}
	}
	if (direction == Down)
	{
		UB_VGA_SetPixel(start_x_pos,start_y_pos,color);
		for (y = start_y_pos; y < start_y_pos+length; ++y)
		{
			UB_VGA_SetPixel(start_x_pos,y,color);
		}
	}
	if (direction == Left)
	{
		UB_VGA_SetPixel(start_x_pos,start_y_pos,color);
		for (x = start_x_pos-length; x < start_x_pos; ++x)
		{
			UB_VGA_SetPixel(x,start_y_pos,color);
		}
	}
	if (direction == Right)
	{
		UB_VGA_SetPixel(start_x_pos,start_y_pos,color);
		for (x = start_x_pos; x < start_x_pos+length; ++x)
		{
			UB_VGA_SetPixel(x,start_y_pos,color);
		}
	}
}


void square(uint16_t start_x_pos,uint16_t start_y_pos, uint16_t length, uint16_t width, uint16_t color, uint16_t thickness_lines, uint16_t fill)
{

int i; // x_teller
int j;	// y_teller

length = length+ start_y_pos;
width = width+start_x_pos;

	if (fill == Nofill)
	{
		for(j = start_y_pos; j < length; ++j)
			{
				for (i = start_x_pos; i < width; ++i)
				{
					if (i>width-thickness_lines||i<start_x_pos-1+thickness_lines||j>length-thickness_lines||j<start_y_pos-1+thickness_lines)
					{
						UB_VGA_SetPixel(i,j,color);
					}
				}
			}


	}

	if (fill == Fill)
	{
		for(j = start_y_pos; j < length; ++j)
			{
				for (i = start_x_pos; i < width; ++i)
				{
					UB_VGA_SetPixel(i,j,color);
				}
		}
	}
}

// morgen opnieuw beginnen
void elipse (uint16_t start_x_pos, uint16_t start_y_pos, uint16_t height, uint16_t width, uint16_t line_thickness, uint16_t color, uint16_t fill)
{
	// ten opzichte van het midden bereken je de rechter boog door de constante te berekenen aan de hand van de breedte op dat moment kan de proportie voor de gradenhoek vanaf het midden worden berekent. met die proportie kan je de schuine zijde berekenen en daarmee de hoogte. hoogte is het punt dat geplaatst moet worden met setpixel.
//	int left_max_elipse;
//	int right_max_elipse;
//	int up_max_elipse;
//	int down_max_elipse;
	int half_width = 0.5* width;
	int half_height = 0.5*height;
	float triangle_const_elipse = 2*(sqrtf((half_height*half_height)+(half_width*half_width)));
	int half_triangle_const_elipse = triangle_const_elipse*0.5;
	float degrees;
	float diagonal;
	int i; // x_teller
	int j; // y_teller
	int h;
	float y;
	float negy;


//	up_max_elipse= start_y_pos + half_height;
//	down_max_elipse = start_y_pos - half_height;
//	right_max_elipse = start_x_pos + half_width;
//	left_max_elipse = start_x_pos - half_width;

	if (fill== Nofill)
	{
		for (j = 0; j < line_thickness; ++j)
		{
			half_width=half_width-j;
			half_height=half_height-j;




				for (i = 0; i < half_width; ++i)
				{
					degrees = 45*(i/half_width);

					y = tanf(degrees)*i;
					negy = start_y_pos - y;
					//y = start_y_pos +(sqrt(((half_triangle_const_elipse*half_triangle_const_elipse)-(half_width*half_width))));
					//negy = start_y_pos -(sqrt(((half_triangle_const_elipse*half_triangle_const_elipse)-(half_width*half_width))));

					UB_VGA_SetPixel(start_x_pos+half_width-i,y,color); // rechtsboven t.o.v.  midden
					UB_VGA_SetPixel(start_x_pos-half_width+i,y,color); // linksboven t.o.v. midden
					UB_VGA_SetPixel(start_x_pos+half_width-i,negy,color); // rechtsonder t.o.v. midden
					UB_VGA_SetPixel(start_x_pos-half_width+i,negy,color); // linksonder t.o.v. midden
				}

		}
	}
	if (fill== Fill)
	{
		for (j = 0; j < half_width; ++j)
		{


			for (i = 0; i < half_width; ++i)
			{
				half_width= half_width-i;
				int y = start_y_pos +(sqrt(((half_triangle_const_elipse*half_triangle_const_elipse)-(half_width*half_width))));
				int negy = start_y_pos -(sqrt(((half_triangle_const_elipse*half_triangle_const_elipse)-(half_width*half_width))));

				UB_VGA_SetPixel(start_x_pos+half_width-i,y,color); // rechtsboven t.o.v.  midden
				UB_VGA_SetPixel(start_x_pos-half_width+i,y,color); // linksboven t.o.v. midden
				UB_VGA_SetPixel(start_x_pos+half_width-i,negy,color); // rechtsonder t.o.v. midden
				UB_VGA_SetPixel(start_x_pos-half_width+i,negy,color); // linksonder t.o.v. midden
		}
	}
	}
}

// werkt alleen met verhouding van 0.5
void triangle(uint16_t start_x_pos, uint16_t start_y_pos, uint16_t height, uint16_t width, uint16_t color, uint16_t fill, uint16_t thickness_line, uint16_t direction, uint16_t position)
{
int i; // y_pos_counter
int j; // x_pos_counter
int k; // line thickness counter


float proportion = height/width;

if (fill ==  Fill)
{

if (direction == Right && position == Up)
{
	for (i = 0; i < height; ++i)
	{
		for (j = 0; j < width; ++j)
		{
			if(j<i*proportion)
			{
			UB_VGA_SetPixel(j,i,color);
			}
		}
	}
}

if (direction == Right && position == Down)
{

}

if (direction == Left && position == Up)
{
	for (i = 0; i < height; ++i)
	{
		for (j = 0; j < width; ++j)
		{
			if(j<i*proportion)
			{
			UB_VGA_SetPixel(height-j,i,color);
			}
		}
	}
}

if (direction == Left && position == Down)
{
	for (i = 0; i < height; ++i)
	{
		for (j = 0; j < width; ++j)
		{
			if(j<i*proportion)
			{
			UB_VGA_SetPixel(width-j,height-i,color);
			}
		}
	}
}
}

if ( fill == Nofill)
{
	if (direction == Right && position == Up)
	{
		for (i = 0; i < height; ++i)
		{
			for (j = 0; j < width; ++j)
			{
				if(j+1< thickness_line||i<thickness_line)
				UB_VGA_SetPixel(j,i,color);

				for (k = 0; k < thickness_line; ++k)
				{
				if(j == i*proportion+2-k)// de 2 pixel shift zorgt voor een mooiere aansluiting op de hoeken
				UB_VGA_SetPixel(j,height-i,color);
				}

			}
		}
	}

	if (direction == Right && position == Down)
	{
		for (i = 0; i < height; ++i)
		{
			for (j = 0; j < width; ++j)
			{
				if(j+1< thickness_line||i>height-thickness_line)
				UB_VGA_SetPixel(j,i,color);

				for (k = 0; k < thickness_line; ++k)
				{
					if(j == i*proportion+2-k)// de 2 pixel shift zorgt voor een mooiere aansluiting op de hoeken
					{
					UB_VGA_SetPixel(j,i,color);
					}
				}
			}
		}

	}


	if (direction == Left && position == Up)
	{
		for (i = 0; i < height; ++i)
		{
			for (j = 0; j < width; ++j)
			{
				if(j>width-thickness_line||i+1<thickness_line)
				{
					UB_VGA_SetPixel(j,i,color);
				}
				for (k = 0; k < thickness_line; ++k)
				{
					if(j == i*proportion-k)
					{
					UB_VGA_SetPixel(j,i,color);
					}
				}


			}
		}
	}

	if (direction == Left && position == Down)
	{
		for (i = 0; i < height; ++i)
		{
			for (j = 0; j < width; ++j)
			{
				for (j = 0; j < width; ++j)
				{
					if(j+1> width-thickness_line||i>height-thickness_line)
					UB_VGA_SetPixel(j,i,color);

					for (k = 0; k < thickness_line; ++k)
					{
					if(j == i*proportion-k)// de 2 pixel shift zorgt voor een mooiere aansluiting op de hoeken
					UB_VGA_SetPixel(j,height-i,color);
					}

				}
			}
		 }
	 }
}


}
