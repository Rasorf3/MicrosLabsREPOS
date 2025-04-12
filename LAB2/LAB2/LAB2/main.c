/*
 * LAB2.c
 *
 * Created: 4/12/2025 11:38:04 AM
 * Author : Luis Felipe Holchor Virgen
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include "LCD_Module.h"
#include "UltraSonicModule.h"
#include "RTC_Module.h"
#include "Timer1.h"
#include "ROLL_Module.h"
#include "Timer2.h"
#include "Timer0.h"

#define ONE_SECOND 1000000UL
int main(void)
{
	DDRC |= 0x30; //I2C PORTS
	twi_init();
	LCD_Init();
	UltraSonicInit();
	Timer1_Init();
	DHT22_init();
	Roll_Init();
	Timer2_Init();
	Timer0_Init();
	//RTC_updateTime("19:54:00");
	//RTC_updateDate("10/04/2025-5");
	_delay_ms(100);
	
	while (1)
	{
		unsigned char flag_check = 0;
		unsigned char counter = 0;
		float AverageUltraSonic  = 0;
		unsigned char buffer[20];
		
		
		Timer1_reset();
		while(flag_check == 0)
		{
			RTC_display_data();
			if(Timer1_getTime() >= ONE_SECOND)
			{
				counter++;
				AverageUltraSonic = UltraSonic_AvarageData(counter);
				Timer1_reset();
			}
			if(counter >= 10)
			{
				UltraSonic_Display_Data(AverageUltraSonic);
				flag_check = 1;
			}
			LCD_SetCursor(0,2);
			sprintf(buffer,"time: %ul",Timer1_getTime());
			LCD_Write_String(buffer);
			_delay_ms(100);
		}
		//RTC_display_data();
		/*if(!ReadRollPin())
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
			//DHT_Display_Data();
			_delay_ms(100);
		}*/
		
	}
}

