/*
 * Lab1.c
 *
 * Created: 3/29/2025 5:31:26 PM
 * Author : Luis Felipe Holchor Virgen
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "LCD_Module.h"
#include "UltraSonicModule.h"

#define F_CPU 8000000UL


int main(void)
{
	float distancia_basura = 0;
	unsigned char Datos_LCD[15];
    /* Replace with your application code */
	LCD_Init();
	UltraSonicInit();
    while (1) 
    {
		//distancia_basura = (int)GetDistance();
		LCD_SetCursor(0,0);
		distancia_basura = GetDistance();
		dtostrf(distancia_basura, 5, 2, Datos_LCD);
		LCD_Write_String(Datos_LCD);
		LCD_Write_String(" cm");
		_delay_ms(2000);
		LCD_Command(LCD_CLEAR);
		_delay_ms(500);
		
    }
}

