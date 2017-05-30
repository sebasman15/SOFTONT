#include "stm32_ub_vga_screen.h"
#include <tekst.h>
#include "georgia_16pts.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"
#include "stdio.h"
#include "stdarg.h"

typedef enum font_setting{GEORGIA_NORMAL,GEORGIA_ITALIC,GEORGIA_BOLD};
int Descriptor[];
int Bitmap[];


int determine_char(char letter)
{
	// 'Case statement' die bepaald welk nummer ('!' = 0, '~' = 93)
	// LET OP: excl. SPATIE.

	int letternummer = letter;
	letternummer = letternummer-33; //asci ! begint
	return letternummer;
}

int calc_writing_space(int letter,int font)
{
	// Bepaalt de schrijfruimte voor een karakter (in bytes).
	// LET OP: Spatie nog niet meegenomen, neem 2 bytes

	int space;
	int val;

	switch (font) {
	case GEORGIA_NORMAL:
		val = georgia_16pt_normal_Descriptors[letter*2];
		break;
	case GEORGIA_ITALIC:
		val = georgia_16pt_italic_Descriptors[letter*2];
		break;
	case GEORGIA_BOLD:
		val = georgia_16pt_bold_Descriptors[letter*2];
		break;
	default:
		break;
	}

	if (val >= 0 && val <= 8)
		space = 1;

	if (val > 8 && val <= 16)
		space = 2;

	if (val > 16 && val <= 24)
		space = 3;

	if (val > 24 && val <= 32) // Dit komt volgens mij niet voor
		space = 4;

	return space;
}

int calc_char_space(int letter, int font)
{
	int val;
	switch (font) {
	case GEORGIA_NORMAL:
		val = georgia_16pt_normal_Descriptors[letter*2];
		break;
	case GEORGIA_ITALIC:
		val = georgia_16pt_italic_Descriptors[letter*2];
		break;
	case GEORGIA_BOLD:
		val = georgia_16pt_bold_Descriptors[letter*2];
		break;
	default:
		break;
	}
	return val;
}

int determine_offset(int letter,int font)
{
	int val;
	switch (font) {
	case GEORGIA_NORMAL:
		val = georgia_16pt_normal_Descriptors[letter*2+1];
		break;
	case GEORGIA_ITALIC:
		val = georgia_16pt_italic_Descriptors[letter*2+1];
		break;
	case GEORGIA_BOLD:
		val = georgia_16pt_bold_Descriptors[letter*2+1];
		break;
	default:
		break;
	}
//	int val = georgia_16pt_italic_Descriptors[letter*2+1]; // alleen de tweede waarde
	return val;
}


void write_char_VGA(int x_pos, int y_pos, int col, int write_space, int font, int start_offset)
{
	int val_old;
	int val_new;
	int xt;
	int yt;
	int bt;
	int comp;
	int macht;
	int i=0;

	for(yt=0;yt<21;yt++)
	{
		for (xt = 0; xt < write_space ; xt++)
		{
			comp = 0;
			macht = 1;
			switch (font) {
			case GEORGIA_NORMAL:
				val_old = georgia_16pt_normal_Bitmaps[i+start_offset];
				break;
			case GEORGIA_ITALIC:
				val_old = georgia_16pt_italic_Bitmaps[i+start_offset];
				break;
			case GEORGIA_BOLD:
				val_old = georgia_16pt_bold_Bitmaps[i+start_offset];
				break;
			default:
				break;
			}
//			val_old = georgia_16pt_italic_Bitmaps[i+start_offset];

			for (bt=1;bt<=8;bt++)
			{
				comp = 0x80/macht; // vergelijkingsbit
				val_new = val_old - comp; // Bits vergelijken

				if((val_new)>=0)
				{
					UB_VGA_SetPixel((xt*8)+bt+x_pos, yt+y_pos, col+bt+xt+yt);
					val_old = val_new;		// alleen nieuwe als bit er in zat
				}
				macht = macht * 2; // 00 - 80
			}
			i++; // volgende in array
		}
	}
}

void fill_descriptors_bitmap(int font)
{
//	int i;
//	int j;
////	for(i=0;i<4000;i++)
////	{
////	i=sizeof(Bitmap);
//		switch(font){
//		case GEORGIA_NORMAL:
//			for(i=0;i<94;i++)
//			{
//				Descriptor[i] = georgia_16pt_normal_Descriptors[i];
//			}
//			for(j=0;j<3338;j++)
//			{
//				Bitmap[j] = georgia_16pt_normal_Bitmaps[j];
//			}
////			Descriptor[i] = georgia_16pt_normal_Descriptors[i];
////			Bitmap[i] = georgia_16pt_normal_Bitmaps[i];
//			if (Descriptor[50]==georgia_16pt_normal_Descriptors[50])
//				UB_VGA_SetPixel(100,100,VGA_COL_GREEN);
//			else
//				UB_VGA_SetPixel(100,100,VGA_COL_RED);
//			if (Bitmap[1]==georgia_16pt_normal_Bitmaps[1])
//				UB_VGA_SetPixel(101,100,VGA_COL_GREEN);
//			else
//				UB_VGA_SetPixel(101,100,VGA_COL_RED);
//			break;
//		case GEORGIA_ITALIC:
////			Descriptor[i] = georgia_16pt_italic_Descriptors[i];
////			Bitmap[i] = georgia_16pt_italic_Bitmaps[i];
////			strcpy(Descriptor,georgia_16pt_italic_Descriptors);
////			strcpy(Bitmap,georgia_16pt_italic_Bitmaps);
//			break;
//		case GEORGIA_BOLD:
////			Descriptor[i] = georgia_16pt_bold_Descriptors[i];
////			Bitmap[i] = georgia_16pt_bold_Bitmaps[i];
//			break;
//		}
////	}
}

void char_write(int x_pos, int y_pos, int font_setting, int col, char letter)
{
//	fill_descriptors_bitmap(font_setting);
	int char_number = determine_char(letter); // gets number value of character
	int write_space = calc_writing_space(char_number,font_setting); // gets value of writing space in bytes
	int char_space = calc_char_space(char_number,font_setting); // gets value of real space
	int start_offset = determine_offset(char_number,font_setting);

	write_char_VGA(x_pos, y_pos, col, write_space, font_setting, start_offset); // Daadwerkelijke functie
//	empty_descriptors_bitmap();
}

void str_write(int x_pos, int y_pos,int font_setting, int col, char *s)
{
//	fill_descriptors_bitmap(font_setting);
	int new_pos = 0;
	int k;
	int letter_afstand = 1;
	int regel_afstand = 22;
	int VGA_breedte = 310;

	for(k=0;s[k];k++)
	{
		int char_number = determine_char(s[k]); // gets number value of character
		int write_space = calc_writing_space(char_number,font_setting); // gets value of writing space in bytes
		int char_space = calc_char_space(char_number,font_setting); // gets value of real space
		int start_offset = determine_offset(char_number,font_setting);


		if (s[k] != ' ')
			write_char_VGA(x_pos+new_pos, y_pos, col, write_space, font_setting, start_offset); // Daadwerkelijke functie
		else
			char_space = 6;

		new_pos = new_pos + char_space + letter_afstand;

		if (new_pos + x_pos > VGA_breedte)
		{
			new_pos = 0;
			y_pos = y_pos + regel_afstand;
		}
	}
}



//void letter_g_write(int x_pos, int y_pos, int col)
//{
//	int val_old;
//	int val_new;
//	int xt;
//	int yt;
//	int bt;
//	int comp;
//	int macht;
//	int i=0;
//
//	for(yt=0;yt<21;yt++)
//	{
//		for (xt=0;xt<2;xt++)
//		{
//			comp = 0;
//			macht = 1;
//			val_old = letter_g[i];
//
//			for (bt=1;bt<=8;bt++)
//			{
//				comp = 0x80/macht; // vergelijkingsbit
//				val_new = val_old - comp; // Bits vergelijken
//
//				if((val_new)>=0)
//				{
//					UB_VGA_SetPixel((xt*8)+bt+x_pos, yt+y_pos, col);
//					val_old = val_new;		// alleen nieuwe als bit er in zat
//				}
//				macht = macht * 2; // 00 - 80
//			}
//			i++; // volgende in array
//		}
//	}
//}

