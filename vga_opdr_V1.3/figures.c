#include "stm32_ub_vga_screen.h"


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

//	// horizontale lijnen linkssboven naar rechtsonder
 	maxy = start_y_pos-end_y_pos;
 	maxx = start_x_pos-end_x_pos;
	fverhouding_xy= maxx/maxy;
	fverhouding_yx= maxy/maxx;



	for (i = 0; i < VGA_DISPLAY_Y; ++i)
	{
		for (j = 0; j < VGA_DISPLAY_X; ++j)
		{
			if (j>start_x_pos && j< end_x_pos && i > start_y_pos && i < end_y_pos)
			{
					UB_VGA_SetPixel(j,j*fverhouding_yx+start_y_pos, color);
			}
		}
	}
//
//	// horizontale lijnen linksonder naar rechtsboven
	maxy = end_y_pos-start_y_pos;
	maxx = start_x_pos-end_x_pos;
	fverhouding_xy= maxx/maxy;
	fverhouding_yx= maxy/maxx;

	for (i = 0; i < VGA_DISPLAY_Y; ++i)
	{
		for (j = 0; j < VGA_DISPLAY_X; ++j)
		{
			if (j>start_x_pos && j< end_x_pos && i < start_y_pos && i > end_y_pos)
			{
					UB_VGA_SetPixel(j,start_y_pos-j*fverhouding_yx, color);
			}
		}
	}

//	// verticale lijnen rechts boven naar links onder
	 	maxy = start_y_pos-end_y_pos;
	 	maxx = end_x_pos-start_x_pos;
		fverhouding_xy= maxx/maxy;
		fverhouding_yx= maxy/maxx;
		verhouding_xy= fverhouding_xy;


	for (i = 0; i < VGA_DISPLAY_Y; ++i)
	{
		for (j = 0; j < VGA_DISPLAY_X; ++j)
		{
			if (j<start_x_pos && j > end_x_pos && i > start_y_pos && i < end_y_pos)
			{
					UB_VGA_SetPixel(start_x_pos-i*fverhouding_xy,i, color);
			}
		}
	}




//	// verticale lijnen links boven naar rechts onder
	 	maxy = start_y_pos-end_y_pos;
	 	maxx = start_x_pos-end_x_pos;
		fverhouding_xy= maxx/maxy;
		fverhouding_yx= maxy/maxx;
		verhouding_xy= fverhouding_xy;


	for (i = 0; i < VGA_DISPLAY_Y; ++i)
	{
		for (j = 0; j < VGA_DISPLAY_X; ++j)
		{
			if (j>start_x_pos && j< end_x_pos && i > start_y_pos && i < end_y_pos)
			{
					UB_VGA_SetPixel(i*fverhouding_xy+start_x_pos,i, color);
			}
		}
	}
	// rechte lijnen verticaal
			for (i = 0; i < VGA_DISPLAY_Y; ++i)
			{
				for (j = 0; j < VGA_DISPLAY_X; ++j)
				{
					if (start_y_pos==end_y_pos && j> start_x_pos && j<end_x_pos)
						UB_VGA_SetPixel(j,start_y_pos,color);
				}
			}
				//rechte lijnen horizontaal
			for (i = 0; i < VGA_DISPLAY_Y; ++i)
			{
				for (j = 0; j < VGA_DISPLAY_X; ++j)
					{
							if (start_x_pos==end_x_pos&& i> start_y_pos && i<end_y_pos)
							UB_VGA_SetPixel(start_x_pos,i,color);
					}
		}

}








void square(int start_x_pos,int start_y_pos, int end_x_pos , int end_y_pos, int color, int thickness_lines, int fill)
{

int length = end_y_pos - start_y_pos;
int width = end_x_pos - start_x_pos;

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
		thickness_lines = 1;
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
void elipse (int start_x_pos, int start_y_pos, int radius_x, int radius_y , int color, int line_thickness, int fill)
{
// ten opzichte van het midden bereken je de rechter boog door de constante te berekenen aan de hand van de breedte op dat moment kan de proportie voor de gradenhoek vanaf het midden worden berekent. met die proportie kan je de schuine zijde berekenen en daarmee de hoogte. hoogte is het punt dat geplaatst moet worden met setpixel.
//	float triangle_const_elipse = 2*(sqrtf((half_height*half_height)+(half_width*half_width)));
//	int half_triangle_const_elipse = triangle_const_elipse*0.5;
	float verhouding_xy=radius_x/radius_y;
	float verhouding_yx=radius_y/radius_x;
//	float diagonal;
	int i; // y_teller
	int j; // x_teller
	int h;
//	float y;
//	float negy;

	UB_VGA_SetPixel(start_x_pos-radius_x,start_y_pos,color);
	UB_VGA_SetPixel(start_x_pos+radius_x,start_y_pos,color);
	UB_VGA_SetPixel(start_x_pos,start_y_pos-radius_y,color);
	UB_VGA_SetPixel(start_x_pos,start_y_pos+radius_y,color);

	for (i = 0; i < VGA_DISPLAY_Y; ++i)
	{
		for (j = 0; j < VGA_DISPLAY_X; ++j)
		{
			if (j> start_x_pos-radius_x && j< start_x_pos+radius_x && i> start_y_pos-radius_y && i< start_y_pos+radius_y)
			{
				if (j<start_x_pos && i<start_y_pos)
				{
					UB_VGA_SetPixel(j,j*verhouding_yx,color);
				}
				if (j>start_x_pos && i<start_y_pos)
				{
					UB_VGA_SetPixel(j,j*verhouding_yx,color);
				}
				if (j>start_x_pos && i>start_y_pos)
				{
					UB_VGA_SetPixel(j,j*verhouding_yx,color);
				}
				if (j<start_x_pos && i>start_y_pos)
				{
					UB_VGA_SetPixel(j,j*verhouding_yx,color);
				}
			}
		}

	}



//	up_max_elipse= start_y_pos + half_height;
//	down_max_elipse = start_y_pos - half_height;
//	right_max_elipse = start_x_pos + half_width;
//	left_max_elipse = start_x_pos - half_width;

//	if (fill== Nofill)
//	{
//		for (j = 0; j < line_thickness; ++j)
//		{
//			half_width=half_width-j;
//			half_height=half_height-j;
//
//
//
//
//				for (i = 0; i < half_width; ++i)
//				{
//					degrees = 45*(i/half_width);
//
//					y = tanf(degrees)*i;
//					negy = start_y_pos - y;
//					//y = start_y_pos +(sqrt(((half_triangle_const_elipse*half_triangle_const_elipse)-(half_width*half_width))));
//					//negy = start_y_pos -(sqrt(((half_triangle_const_elipse*half_triangle_const_elipse)-(half_width*half_width))));
//
//					UB_VGA_SetPixel(start_x_pos+half_width-i,y,color); // rechtsboven t.o.v.  midden
//					UB_VGA_SetPixel(start_x_pos-half_width+i,y,color); // linksboven t.o.v. midden
//					UB_VGA_SetPixel(start_x_pos+half_width-i,negy,color); // rechtsonder t.o.v. midden
//					UB_VGA_SetPixel(start_x_pos-half_width+i,negy,color); // linksonder t.o.v. midden
//				}
//
//		}
//	}
//	if (fill== Fill)
//	{
//		for (j = 0; j < half_width; ++j)
//		{
//
//
//			for (i = 0; i < half_width; ++i)
//			{
//				half_width= half_width-i;
//				int y = start_y_pos +(sqrt(((half_triangle_const_elipse*half_triangle_const_elipse)-(half_width*half_width))));
//				int negy = start_y_pos -(sqrt(((half_triangle_const_elipse*half_triangle_const_elipse)-(half_width*half_width))));
//
//				UB_VGA_SetPixel(start_x_pos+half_width-i,y,color); // rechtsboven t.o.v.  midden
//				UB_VGA_SetPixel(start_x_pos-half_width+i,y,color); // linksboven t.o.v. midden
//				UB_VGA_SetPixel(start_x_pos+half_width-i,negy,color); // rechtsonder t.o.v. midden
//				UB_VGA_SetPixel(start_x_pos-half_width+i,negy,color); // linksonder t.o.v. midden
//		}
//	}
//	}
}

// werkt alleen met verhouding van 0.5
void triangle(int start_x_pos, int start_y_pos, int x_pos2, int y_pos2 , int end_x_pos, int end_y_pos,  int color,int line_thickness, int fill)
{

	line(start_x_pos,start_y_pos,x_pos2,y_pos2,color);
	line(x_pos2,y_pos2,end_x_pos,end_y_pos,color);
	line(start_x_pos,start_y_pos,end_x_pos,end_y_pos,color);

}
