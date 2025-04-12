/*
 * RTC_Module.c
 *
 * Created: 4/4/2025 11:40:17 PM
 *  Author: rasor
 */ 
#include <avr/io.h>
#include "I2C_Module.h"
#include "RTC_Module.h"
#include "LCD_Module.h"

unsigned char rtc_register[7];
unsigned char time[10]; 		//xx:xx:xx;
unsigned char date[12];			//xx/xx/xxxx;
unsigned char day;

//******************************************************************
//Function to initialize the DS1307 memory address.
//******************************************************************
void RTC_setStartAddress(void)
{
   unsigned char errorStatus;
   
   errorStatus = i2c_start();
   if(errorStatus == 1)
   {
   	 i2c_stop();
	 return;
   } 
   
   errorStatus = i2c_sendAddress(DS1307_W);
   
   if(errorStatus == 1)
   {
	 i2c_stop();
	 return;
   } 
   
   errorStatus = i2c_sendData(0x00);
   if(errorStatus == 1)
   {
	 i2c_stop();
	 return;
   } 

   i2c_stop();
}

//******************************************************************
//Function to read RTC data.
//******************************************************************    
void RTC_read(void)
{

  unsigned char errorStatus, i, data;
  
  errorStatus = i2c_start();
   if(errorStatus == 1)
   {
   	 i2c_stop();
	 return;
   } 
   
   errorStatus = i2c_sendAddress(DS1307_W);
   
   if(errorStatus == 1)
   {
	 i2c_stop();
	 return;
   } 
   
   errorStatus = i2c_sendData(0x00);
   if(errorStatus == 1)
   {
	 i2c_stop();
	 return;
   } 

    errorStatus = i2c_repeatStart();
   if(errorStatus == 1)
   {
   	 i2c_stop();
	 return;
   } 
   
    errorStatus = i2c_sendAddress(DS1307_R);
   
   if(errorStatus == 1)
   {
	 i2c_stop();
	 return;
   } 
 
    for(i=0;i<7;i++)
   {
      if(i == 6)  	 //no Acknowledge after receiving the last byte
	   	  data = i2c_receiveData_NACK();
	  else
	  	  data = i2c_receiveData_ACK();
		  
   	  if(data == ERROR_CODE)
   	  {
			i2c_stop();
	   		return;
   	  }
	  
	  rtc_register[i] = data;
	}
	
	i2c_stop();
}	  

//******************************************************************
//Function to read time data.
//****************************************************************** 
void RTC_getTime(void)
{
   RTC_read();
   time[9] = 0x00;	  //NIL
   time[8] = ' ';
   time[7] = (SECONDS & 0x0f) | 0x30;		//seconds(1's)
   time[6] = ((SECONDS & 0x70) >> 4) | 0x30;		//seconds(10's)
   time[5] = ':';
   
   time[4] = (MINUTES & 0x0f) | 0x30;
   time[3] = ((MINUTES & 0x70) >> 4) | 0x30;
   time[2] = ':'; 
   
   time[1] = (HOURS & 0x0f) | 0x30;	
   time[0] = ((HOURS & 0x30) >> 4) | 0x30;
}

//******************************************************************
//Function to read date data
//****************************************************************** 
void RTC_getDate(void)
{
  RTC_read();
  date[11] = 0x00;  //NIL
  date[11] = ' ';
  date[9] = (YEAR & 0x0f) | 0x30;
  date[8] = ((YEAR & 0xf0) >> 4) | 0x30;
  date[7] = '0';
  date[6] = '2';
  date[5] = '/';
  date[4] = (MONTH & 0x0f) | 0x30;
  date[3] = ((MONTH & 0x10) >> 4) | 0x30;
  date[2] = '/';
  date[1] = (DATE & 0x0f) | 0x30;
  date[0] = ((DATE & 0x30) >> 4) | 0x30;
}  
  
//******************************************************************
//Function to display time. 
//****************************************************************** 
void RTC_displayTime(void)
{
  RTC_getTime();
  
  LCD_Write_String(time);
}

//******************************************************************
//Function to display date
//****************************************************************** 
void RTC_displayDate(void)
{
	RTC_getDate();
	LCD_Write_String(date);
}

void RTC_displayDay(void)
{
  switch(DAY)
  {
   case 1:
		  LCD_Write_String("Sunday ");
          break; 
   case 2:
		  LCD_Write_String("Monday ");
          break; 
   case 3:
		  LCD_Write_String("Tuesday ");
          break; 
   case 4:
		  LCD_Write_String("Wednesday ");
          break; 
   case 5:
		  LCD_Write_String("Thursday ");
          break; 		  
   case 6:
		  LCD_Write_String("Friday  ");
          break; 		  
   case 7:
		  LCD_Write_String("Saturday ");
          break; 
   default:
		  LCD_Write_String("Unknown ");
  }
}	
void RTC_display_data(void)
{
	LCD_Command(LCD_CLEAR);
	LCD_SetCursor(0,0);
	LCD_Write_String(" --------------------------------------");
	LCD_SetCursor(0,1);
	RTC_displayDate();
	LCD_SetCursor(10,1);
	LCD_Write_String("  ");
	RTC_displayDay();
	LCD_SetCursor(0,2);
	RTC_displayTime();
	LCD_SetCursor(0,3);
	LCD_Write_String("--------------------");	
}
		  		     	  
//******************************************************************
//Function to format registers 
//****************************************************************** 
void RTC_updateRegisters(void)
{
  SECONDS = ((time[6] & 0x07) << 4) | (time[7] & 0x0f);
  MINUTES = ((time[3] & 0x07) << 4) | (time[4] & 0x0f);
  HOURS = ((time[0] & 0x03) << 4) | (time[1] & 0x0f);  
  DAY = date[10];
  DATE = ((date[0]& 0x03) << 4) | (date[1] & 0x0f);
  MONTH = ((date[3] & 0x01) << 4) | (date[4] & 0x0f);
  YEAR = ((date[8] & 0x0f) << 4) | (date[9] & 0x0f);
}  


//******************************************************************
//Function to write time into the DS1307 
//******************************************************************   
unsigned char RTC_writeTime(void)
{
  unsigned char errorStatus, i;
  
   errorStatus = i2c_start();
   if(errorStatus == 1)
   {
   	 i2c_stop();
	 return(1);
   } 
   
   errorStatus = i2c_sendAddress(DS1307_W);
   
   if(errorStatus == 1)
   {
	 i2c_stop();
	 return(1);
   } 
   
   errorStatus = i2c_sendData(0x00);
   if(errorStatus == 1)
   {
	 i2c_stop();
	 return(1);
   } 

    for(i=0;i<3;i++)
    {
	  errorStatus = i2c_sendData(rtc_register[i]);  
   	  if(errorStatus == 1)
   	  {
			i2c_stop();
	   		return(1);
   	  }
    }
	
	i2c_stop();
	return(0);
}


//******************************************************************
//Function to write date into the DS1307 
//******************************************************************   
unsigned char RTC_writeDate(void)
{
  unsigned char errorStatus, i;
  
   errorStatus = i2c_start();
   if(errorStatus == 1)
   {
   	 i2c_stop();
	 return(1);
   } 
   
   errorStatus = i2c_sendAddress(DS1307_W);
   
   if(errorStatus == 1)
   {
	 i2c_stop();
	 return(1);
   } 
   
   errorStatus = i2c_sendData(0x03);
   if(errorStatus == 1)
   {
	 i2c_stop();
	 return(1);
   } 

    for(i=3;i<7;i++)
    {
	  errorStatus = i2c_sendData(rtc_register[i]);  
   	  if(errorStatus == 1)
   	  {
			i2c_stop();
	   		return(1);
   	  }
    }
	
	i2c_stop();
	return(0);
}
  
//******************************************************************
//Function to update time into the DS1307 
// Format (24h): hh:mm:ss
//******************************************************************   
void RTC_updateTime(unsigned char *strTime)
{
  unsigned char i, data;
  int timeError = 0;
  
  time[2] = ':';
  time[5] = ':';
  data = 0x31;
  if(data < 0x30 || data > 0x32)	//Hours.
	timeError = 1;
  else
    time[0] = *strTime;
	
  if(data < 0x30 || data > 0x39)
	timeError = 1;
  else
    time[1] = *(strTime + 1);
  
  if(data < 0x30 || data > 0x35)	//Minutes
	timeError = 1;
  else
    time[3] = *(strTime + 3);
	
  if(data < 0x30 || data > 0x39)
	timeError = 1;
  else
    time[4] = *(strTime + 4);
  
  if(data < 0x30 || data > 0x35)	//Seconds
	timeError = 1;
  else
    time[6] = *(strTime + 6);
	
  if(data < 0x30 || data > 0x39)
	timeError = 1;
  else
    time[7] = *(strTime + 7);
  
  if (timeError)
  {
	  return;
  }	  
  else
  {
	  RTC_updateRegisters();
	  data = RTC_writeTime();
  }	  
	
  return;
}  
//******************************************************************
//Function to update date into the DS1307
// Format: dd/mm/yy-WeekDay
//******************************************************************   
//RTC_updateDate(unsigned char *strDate);
void RTC_updateDate(unsigned char *strDate)
{
  unsigned char  data;
  int dateError = 0;
  data = 0x31;
  date[2] = '/';
  date[5] = '/';
  
  if(data < 0x30 || data > 0x33)	// Day.
	dateError =1;
  else
    date[0] = *strDate;
	
  if(data < 0x30 || data > 0x39)
	dateError =1;
  else
    date[1] = *(strDate + 1);
  
  if(data < 0x30 || data > 0x31)	// Month
	dateError =1;
  else
    date[3] = *(strDate + 3);
	
  if(data < 0x30 || data > 0x39)
	dateError =1;
  else
    date[4] = *(strDate + 4);
  
  date[6] = '2';					// Year
  date[7] = '0';
  
  if(data < 0x30 || data > 0x39)
	dateError =1;
  else
    date[8] = *(strDate + 8);
	
  if(data < 0x30 || data > 0x39)
	dateError =1;
  else
    date[9] = *(strDate + 9);

  if(data < 0x30 || data > 0x36)
	dateError =1;
  else
	date[10] = *(strDate + 11);
  
  if (dateError)
  {
		return;  
  }
  else
  {
	  RTC_updateRegisters();
	  data = RTC_writeDate();
  }
  
  return;
}  
