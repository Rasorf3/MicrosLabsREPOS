/*
 * Lab1.c
 *
 * Created: 3/29/2025 5:31:26 PM
 * Author : Luis Felipe Holchor Virgen
 */ 
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "LCD_Module.h"
#include "UltraSonicModule.h"
#include "RTC_Module.h"
#include "Timer1.h"
#include "ROLL_Module.h"

int main(void)
{
	float distancia_basura = 0;
	unsigned char Datos_LCD[15];
	DDRC |= 0x30;
  
	LCD_Init();
	UltraSonicInit();
	twi_init();
	Timer1_Init();
	DHT22_init();
    while (1) 
    {
		

		RTC_display_data();
		
		if(ReadRollPin())
		{
			LCD_Command(LCD_CLEAR);
			LCD_SetCursor(0,0);
			LCD_Write_String(" -------------------");
			LCD_SetCursor(0,1);
			LCD_Write_String("****Bote Abierto****");
			LCD_SetCursor(0,2);
			LCD_Write_String("**Esperando Cierre***");
			LCD_SetCursor(0,3);
			LCD_Write_String(" -------------------");
			_delay_ms(2000);
		}
		else
		{
			UltraSonic_Display_Data();
			DHT_Display_Data();
		}
		
		_delay_ms(2000); // Espera 2 segundos entre lecturas (DHT22 necesita al menos 2s)
	
		

		
    }
}

