/*************************************************************************
* Title:    I2C master library using hardware TWI interface
* Author:   Peter Fleury <pfleury@gmx.ch>  http://jump.to/fleury
* File:     $Id: twimaster.c,v 1.3 2005/07/02 11:14:21 Peter Exp $
* Software: AVR-GCC 3.4.3 / avr-libc 1.2.3
* Target:   any AVR device with hardware TWI 
* Usage:    API compatible with I2C Software Library i2cmaster.h
**************************************************************************/

#include <avr/io.h>
#include <inttypes.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include "i2cmaster.h"

#define TW_STATE_START      0
#define TW_STATE_ADDRESS    1
#define TW_STATE_READY      2
#define TW_STATE_STOP       3
#define TW_STATE_WRITE      4
#define TW_STATE_READ       5
#define TW_STATE_NULL       6
#define TRUE 1
#define FALSE 0


/**
 * @brief estado de la interfaz TWI
 * Puede tomar uno de los siguientes valores:
 */
static volatile unsigned char tw_state;
static volatile unsigned char isRead;
static volatile unsigned char isReady;
static unsigned char i2cLen = 0;
static I2C_frame_t I2C_frame;


/*************************************************************************
 Initialization of the I2C bus interface. Need to be called only once
*************************************************************************/
/**
 * @fn void i2c_init()
 * @brief funcion que inicializa el canal I2C, velocidad del bus 100kHz, prescaler = 1 
 */
void i2c_init()
{
	/* initialize TWI clock: 100 kHz clock, TWPS = 0 => prescaler = 1 */
	TWSR = TWI_TWPS;
	TWBR = TWI_TWBR; /* must be > 10 for stable operation */
	TWCR = (1<<TWIE);
  
	tw_state = TW_STATE_NULL;
	
	isReady = FALSE;
	i2cLen = 0;

}/* i2c_init */


/*************************************************************************	
  Issues a start condition and sends address and transfer direction.
  return 0 = device accessible, 1= failed to access device
*************************************************************************/

/**
 * @fn void _i2c_write(unsigned char address, unsigned char reg_add, unsigned char* data, unsigned char length)
 * @brief funcion que envía condición de start, completa la estructura de datos que se usa en la máquina de estados
 * y setea variables para escritura en registros de dispositivo esclavo.
 * @param address unsigned char con dirección de escritura de dispositivo esclavo.
 * @param reg_add unsigned char con dirección de registro a escribir.
 * @param *data unsigned char, es la dirección donde se encuentran los datos a escribir.
 * @param length unsigned char, cantidad de caracteres a escribir en el bus.
 */
void _i2c_write(unsigned char address, unsigned char reg_add, unsigned char* data, unsigned char length)
{
	
	//enciende la unidad TWI y envia bit de START.
	TWCR |= (1<<TWSTA) | (1<<TWEN) | (1<<TWINT);
	
	//alistar la estructura para enviar 
	I2C_frame.SLA_address = address;
	I2C_frame.REG_address = reg_add;
	I2C_frame.data = data;
	I2C_frame.length = length;
	
	isRead = FALSE;
	isReady = FALSE;
	tw_state = TW_STATE_START;
	
}

/**
 * @fn void _i2c_read(unsigned char address, unsigned char reg_add, unsigned char* data, unsigned char length)
 * @brief funcion que envía condición de start, completa la estructura de datos que se usa en la máquina de estados
 * y setea variables para lectura de registros de dispositivo esclavo.
 * @param address unsigned char con dirección de escritura de dispositivo esclavo.
 * @param reg_add unsigned char con dirección de registro a leer.
 * @param *data unsigned char, es la dirección donde guardarn los datos leidos.
 * @param length unsigned char, cantidad de caracteres a leer del bus 
  */
void _i2c_read(unsigned char address, unsigned char reg_add, unsigned char* data, unsigned char length)
{
	
	//enciende la unidad TWI y envia bit de START.
	TWCR |= (1<<TWSTA) | (1<<TWEN) | (1<<TWINT);
	
	//alistar la estructura para enviar
	I2C_frame.SLA_address = address;
	I2C_frame.REG_address = reg_add;
	I2C_frame.data = data;
	I2C_frame.length = length;
	
	isRead = TRUE;
	isReady = FALSE;
	tw_state = TW_STATE_START;
}

/**
 * @fn unsigned char i2c_isReady(void)
 * @brief Función que retorna uno cuando el bus I2C está libre.
 * @return isReady=1 cuando está libre el bus, isReady= 0 cuando está ocupado
  */
unsigned char i2c_isReady(void)
{
	return isReady;
}


ISR(TWI_vect)
{
	unsigned char tmp;
	//static unsigned char i = 0;
	switch(tw_state)
	{
		
		//ya envio el start
		case TW_STATE_START:
			
			tmp = TW_STATUS;
			if ( (tmp == TW_START) || (tmp == TW_REP_START))
			{
				// send device address
				tw_state = TW_STATE_ADDRESS;
				
				TWDR = I2C_frame.SLA_address;
				//la bandera siempre se tiene que limpiar manualmente
				TWCR |= (1<<TWINT);
			
			}
			//vuelve a enviar START
			else
			{
				//enciende la unidad TWI y envia bit de START.
				TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN) | (1<<TWIE);

				tw_state = TW_STATE_START;
			}

		
		break;
	  
		//ya envio la direccion del esclavo
		case TW_STATE_ADDRESS:
			tmp = TW_STATUS;
			if ( (tmp == TW_MT_SLA_ACK) || (tmp == TW_MR_SLA_ACK) )
			{
				if(I2C_frame.SLA_address & I2C_READ)
				{
					tw_state = TW_STATE_READ;
					
					TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA) | (1<<TWIE);
				}
				else
				{
					tw_state = TW_STATE_WRITE;
					
					TWDR = I2C_frame.REG_address;
					//la bandera siempre se tiene que limpiar manualmente
					TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);	
				}
				
				
			}
			//vuelve a enviar START
			else
			{
				//enciende la unidad TWI y envia bit de START.
				TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN) | (1<<TWIE);

				tw_state = TW_STATE_START;
			}
			
	  	break;
		
		//ya envio direccion de datos, ahora si la info
		case TW_STATE_WRITE:
		
			tmp = TW_STATUS;
			if ( (tmp == TW_MT_DATA_ACK ))
			{
				if(isRead)
				{
					//hasta aqui ya ha enviado la direccion del registro. Se requiere volver a generar un START
					tw_state = TW_STATE_START;
					
					I2C_frame.SLA_address += I2C_READ;
					
					TWCR = (1<<TWSTA) | (1<<TWEN) | (1<<TWINT) | (1<<TWIE);
				}
				else
				{
					tw_state = TW_STATE_WRITE;
					//
					if(i2cLen<I2C_frame.length)
					{
						TWDR = I2C_frame.data[i2cLen++];
						//la bandera siempre se tiene que limpiar manualmente
						TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);
					}
					else
					{
						//reinicia el contador
						i2cLen = 0;
						tw_state = TW_STATE_STOP;
						//evnviar bit de STOP despues de enviar todos los bytes
						TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO) | (1<<TWIE);
						
						//ya la unidad I2C esta lista para enviar otros datos
						isReady = TRUE;
						
					}	
				}
				
					
			}
			//falta definir que hace cuando no recibe ACK. deberia enviar el mismo byte o empezar la trama?
		
		break;
		
		case TW_STATE_READ:
		
			if(i2cLen<I2C_frame.length)
			{
				I2C_frame.data[i2cLen++] = TWDR;
				
				if(i2cLen<I2C_frame.length-1)
					TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA) | (1<<TWIE);
				else
					//se va a enviar el ultimo byte
					TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);
			}
			
			else
			{
				//reinicia el contador
				i2cLen = 0;
				tw_state = TW_STATE_STOP;
				//enviar bit de STOP despues de recibir todos los bytes
				TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO) | (1<<TWIE);
				
				//ya la unidad I2C esta lista para enviar otros datos
				isReady = TRUE;
			}
			
		break;
		case TW_STATE_STOP:
		
			if(!(TWCR & (1<<TWSTO)))
				tw_state = TW_STATE_NULL;
		
		break;
		
		case TW_STATE_NULL:
		default:
			//no hace nada
		break;
		
		
  }
  

}
