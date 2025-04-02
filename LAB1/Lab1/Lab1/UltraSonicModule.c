/*
 * UltraSonicModule.c
 *
 * Created: 3/29/2025 8:17:20 PM
 *  Author: Luis Felipe Holchor Virgen
 */ 

#include "UltraSonicModule.h"
#define F_CPU 8000000UL
void UltraSonicInit()
{
	DDRC |= (1 << TRIGGER);
	DDRC &= ~(1 << ECHO);
	DDRC |= (1 << 6);
}

float GetDistance()
{
	int counter = 0;
	
	float distance = 0;
	PORTC |= (1 << TRIGGER);
	_delay_us(15);
	PORTC &= ~(1 << TRIGGER);
	// Esperar flanco de subida en Echo
	while (!(PINC & (1 << ECHO)));
	
	// Medir duraci?n del pulso (en us)
	while (PINC & (1 << ECHO)) {//PIND & (1 << ECHO)
		counter++;
		PORTC |= (1 << 6);
		_delay_us(1);  
	}
	PORTC &= ~(1 << 6);
	// Calcular distancia (en cm)
	distance = (float)counter / 58.0;  // Formula: (us / 58) = cm
	
	return distance;
}