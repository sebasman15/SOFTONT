/*
    ARM IP interface

    lcd.h:
          LCD-driver testboard

*/

#define eenregel		// define voor 1x16 display

#ifndef LCD_H
#define LCD_H

/* I/O-pinnen */
#define  STARTBIT       0
#define  LCD_D7         GPIOE, GPIO_Pin_6
#define  LCD_D6         GPIOE, GPIO_Pin_5
#define  LCD_D5         GPIOE, GPIO_Pin_4
#define  LCD_D4         GPIOE, GPIO_Pin_3
#define  LCD_EN         GPIOD, GPIO_Pin_0
#define  LCD_RS         GPIOC, GPIO_Pin_13
#define  LCD_RW         GPIOC, GPIO_Pin_11

#define  LCD_DATA       (LCD_D4|LCD_D5|LCD_D6|LCD_D7)
#define  LCD_IOALL      (LCD_D4|LCD_D5|LCD_D6|LCD_D7|LCD_EN|LCD_RS)

#define LCD_display_on()     LCD_writecontrol(0x0E)
#define LCD_display_off()    LCD_writecontrol(0x08)

#define LCD_cursor_blink()   LCD_writecontrol(0x0F)
#define LCD_cursor_on()      LCD_writecontrol(0x0E)
#define LCD_cursor_off()     LCD_writecontrol(0x0C)

#define LCD_cursor_left()    LCD_writecontrol(0x10)
#define LCD_cursor_right()   LCD_writecontrol(0x14)
#define LCD_display_sleft()  LCD_writecontrol(0x18)
#define LCD_display_sright() LCD_writecontrol(0x1C)

void LCD_init(void);
void LCD_clear(void);
void LCD_cursor_home(void);
void LCD_writecontrol(unsigned char data);
void LCD_put(char *c);
void LCD_puts(char *c);
void LCD_putchar(char c);
void LCD_putint(unsigned int num);
void delay_LCD(unsigned int time);
void LCD_XY(unsigned int x, unsigned int y);
void busyflag(void);

#endif /*LCD_H*/
