/*
 * LAB3.c
 *
 * Created: 7/07/2025 10:13:09 a. m.
 * Author : rasor
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "UltraSonicModule.h"
#include "ROLL_Module.h"
#include "Timer2.h"
#include "Timer0.h"
#include "i2c_async.h"
#include "RTC_Module.h"
#include "LCD_Module.h"
#include "USART_Module.h"
#define LINE_BUF_SIZE  64
#define TIME_CONSTANT_MS 200
int main(void)
{
	DHT22_init();
	UltraSonicInit();
	LCD_Init();
	Roll_Init();
	Timer0_Init();
	Timer2_Init();
	i2c_async_init();
	USART_Init();
	sei();
	//RTC_updateTime("19:54:00");
	//RTC_updateDate("10/04/2025-5");
    char line[LINE_BUF_SIZE];
    uint8_t  idx = 0;
    uint8_t  c;
	// Ejemplo TX
	//const char msg[] = "AT+NAMETRASH BIN MICROS";

	//USART_SendBuffer((const uint8_t*)msg, sizeof(msg)-1);
	LCD_Command(LCD_CLEAR);
	unsigned char flag_check = 0;
	unsigned char counterTime = 0;
	unsigned char counter = 0;
	unsigned char RollAverage = 0;
	float AverageUltraSonicData = 0;
	float UltraSonicData[10];
	unsigned int AverageTemp = 0;
	unsigned int AverageHum= 0;
	unsigned char DHTreadCheck = 0;
	unsigned char buffer[20];
	unsigned char result = 0;
	unsigned char bufferUSART[21];
	char floatBuffer[10];
	char Hd = "";
	char Hu = "";
	char Md = "";
	char Mu = "";
	// 3) Primera lectura asíncrona del RTC
	RTC_read_async();
	Timer2_reset();
	Timer2_Stop();
	
	Timer0_Stop();
	Timer0_Start();
	Timer0_reset();

	while (1) {
		
        if (USART_ReceiveByte(&c)) 
		{
	        // Si llega CR o LF, procesamos la línea completa
	        if (c == '\r' || c == '\n')
			 {
		        if (idx > 0)
				{
			        line[idx] = '\0';
			        
			        //USART_SendBuffer((uint8_t*)line, idx);
			        //USART_SendBuffer((uint8_t*)"\r\n", 2);
			        idx = 0;
					LCD_Command(LCD_CLEAR);
					LCD_SetCursor(0,0);
					LCD_Write_String("---------------------------------------");
					LCD_SetCursor(0,1);
					LCD_Write_String("Dato: ");
								
					LCD_Write_String(line);
					LCD_SetCursor(0,3);
					LCD_Write_String("--------------------");
					_delay_ms(4000);
		       }
			}
	        else 
			{
		        
		        if (idx < LINE_BUF_SIZE-1) 
				{
			        line[idx++] = c;
		        }
		        else 
				{
			        // Overflow de buffer: reiniciamos
			        idx = 0;
		        }
	        }
		}
		else
		{
			RTC_read_async();
			_delay_ms(30);
			
			Hu= getHu();
			Hd = getHd();
			Md = getMd();
			Mu = getMu();
			
			if(Timer0_milis(TIME_CONSTANT_MS))
			{
				
				Timer0_reset();
				if(ReadRollPin())
				{
					RollAverage++;
				}
				counterTime++;
				
				
			}
			if(counterTime == 5)
			{
				if(RollAverage <= 5)
				{
					counterTime = 0;
					counter++;
					DHTreadCheck = DHT22_read();
					AverageHum = DHT_Average_Temp(counter);
					AverageTemp = DHT_Average_Hum(counter);
					SendTrigger();
					RollAverage = 0;
				}
				else
				{
					counterTime = 0;
					counter = 0;
					RollAverage = 0;
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
				//AverageUltraSonicData = 20.21;
				UltraSonic_Display_Data(AverageUltraSonicData);
				/////////////////////////////////////////////////////////////////
				
				/////////////////////////////////////////////////////////////////
				
				DHT_Display_Data(AverageTemp,AverageHum);
				

				
				dtostrf(AverageUltraSonicData, 5, 2, floatBuffer);
				sprintf(bufferUSART,"%u_%u_%s_%c_%c%c_%c%c",AverageHum,AverageTemp,floatBuffer,'0',Hd,Hu,Md,Mu);
				USART_SendBuffer(bufferUSART, sizeof(bufferUSART)-1);
			}
			if(current_state == STATE_READY)
			{
				distance_cm = (float)measure_pulse_width() / 58.0f;
				UltraSonicData[counter-1] = distance_cm;
				current_state = STATE_IDLE;
			}

			
		}
		_delay_ms(10);
	}
}