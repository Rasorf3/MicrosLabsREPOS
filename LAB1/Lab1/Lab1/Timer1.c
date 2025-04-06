/*
 * Timer1.c
 *
 * Created: 2/04/2025 2:16:30 p. m.
 *  Author: rasor
 */ 
#include "Timer1.h"

#include <avr/io.h>

// Función para configurar Timer1
void Timer1_Init() {
	TCCR1A = 0;             // Modo normal
	TCCR1B = (1 << CS11);   // Prescaler 1 (sin prescaler)
	// Para pulsos más largos usa prescaler 8: TCCR1B = (1 << CS11);
}

