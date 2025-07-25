/*
 * templateFRTOS.c
 *
 * Created: 14/05/2013 20:10:58
 *  Author: eBlady
 */ 


//FreeRTOS include files
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "FreeRTOS.h"
#include "task.h"
#include "configModule.h"
#include "lcd.h"
#include <string.h>

/* Tarea que envía un mensaje de prueba */
static void vTaskSender(void *pv)
{
	LCDMessage_t msg = {
		.row = 1,
		.col = 2,
	};
	strcpy(msg.text, "Prueba LCD FreeRTOS");

	/* Pequeña demora para asegurar que vTaskLCD creó la cola */
	vTaskDelay(pdMS_TO_TICKS(100));

	xQueueSend(xLCDQueue, &msg, portMAX_DELAY);

	/* Auto-destrucción: no se necesita más */
	vTaskDelete(NULL);
}

int main(void)
{
	/* 1. Hardware y LCD */
	//vInit();
	lcdInit();
	while (1) {
		lcdSendData('H');
		_delay_ms(500);
		lcdSendData('i');
		_delay_ms(500);
	}

}


