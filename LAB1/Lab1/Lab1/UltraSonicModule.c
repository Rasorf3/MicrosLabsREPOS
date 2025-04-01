/*
 * UltraSonicModule.c
 *
 * Created: 3/29/2025 8:17:20 PM
 *  Author: Luis Felipe Holchor Virgen
 */ 

#include "UltraSonicModule.h"

void UltraSonicInit()
{
	DDRD |= (1 << TRIGGER) | (0 << ECHO);
	DDRD |= (1 << 6);
}

float GetDistance()
{
	int counter = 0;
	float distance = 0;
	PORTD |= (1 << TRIGGER);
	_delay_us(10);
	PORTD &= ~(1 << TRIGGER);
	// Esperar flanco de subida en Echo
	while (!(PIND & (1 << ECHO)));
	
	// Medir duraci?n del pulso (en us)
	while (PIND & (1 << ECHO)) {//PIND & (1 << ECHO)
		counter++;
		PORTD |= (1 << 6);
		_delay_us(1);  
	}
	PORTD &= ~(1 << 6);
	// Calcular distancia (en cm)
	distance = ((float)counter / 58.0);  // Formula: (us / 58) = cm
	
	return distance;
}