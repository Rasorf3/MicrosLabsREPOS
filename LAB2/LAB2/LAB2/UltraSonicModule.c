/*
 * UltraSonicModule.c
 *
 * Created: 3/29/2025 8:17:20 PM
 *  Author: Luis Felipe Holchor Virgen
 */ 

#include "UltraSonicModule.h"
#include "Timer2.h"
#include "LCD_Module.h"

#include <avr/interrupt.h>

void UltraSonicInit()
{
	DDRC |= (1 << TRIGGER);
	DDRC &= ~(1 << ECHO);
	
	//Habilitar Interrupcion ECHO
	PCICR |= (1 << PCIE1);  // Habilita PCI1 
	PCMSK1 |= (1 << PCINT11); // Habilita mascara para PCINT11
	
	
	start_time = 0;
	end_time = 0;
	current_state = STATE_IDLE;
	distance_cm = 0;
}

void SendTrigger()
{
	PORTC |= (1 << TRIGGER);
	_delay_us(12);
	PORTC &= ~(1 << TRIGGER);
	current_state = STATE_TRIGGER_SENT;
}

uint16_t  measure_pulse_width() 
{
	return end_time - start_time;
}

void UltraSonic_Display_Data(float average)
{
	float distancia_basura = 0;
	unsigned char buffer[20];
	
	distancia_basura = average;
	
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
ISR(PCINT1_vect)
{
	if(PINC & (1 << ECHO) && current_state == STATE_TRIGGER_SENT)
	{
		Timer2_Start();
		Timer2_reset();              // Reiniciar contador
		start_time = Timer2_getTime();     // Leer timer (casi cero)
		current_state = STATE_MEASURING;
	}
	else if(current_state == STATE_MEASURING && !(PINC & (1 << ECHO)))
	{
		end_time = Timer2_getTime();
		Timer2_Stop();
		current_state = STATE_READY;
	}
}