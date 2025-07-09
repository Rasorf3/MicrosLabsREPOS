/*
 * RTC_Module.h
 *
 * Created: 4/4/2025 11:40:32 PM
 *  Author: rasor
 */ 


#ifndef RTC_MODULE_H_
#define RTC_MODULE_H_

#include <stdint.h>

/** Búfer de 7 bytes BCD: seg, min, hr, día_sem, fecha, mes, año */
extern uint8_t rtc_register[7];

/** Cadenas para mostrar en LCD */
extern char time_str[9];   // "hh:mm:ss\0"
extern char date_str[11];  // "dd/mm/yyyy\0"

/** Dirección I2C del DS3232 (8-bit) */
#define DS3232_W  0xD0
#define DS3232_R  0xD1

/** 
 * Inicializa el módulo RTC. 
 * Llama internamente a i2c_async_init() y en el callback
 * actualiza lcd con date_str/time_str.
 */
void RTC_init(void);

/**
 * Inicia la lectura asíncrona de los 7 registros BCD.
 * Al terminar, en el callback se formatean y muestran en pantalla.
 */
void RTC_read_async(void);

/** 
 * Escribe la hora contenida en rtc_register[0..2] (BCD) en el DS3232. 
 * Devuelve 0 si OK, !=0 si error I2C.
 */
uint8_t RTC_writeTime(void);

/** 
 * Escribe la fecha contenida en rtc_register[3..6] (BCD) en el DS3232. 
 */
uint8_t RTC_writeDate(void);

/** 
 * Formatea la hora actual (rtc_register) en time_str. 
 * Puedes usarla si quieres refrescar la cadena sin leer. 
 */
void RTC_getTime(void);

/** 
 * Formatea la fecha actual (rtc_register) en date_str. 
 */
void RTC_getDate(void);

/** Muestra time_str en la posición 0,1 del LCD */
void RTC_displayTime(void);

/** Muestra date_str en la posición 0,0 del LCD */
void RTC_displayDate(void);

/** Muestra el día de la semana (rtc_register[3]) en 1,1 */
void RTC_displayDay(void);

/** Pantalla completa: fecha, día y hora */
void RTC_display_data(void);

/**
 * Rellena rtc_register a partir de cadenas:
 *    strTime = "hh:mm:ss"
 *    strDate = "dd/mm/yyyy"
 */
void RTC_updateTime(const char *strTime);
void RTC_updateDate(const char *strDate);

/** Convierte time_str/date_str ? rtc_register (BCD) */
void RTC_updateRegisters(void);

#endif  // RTC_MODULE_H_
