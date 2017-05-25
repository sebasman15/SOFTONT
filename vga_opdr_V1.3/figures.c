#include "stm32_ub_vga_screen.h"

typedef enum direction{Up,Down,Right,Left};
typedef enum fill{Nofill,Fill};



void line(int start_x_pos, int start_y_pos, int end_x_pos, int end_y_pos, uint16_t color)
{

	float fverhouding_xy;
	float fverhouding_yx;
	int verhouding_xy;
	int verhouding_yx;
	int i; // y_teller
	int j; // x_teller
	int x_pos;
	float y_pos;
	float maxy;
	float maxx;
	int select;
	//	verhouding_xy= fverhouding_xy;
	//	verhouding_yx=fverhouding_yx;

	UB_VGA_SetPixel(start_x_pos,start_y_pos,VGA_COL_RED);
	UB_VGA_SetPixel(end_x_pos,end_y_pos,VGA_COL_BLUE);


	// horizontale lijnen linksonder naar rechtsboven
//	maxy = end_y_pos-start_y_pos;
//	maxx = start_x_pos-end_x_pos;
//	fverhouding_xy= maxx/maxy;
//	fverhouding_yx= maxy/maxx;
//
//	for (i = 0; i < VGA_DISPLAY_Y; ++i)
//	{
//		for (j = 0; j < VGA_DISPLAY_X; ++j)
//		{
//			if (j>start_x_pos && j< end_x_pos && i < start_y_pos && i > end_y_pos)
//			{
//					UB_VGA_SetPixel(j,240-(j*fverhouding_yx+end_y_pos-start_x_pos), color);
//			}
//		}
//	}


	// horizontale lijnen linkssboven naar rechtsonder
// 	maxy = start_y_pos-end_y_pos;
// 	maxx = start_x_pos-end_x_pos;
//	fverhouding_xy= maxx/maxy;
//	fverhouding_yx= maxy/maxx;
//
//
//
//	for (i = 0; i < VGA_DISPLAY_Y; ++i)
//	{
//		for (j = 0; j < VGA_DISPLAY_X; ++j)
//		{
//			if (j>start_x_pos && j< end_x_pos && i > start_y_pos && i < end_y_pos)
//			{
//					UB_VGA_SetPixel(j,j*fverhouding_yx+start_y_pos-start_x_pos, color);
//			}
//		}
//	}

//	// verticale lijnen links boven naar rechts onder
//	 	maxy = start_y_pos-end_y_pos;
//	 	maxx = start_x_pos-end_x_pos;
//		fverhouding_xy= maxx/maxy;
//		fverhouding_yx= maxy/maxx;
//
//
//	for (i = 0; i < VGA_DISPLAY_Y; ++i)
//	{
//		for (j = 0; j < VGA_DISPLAY_X; ++j)
//		{
//			if (j>start_x_pos && j< end_x_pos && i > start_y_pos && i < end_y_pos)
//			{
//					UB_VGA_SetPixel(i*fverhouding_xy-start_y_pos+start_x_pos,i, color);
//			}
//		}
//	}

	// verticale lijnen rechts boven naar links onder
	 	maxy = start_y_pos-end_y_pos;
	 	maxx = end_x_pos-start_x_pos;
		fverhouding_xy= maxx/maxy;
		fverhouding_yx= maxy/maxx;


	for (i = 0; i < VGA_DISPLAY_Y; ++i)
	{
		for (j = 0; j < VGA_DISPLAY_X; ++j)
		{
			if (j<start_x_pos && j > end_x_pos && i > start_y_pos && i < end_y_pos)
			{
					UB_VGA_SetPixel(340-((i-end_x_pos)*fverhouding_xy+start_y_pos),i, color);
			}
		}
	}

//	if (start_y_pos > end_y_pos)
//		{

//			select = 1;
//		}
//		if (start_y_pos<end_y_pos)
//		{
//			maxy = start_y_pos-end_y_pos;
//			maxx = start_x_pos-end_x_pos;
//			fverhouding_xy= maxx/maxy;
//			fverhouding_yx= maxy/maxx;
//			select =0;
//		}
//		else if (start_y_pos==end_y_pos)
//		{
//			for (i = 0; i < VGA_DISPLAY_Y; ++i)
//			{
//				for (j = 0; j < VGA_DISPLAY_X; ++j)
//				{
//					if (j>start_x_pos && j< end_x_pos && i > start_y_pos && i < end_y_pos)
//						UB_VGA_SetPixel(j,start_y_pos,color);
//				}
//			}
//		}
//
//		else if (start_x_pos==end_x_pos)
//		{
//			for (i = 0; i < VGA_DISPLAY_Y; ++i)
//			{
//				for (j = 0; j < VGA_DISPLAY_X; ++j)
//				{
//					if (j>start_x_pos && j< end_x_pos && i > start_y_pos && i < end_y_pos)
//						UB_VGA_SetPixel(start_x_pos,i,color);
//				}
//			}
//		}
//
//
//
//			for (i = 0; i < VGA_DISPLAY_Y; ++i)
//			{
//				for (j = 0; j < VGA_DISPLAY_X; ++j)
//				{
//					if (j>start_x_pos && j< end_x_pos && i > start_y_pos && i < end_y_pos)
//					{
//						if (select == 1)
//						UB_VGA_SetPixel(j,j*fverhouding_yx+end_y_pos, color);
//						if (select == 0)
//							UB_VGA_SetPixel(j,j*fverhouding_yx+end_y_pos-start_x_pos, color);
//
//					}
//				}
//			}
//		}


//


//	 if (start_y_pos<end_y_pos)
//	{
//		maxy = end_y_pos-start_y_pos;
//		maxx = end_x_pos-start_x_pos;
//		fverhouding_xy= maxx/maxy;
//		fverhouding_yx= maxy/maxx;



//	}

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


int proportion = abs(height)/abs(width);
//int	  proportion = abs(proportion);

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
