/*Adaptado a LCD 20x4
 * LCD_Module.c
 *
 * Created: 3/29/2025 6:14:40 PM
 *  Author: Luis Felipe Holchor Virgen
 */ 
#include "LCD_Module.h"

void LCD_Init()
{
	DDRB |= 0x3F; 
	DDRD |= 0x0C;
	DDRC |= (1<<RS_LCD) | (1 << EN_LCD);
	
	//Comandos Iniciales al encender EL LCD
	_delay_ms(15);
	LCD_Command(0x30);
	_delay_ms(4.1);
	LCD_Command(0x30);
	_delay_us(100);
	LCD_Command(0x30);
	
	//Comandos para configurar EL LCD
	LCD_Command(LCD_INTERFACE_DATA1);
	LCD_Command(LCD_DISPLAY_ON_CURSOR_OF);
	LCD_Command(LCD_CLEAR);
}

void LCD_Command(unsigned char command)
{
	PORTB &= ~(0x3F);
	PORTB |= (command & 0x3F);
	PORTD &= ~(0x0C);
	PORTD |= ((command >> 4) & 0x0C);
	PORTC &= ~(1 << RS_LCD); //Colocamos RS_LCD en 0
	PORTC &= ~(1 << EN_LCD); //EN_LCD en 0
	_delay_us(10);
	PORTC |= (1 << EN_LCD); //EN_LCD en 1
	_delay_us(100);
}

void LCD_Write_Char(unsigned char character)
{
	PORTC |= (1 << RS_LCD);
	PORTB &= ~(0x3F);
	PORTB |= (character & 0x3F);
	PORTD &= ~(0x0C);
	PORTD |= ((character >> 4) & 0x0C);
	PORTC &= ~(1 << EN_LCD); //EN_LCD en 0
	_delay_us(10);
	PORTC |= (1 << EN_LCD); //EN_LCD en 1
	_delay_us(100);
}

void LCD_Write_String(unsigned char *str)
{
	while(*str)
	{
		LCD_Write_Char(*str++);
	}
}
void LCD_SetCursor(char col, char row)
 {
	char address;
	switch (row) {
		case 0: address = 0x00; break;  // Primera fila
		case 1: address = 0x40; break;  // Segunda fila
		case 2: address = 0x14; break;  // Tercera fila 
		case 3: address = 0x54; break;  // Cuarta fila 
		default: address = 0x00;
	}
	LCD_Command(LCD_CHANGE_LINE | (address + col));  // Comando "Set DDRAM Address"
}