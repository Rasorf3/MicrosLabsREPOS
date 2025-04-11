/*
 * UltraSonicModule.c
 *
 * Created: 3/29/2025 8:17:20 PM
 *  Author: Luis Felipe Holchor Virgen
 */ 

#include "UltraSonicModule.h"
#include "Timer1.h"
#include "LCD_Module.h"
#include <stdint.h>
#include <stdio.h>

void UltraSonicInit()
{
	DDRC |= (1 << TRIGGER);
	DDRC &= ~(1 << ECHO);
}

float GetDistance()
{
	unsigned short int counter = 0;
	
	float distance = 0;
	PORTC |= (1 << TRIGGER);
	_delay_us(15);
	PORTC &= ~(1 << TRIGGER);

	counter = measure_pulse_width();
	// Calcular distancia (en cm)
	distance = ((float)counter *0.5 ) / 58.0f;  // Formula: (us / 58) = cm
	
	return distance;
}
unsigned int measure_pulse_width() 
{
	unsigned int start_time, end_time;
	// Esperar flanco bajo (estado inicial alto)
	while (!(PINC & (1 << ECHO)));
	
	// Guardar tiempo de inicio
	TCNT1 = 0;              // Reiniciar contador
	start_time = TCNT1;     // Leer timer (casi cero)
		
	while (PINC & (1 << ECHO)); //PIND & (1 << ECHO)
	
	// Guardar tiempo de finalizaci?n
	end_time = TCNT1;
	// Calcular ancho del pulso
	return end_time - start_time;
}
void UltraSonic_Display_Data()
{
	float distancia_basura = 0;
	float FillingPercentage = 0;
	unsigned char buffer[20];
	
	distancia_basura = GetDistance();
	 
	LCD_Command(LCD_CLEAR);
	LCD_SetCursor(0,0);
	LCD_Write_String("---------------------------------------");
	dtostrf(distancia_basura, 5, 2, buffer);
	LCD_SetCursor(0,1);
	LCD_Write_String(" Distance: ");
	LCD_Write_String(buffer);
	LCD_Write_String(" cm");
	LCD_SetCursor(0,2);
	LCD_Write_String("            ");
	LCD_SetCursor(0,3);
	LCD_Write_String("--------------------");
	_delay_ms(4000);
}