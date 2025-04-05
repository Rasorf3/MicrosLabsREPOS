/*
 * Lab1.c
 *
 * Created: 3/29/2025 5:31:26 PM
 * Author : Luis Felipe Holchor Virgen
 */ 
#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "LCD_Module.h"
#include "UltraSonicModule.h"
#include "RTC_Module.h"

int main(void)
{
	float distancia_basura = 0;
	unsigned char Datos_LCD[15];
	DDRC |= 0x30;
    /* Replace with your application code */
	LCD_Init();
	UltraSonicInit();
	twi_init();
    while (1) 
    {
		LCD_Command(LCD_CLEAR);
		LCD_SetCursor(0,0);
		distancia_basura = GetDistance();
		dtostrf(distancia_basura, 5, 2, Datos_LCD);
		LCD_Write_String(Datos_LCD);
		LCD_Write_String(" cm");
		_delay_ms(2000);
		LCD_Command(LCD_CLEAR);
		_delay_ms(1);
		LCD_Command(LCD_CLEAR);
		RTC_displayDate();
		_delay_ms(2000);
		
		for(int i; i < 10; i++)
		{
			LCD_Command(LCD_CLEAR);
			RTC_displayTime();
			_delay_ms(100);
		}
		
		

		
    }
}

