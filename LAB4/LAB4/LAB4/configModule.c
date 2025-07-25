/*
 * configModule.c
 *
 * Created: 22/07/2025 9:49:41 a. m.
 *  Author: rasor
 */ 

#include "configModule.h"
#include "lcd.h"
#include <avr/interrupt.h>

/* Inicializa hardware y LCD */
void vInit(void)
{

	/* Aquí van tus otros init (GPIO, UART, ADC, SPI...) */
	/* ... */

	/* Inicializa el LCD 20×4 */
	lcdInit();

}

/* Crea únicamente la tarea que atiende el LCD */
void vCreatingTasks(UBaseType_t uxPriority)
{
	BaseType_t xResult;

	xResult = xTaskCreate(
	vTaskLCD,                            /* Función de la tarea */
	"LCD",                               /* Nombre para debug */
	configMINIMAL_STACK_SIZE + 150,      /* Pila */
	NULL,                                /* Parámetro */
	uxPriority,                          /* Prioridad */
	NULL                                 /* Sin handle */
	);
	configASSERT(xResult == pdPASS);
}


