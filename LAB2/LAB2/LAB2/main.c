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
#include <avr/interrupt.h>

#include "LCD_Module.h"
#include "UltraSonicModule.h"
#include "RTC_Module.h"
//#include "Timer1.h"
#include "ROLL_Module.h"
#include "Timer2.h"
#include "Timer0.h"
#include "USART_Module.h"

#define TIME_CONSTANT_MS 200
int main(void)
{
	DDRC |= 0x30; //I2C PORTS
	DDRD |= (1 << 4);
	/*twi_init();
	Timer1_Init();
	*/
	DHT22_init();
	UltraSonicInit();
	LCD_Init();
	Roll_Init();
	USART_Init();
	Timer0_Init();
	Timer2_Init();
	//RTC_updateTime("19:54:00");
	//RTC_updateDate("10/04/2025-5");
	
	txFlag = 0;
	rxFlag = 0;
	indexBuffer = 0;
	sei();
	unsigned char flag_check = 0;
	unsigned char counterTime = 0;
	unsigned char counter = 0;
	float AverageUltraSonicData = 0;
	float UltraSonicData[10];
	unsigned int AverageTemp = 0;
	unsigned int AverageHum= 0;
	unsigned char buffer[20];
	unsigned char result = 0;
	Timer2_reset();
	Timer2_Stop();
	
	LCD_Command(LCD_CLEAR);
	LCD_SetCursor(0,0);
	LCD_Write_String("---------------------------------------");
	LCD_SetCursor(0,1);
	LCD_Write_String("      WAIT FOR     ");
	LCD_SetCursor(0,2);
	LCD_Write_String("        DATA        ");
	LCD_SetCursor(0,3);
	LCD_Write_String("--------------------");
	
	Timer0_reset();
	Timer0_Stop();
	while (1)
	{
		
		if(Timer0_milis(TIME_CONSTANT_MS))
		{
			Timer0_reset();
			counterTime++;
			
		}
		if(counterTime == 3)
		{
			counterTime = 0;
			counter++;
			SendTrigger();
			
		}
		if(counter >= 10)
		{
			counter = 0;
			
			/////////////////////////////////////////////////////////////////
			//Bloque Que calcula El promedio del Ultrasonico
			for(uint8_t i = 0; i < 10; i++)
			{
				AverageUltraSonicData+= UltraSonicData[i];
			}
			AverageUltraSonicData = AverageUltraSonicData / 10;
			UltraSonic_Display_Data(AverageUltraSonicData);
			/////////////////////////////////////////////////////////////////
			
			/////////////////////////////////////////////////////////////////
			//Bloque Para Calcular DHT
			/////////////////////////////////////////////////////////////////
		}
		if(current_state == STATE_READY)
		{
			distance_cm = (float)measure_pulse_width() / 58.0f;
			UltraSonicData[counter-1] = distance_cm;
			current_state = STATE_IDLE;
		}
	}
}

