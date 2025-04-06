/*
 * DHT_22.c
 *
 * Created: 4/5/2025 6:19:05 PM
 *  Author: rasor
 */ 
#include "DHT_22.h"
#include "LCD_Module.h"

// Funci?n para configurar el pin del DHT22
void DHT22_init() {
	DDRD |= (1 << DHT22_PIN);   // Configura el pin como salida
	PORTD |= (1 << DHT22_PIN);  // Pone el pin en alto
	_delay_ms(100);             // Espera inicial
}
/*		  18ms	   40us	    80us	80us
	¯¯¯¯|_______|¯¯¯¯¯¯¯¯|_______|¯¯¯¯¯¯¯|....datos....
	
	---------PUC---------|----------dht22-------------|      */	
// Funci?n para leer datos del DHT22
char DHT22_read() {
	char bits[5] = {0, 0, 0, 0, 0};
	char i, j = 0;
	
	// Inicio de la comunicaci?n
	DDRD |= (1 << DHT22_PIN);   // Configura el pin como salida
	PORTD &= ~(1 << DHT22_PIN); // Pone el pin en bajo
	_delay_ms(18);               // Espera al menos 1ms (m?nimo 1ms seg?n datasheet)
	
	PORTD|= (1 << DHT22_PIN);  // Pone el pin en alto
	DDRD &= ~(1 << DHT22_PIN);
	
	// Espera la respuesta del sensor
	unsigned int contador = 0;
	while(PIND & (1<<DHT22_PIN))
	{
		_delay_us(2);
		contador += 2;
		if (contador > 60)
		{
			DDRD |= (1<<DHT22_PIN);	//Pin como salida
			PORTD |= (1<<DHT22_PIN);	//Nivel alto
			return 1;
		}
	}
	contador = 0;
	while(!(PIND & (1<<DHT22_PIN)))
	{
		_delay_us(2);
		contador += 2;
		if (contador > 100)
		{
			DDRD |= (1<<DHT22_PIN);	//Pin como salida
			PORTD |= (1<<DHT22_PIN);	//Nivel alto
			return 1;
		}
	}
	contador = 0;
	while(PIND & (1<<DHT22_PIN))
	{
		_delay_us(2);
		contador += 2;
		if (contador > 100)
		{
			DDRD |= (1<<DHT22_PIN);	//Pin como salida
			PORTD |= (1<<DHT22_PIN);	//Nivel alto
			return 1;
		}
	}
	

	// Lectura de los 40 bits de datos
	for (j=0; j<5; j++)
	{
		unsigned char result=0;
		for (i=0; i<8; i++)
		{
			while (!(PIND & (1<<DHT22_PIN)));
			_delay_us(35);

			if (PIND & (1<<DHT22_PIN))
			result |= (1<<(7-i));
			
			while(PIND & (1<<DHT22_PIN));
		}
		bits[j] = result;
	}
	DDRD |= (1<<DHT22_PIN);	//Pin como salida
	PORTD |= (1<<DHT22_PIN);	//Nivel alto
	
	// Almacena los datos en las variables globales
	unsigned int humidity = (bits[0] << 8) | (bits[1]);
	unsigned int temperature = (bits[2] << 8) | (bits[3]);
	humidity_int = humidity/10;
	humidity_dec = humidity%10;
	temp_int = temperature/10;
	temp_dec = temperature%10;
	checksum = bits[4];
	
	// Verifica el checksum
	if (((bits[0] + bits[1] + bits[2] + bits[3]) & 0xFF) != bits[4])
	{
		return 3; // Error de checksum
	}
	
	
	return 0; // Lectura exitosa
}
void DHT_Display_Data()
{
	unsigned char buffer[20];
	sprintf(buffer," Temp: %d,%d C.",temp_int,temp_dec);
	LCD_Command(LCD_CLEAR);
	LCD_SetCursor(0,0);
	LCD_Write_String(buffer);
	sprintf(buffer," Hum: %d,%d %.",humidity_int,humidity_dec);
	LCD_SetCursor(0,1);
	LCD_Write_String(buffer);
	_delay_ms(2000);
}