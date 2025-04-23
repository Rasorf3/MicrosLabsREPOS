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
//#include "Timer1.h"
#include "ROLL_Module.h"
//#include "Timer2.h"
//#include "Timer0.h"
#include "USART_Module.h"

#define ONE_SECOND 500000UL
int main(void)
{
	DDRC |= 0x30; //I2C PORTS
	DDRD |= (1 << 4);
	/*twi_init();
	
	
	Timer1_Init();
	
	
	Timer2_Init();
	Timer0_Init();*/
	DHT22_init();
	UltraSonicInit();
	LCD_Init();
	Roll_Init();
	USART_Init();
	//RTC_updateTime("19:54:00");
	//RTC_updateDate("10/04/2025-5");
	//sendStringUSART("AT+NAME=TRASH BIN 1\r\n");
	while (1)
	{
		LCD_Command(LCD_CLEAR);
		LCD_SetCursor(0,0);
		LCD_Write_String("--------------------------------");
		LCD_SetCursor(0,1);
		LCD_Write_String("AT+NAME?\r\n");
		_delay_ms(6000);
		sendStringUSART("AT+NAME=TRASH BIN 1\r\n");
		sendStringUSART("AT+NAME?\r\n");
		UART_receive_string();
		LCD_Command(LCD_CLEAR);
		LCD_SetCursor(0,0);
		LCD_Write_String("--------------------------------");
		LCD_SetCursor(0,1);
		LCD_Write_String(buffer);
		_delay_ms(2000);
		if(!ReadRollPin())
		{
			sendStringUSART("AT+NAME?\r\n");
			UART_receive_string();
			LCD_Command(LCD_CLEAR);
			LCD_SetCursor(0,0);
			LCD_Write_String("--------------------------------");
			LCD_SetCursor(0,1);
			LCD_Write_String(buffer);
			_delay_ms(2000);
			/*sendStringUSART(buffer);
			sendStringUSART("AT+PSWD?\r\n");
			UART_receive_string();
			sendStringUSART(buffer);
			sendStringUSART("AT+UART?\r\n");
			UART_receive_string();
			sendStringUSART(buffer);*/
		}
		else
		{
			LCD_Command(LCD_CLEAR);
			LCD_SetCursor(0,0);
			LCD_Write_String("--------------------------------");
			LCD_SetCursor(0,1);
			LCD_Write_String("hola");
			_delay_ms(2000);
		}
		/*unsigned char flag_check = 0;
		unsigned char counter = 0;
		float AverageUltraSonic  = 0;
		unsigned int AverageTemp = 0;
		unsigned int AverageHum= 0;
		unsigned char buffer[20];
		unsigned char result = 0;
		
		Timer1_reset();*/
		/*while(flag_check == 0)
		{
			RTC_display_data();
			if(Timer1_getTime() >= ONE_SECOND)
			{
				counter++;
				AverageUltraSonic = UltraSonic_AvarageData(counter);
				result = DHT22_read();
				AverageTemp = DHT_Average_Temp(counter);
				AverageHum = DHT_Average_Hum(counter);
				Timer1_reset();
				sprintf(buffer,"temp: %d",AverageTemp);
				sendStringUSART(buffer);
			}
			if(counter >= 10)
			{
				UltraSonic_Display_Data(AverageUltraSonic);
				DHT_Display_Data((unsigned char)AverageTemp,(unsigned char)AverageHum);
				flag_check = 1;
			}
			if(getUSARTdata() == 'e')
			{
				PORTD |= (1 << 4);
			}
			if(getUSARTdata() == 'a')
			{
				PORTD &= ~(1 << 4);
			}
			_delay_ms(100);
		}*/
	
		
			



		
	}
}

