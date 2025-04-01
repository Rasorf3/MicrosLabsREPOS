/*
 * LCD_Module.h
 *
 * Created: 3/29/2025 6:14:27 PM
 *  Author: Luis Felipe Holchor Virgen
 */ 


#ifndef LCD_MODULE_H_
#define LCD_MODULE_H_
#include <avr/io.h>
#include <util/delay.h>
//PINS LCD
#define RS_LCD 2
#define EN_LCD 3
#define LCD_DATA PORTD

//Commans for LCD
#define LCD_CLEAR 0x01
#define LCD_DISPLAY_ON_CURSOR_OF 0x0C
#define LCD_DISPLAY_ON_CURSOR_ON_BLINKING 0x0F
#define LCD_DISPLAY_ON_CURSOR_ON_NOBLINKING 0x0E
#define LCD_INTERFACE_DATA1 0x38  //DATA LENGH 8BITS, 2 LINES, 5X8 Dots
#define LCD_INTERFACE_DATA2 0x34  //DATA LENGH 8BITS, 1 LINES, 5X8 Dots
#define LCD_CHANGE_LINE 0x80

void LCD_Init();
void LCD_Command(unsigned char command);
void LCD_Write_Char(unsigned char character);
void LCD_Write_String(unsigned char *str);
void LCD_SetCursor(char col, char row);




#endif /* LCD_MODULE_H_ */