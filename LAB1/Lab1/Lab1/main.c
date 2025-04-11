/*
 * Lab1.c
 *
 * Created: 3/29/2025 5:31:26 PM
 * Author : Luis Felipe Holchor Virgen
 */ 
#define F_CPU 16000000UL

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
	DDRC |= 0x30; //I2C PORTS
	twi_init();
	LCD_Init();
	UltraSonicInit();
	Timer1_Init();
	DHT22_init();
	Roll_Init();
	//RTC_updateTime("19:54:00");
	//RTC_updateDate("10/04/2025-5");
	_delay_ms(100);
	  
    while (1) 
    {
	
	    RTC_display_data();
		if(!ReadRollPin())
		{
			LCD_Command(LCD_CLEAR);
			LCD_SetCursor(0,0);
			LCD_Write_String("---------------------------------------");
			LCD_SetCursor(0,1);
			LCD_Write_String("****Bote Abierto****");
			LCD_SetCursor(0,2);
			LCD_Write_String("**Esperando Cierre***");
			LCD_SetCursor(0,3);
			LCD_Write_String("--------------------");
			_delay_ms(2000);
		}
		else
		{
		UltraSonic_Display_Data();
		DHT_Display_Data();
		_delay_ms(2000);
		}
	
    }
}

