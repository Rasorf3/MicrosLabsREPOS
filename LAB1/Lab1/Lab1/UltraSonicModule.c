/*
 * UltraSonicModule.c
 *
 * Created: 3/29/2025 8:17:20 PM
 *  Author: Luis Felipe Holchor Virgen
 */ 

#include "UltraSonicModule.h"

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
	_delay_us(10);
	PORTC &= ~(1 << TRIGGER);
	// Esperar flanco de subida en Echo
	while (!(PINC & (1 << ECHO)));
	
	// Medir duraci?n del pulso (en us)
	while (PINC & (1 << ECHO)) {//PIND & (1 << ECHO)
		counter++;
	
		_delay_us(1);  
	}
	// Calcular distancia (en cm)
	distance = ((float)counter) / 58.0f;  // Formula: (us / 58) = cm
	
	return distance;
}