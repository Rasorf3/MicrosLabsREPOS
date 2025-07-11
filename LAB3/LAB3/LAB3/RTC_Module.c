/*
 * RTC_Module.c
 *
 * Created: 4/4/2025 11:40:17 PM
 *  Author: rasor
 */ 
#include "RTC_Module.h"
#include "i2c_async.h"
#include "LCD_Module.h"
#include <util/delay.h>
#include <string.h>

uint8_t rtc_register[7];
char    time_str[9];
char    date_str[11];

/** Puntero al registro inicial (00h) */
static uint8_t ptr_reg = 0x00;

/** Prototipos de callbacks internos */
static void rtc_addr_cb(I2C_Result_t res, uint8_t *data, uint8_t len);
static void rtc_read_cb (I2C_Result_t res, uint8_t *data, uint8_t len);
static const char *day_names[8] = {
	"",         // índice 0 no usado
	"Sunday",   // 1
	"Monday",   // 2
	"Tuesday",  // 3
	"Wednesday",// 4
	"Thursday", // 5
	"Friday",   // 6
	"Saturday"  // 7
};

void RTC_init(void)
{
	i2c_async_init();
	// el LCD debe inicializarlo el main antes de llamar a RTC_read_async()
}

/** Secuencia write(ptr=0x00) ? read(7) */
void RTC_read_async(void)
{
	i2c_async_write(DS3232_W, &ptr_reg, 1, rtc_addr_cb);
}

static void rtc_addr_cb(I2C_Result_t res, uint8_t *data, uint8_t len)
{
	if (res == I2C_OK) {
		i2c_async_read(DS3232_R, rtc_register, 7, rtc_read_cb);
	}
}

static void rtc_read_cb(I2C_Result_t res, uint8_t *data, uint8_t len)
{
	if (res != I2C_OK || len != 7) return;

	/** BCD ? ASCII en cadenas */
	time_str[0] = ((rtc_register[2] >> 4)&0x03) + '0';
	time_str[1] = ( rtc_register[2]       &0x0F) + '0';
	time_str[2] = ':';
	time_str[3] = ((rtc_register[1] >> 4)&0x07) + '0';
	time_str[4] = ( rtc_register[1]       &0x0F) + '0';
	time_str[5] = ':';
	time_str[6] = ((rtc_register[0] >> 4)&0x07) + '0';
	time_str[7] = ( rtc_register[0]       &0x0F) + '0';
	time_str[8] = '\0';

	date_str[0]  = ((rtc_register[4] >> 4)&0x03) + '0';
	date_str[1]  = ( rtc_register[4]       &0x0F) + '0';
	date_str[2]  = '/';
	date_str[3]  = ((rtc_register[5] >> 4)&0x01) + '0';
	date_str[4]  = ( rtc_register[5]       &0x0F) + '0';
	date_str[5]  = '/';
	date_str[6]  = '2';
	date_str[7]  = '0';
	date_str[8]  = ((rtc_register[6] >> 4)&0x0F) + '0';
	date_str[9]  = ( rtc_register[6]       &0x0F) + '0';
	date_str[10] = '\0';


	// 3) Limpiar y pintar fecha en fila 0
	LCD_Command(LCD_CLEAR);
	LCD_SetCursor(0, 0);
	LCD_Write_String("--------------------------");
	LCD_SetCursor(0, 1);
	LCD_Write_String(date_str);

	// 4) Pintar nombre del día en fila 1
	{
		uint8_t dow = rtc_register[3];
		if (dow >= 1 && dow <= 7) {
			
			LCD_Write_String(" ");
			LCD_Write_String(day_names[dow]);
		}
	}

	// 5) Pintar hora en fila 2
	LCD_SetCursor(0, 2);
	LCD_Write_String(time_str);
	LCD_SetCursor(0, 3);
	LCD_Write_String("--------------------------");
}

/** Escribe los bytes 0–2 (hora) */
uint8_t RTC_writeTime(void)
{
	uint8_t buf[4] = {
		0x00,
		rtc_register[0],
		rtc_register[1],
		rtc_register[2]
	};
	return i2c_async_write(DS3232_W, buf, 4, NULL) != I2C_OK;
}

/** Escribe los bytes 3–6 (fecha) */
uint8_t RTC_writeDate(void)
{
	uint8_t buf[5] = {
		0x03,
		rtc_register[3],
		rtc_register[4],
		rtc_register[5],
		rtc_register[6]
	};
	return i2c_async_write(DS3232_W, buf, 5, NULL) != I2C_OK;
}

/** Formatea BCD?ASCII en time_str */
void RTC_getTime(void)
{
	time_str[0] = ((rtc_register[2] >> 4)&0x03) + '0';
	time_str[1] = ( rtc_register[2]       &0x0F) + '0';
	time_str[2] = ':';
	time_str[3] = ((rtc_register[1] >> 4)&0x07) + '0';
	time_str[4] = ( rtc_register[1]       &0x0F) + '0';
	time_str[5] = ':';
	time_str[6] = ((rtc_register[0] >> 4)&0x07) + '0';
	time_str[7] = ( rtc_register[0]       &0x0F) + '0';
	time_str[8] = '\0';
}

/** Formatea BCD?ASCII en date_str */
void RTC_getDate(void)
{
	date_str[0]  = ((rtc_register[4] >> 4)&0x03) + '0';
	date_str[1]  = ( rtc_register[4]       &0x0F) + '0';
	date_str[2]  = '/';
	date_str[3]  = ((rtc_register[5] >> 4)&0x01) + '0';
	date_str[4]  = ( rtc_register[5]       &0x0F) + '0';
	date_str[5]  = '/';
	date_str[6]  = '2';
	date_str[7]  = '0';
	date_str[8]  = ((rtc_register[6] >> 4)&0x0F) + '0';
	date_str[9]  = ( rtc_register[6]       &0x0F) + '0';
	date_str[10] = '\0';
}

/** Muestra en LCD sin cambiar cursor */
void RTC_displayTime(void)
{
	RTC_getTime();
	LCD_Write_String(time_str);
}

void RTC_displayDate(void)
{
	RTC_getDate();
	LCD_Write_String(date_str);
}

void RTC_displayDay(void)
{
	uint8_t d = rtc_register[3];
	switch(d) {
		case 1: LCD_Write_String("Sunday ");   break;
		case 2: LCD_Write_String("Monday ");   break;
		case 3: LCD_Write_String("Tuesday ");  break;
		case 4: LCD_Write_String("Wednesday ");break;
		case 5: LCD_Write_String("Thursday "); break;
		case 6: LCD_Write_String("Friday  ");  break;
		case 7: LCD_Write_String("Saturday ");break;
		default: LCD_Write_String("Unknown "); break;
	}
}

void RTC_display_data(void)
{
	LCD_Command(LCD_CLEAR);
	LCD_SetCursor(0,0);
	LCD_Write_String("----------------");
	LCD_SetCursor(1,0);
	RTC_displayDate();
	LCD_SetCursor(11,1);
	LCD_Write_String(" ");
	RTC_displayDay();
	LCD_SetCursor(0,2);
	RTC_displayTime();
	LCD_SetCursor(0,3);
	LCD_Write_String("----------------");
	_delay_ms(100);
}

/** Convierte time_str/date_str ? rtc_register (BCD) */
void RTC_updateRegisters(void)
{
	rtc_register[0] = ((time_str[6]-'0')<<4) | (time_str[7]-'0'); // seg
	rtc_register[1] = ((time_str[3]-'0')<<4) | (time_str[4]-'0'); // min
	rtc_register[2] = ((time_str[0]-'0')<<4) | (time_str[1]-'0'); // hr

	// día_sem no lo cubre time_str
	rtc_register[4] = ((date_str[0]-'0')<<4) | (date_str[1]-'0'); // date
	rtc_register[5] = ((date_str[3]-'0')<<4) | (date_str[4]-'0'); // month
	rtc_register[6] = ((date_str[8]-'0')<<4) | (date_str[9]-'0'); // year
}

/** Recibe "hh:mm:ss". Actualiza RTC. */
void RTC_updateTime(const char *strTime)
{
	memcpy(time_str, strTime, 8);
	time_str[8]='\0';
	RTC_updateRegisters();
	RTC_writeTime();
}

/** Recibe "dd/mm/yyyy". Actualiza RTC. */
void RTC_updateDate(const char *strDate)
{
	memcpy(date_str, strDate, 10);
	date_str[10]='\0';
	RTC_updateRegisters();
	RTC_writeDate();
}

//extern char time_str[9];   // "hh:mm:ss\0"

char getHu()
{
	return time_str[1];
}
char getHd()
{
	return time_str[0];
}
char getMd()
{
	return time_str[3];
}
char getMu()
{
	return time_str[4];
}
