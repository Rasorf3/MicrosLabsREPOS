/*
 * RTC_Module.h
 *
 * Created: 4/4/2025 11:40:32 PM
 *  Author: rasor
 */ 


#ifndef RTC_MODULE_H_
#define RTC_MODULE_H_

#define		SECONDS			rtc_register[0]
#define		MINUTES			rtc_register[1]
#define		HOURS			rtc_register[2]
#define		DAY    			rtc_register[3]
#define		DATE   			rtc_register[4]
#define		MONTH  			rtc_register[5]
#define		YEAR   			rtc_register[6]

#define  DS1307_W			0xd0
#define  DS1307_R			0xd1

void RTC_setStartAddress(void);
void RTC_read(void);
void RTC_write(void);
void RTC_getTime(void);
void RTC_getDate(void);
void RTC_displayTime(void);
void RTC_displayDate(void);
void RTC_displayDay(void);
void RTC_updateRegisters(void);
unsigned char RTC_writeTime(void);
unsigned char RTC_writeDate(void);
void RTC_updateTime(unsigned char *strTime);
void RTC_updateDate(unsigned char *strDate);


#endif /* RTC_MODULE_H_ */