/*
 * DHT_22.h
 *
 * Created: 4/5/2025 6:18:58 PM
 *  Author: rasor
 */ 


#ifndef DHT_22_H_
#define DHT_22_H_

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define DHT22_PIN 7 

// Variables globales para los datos
volatile char humidity_int = 0;
volatile char humidity_dec = 0;
volatile char temp_int = 0;
volatile char temp_dec = 0;
volatile char checksum = 0;
volatile char dht22_data[5] = {0, 0, 0, 0, 0};

// Funci?n para configurar el pin del DHT22
void DHT22_init();

// Funci?n para leer datos del DHT22
char DHT22_read();

void DHT_Display_Data();

#endif /* DHT_22_H_ */