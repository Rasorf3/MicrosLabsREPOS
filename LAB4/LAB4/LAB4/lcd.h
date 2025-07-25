/*
 * lcd.h
 *
 * Created: 7/25/2025 10:26:15 AM
 *  Author: rasor
 */ 


#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>
#include "FreeRTOS.h"
#include "queue.h"

typedef struct {
	uint8_t row;            /* 0?3 */
	uint8_t col;            /* 0?19 */
	char    text[21];       /* 20 chars + '\0' */
} LCDMessage_t;

extern QueueHandle_t xLCDQueue;

void lcdInit(void);
void lcdSendCommand(uint8_t cmd);
void lcdSendData(uint8_t data);
void lcdSetCursor(uint8_t row, uint8_t col);
void vTaskLCD(void *pvParameters);

#endif /* LCD_H_ */
