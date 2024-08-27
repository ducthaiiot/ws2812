/**
@file
Application for communicating with OP320A&S SNM over RS232/485 (via RTU protocol).
*/
/*
  Application_SNM.h - Arduino library for communicating with OP320A&S SNM
  over RS232/485 (via RTU protocol).

  Library:: chickyPig_OP320_V1

  This version is

  Copyright:: 2021 nguyentrinhtuan1996@gmail.com
*/

#ifndef _Application_atApp_HMI_
#define _Application_atApp_HMI_
/* _____PROJECT INCLUDES____________________________________________________ */
#include "App.h"
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "atApp_DS18B20.h"
#include <TM1637.h>
/* _____DEFINETIONS__________________________________________________________ */
#define Up 		256
#define Down	1
#define Back	512
#define Ok		2
enum screen_Index{
	screen_Index_Monitor,
	screen_Index_Setting,
	screen_Index_Setting_Temperature_High_Limit,
	screen_Index_Monitoring_All_Sensor,
	screen_Index_Monitoring_Sensor,
	screen_Index_Alarm
};
enum alarm_states{
	alarm_Not_Yet,
	alarm_Executing,
	alarm_Done
};
/* _____GLOBAL VARIABLES_____________________________________________________ */
TM1637 _tm1637(29, 17);  // DIO=2, CLK=3


///////////////////////////////////////////////Testing part//
/* _____GLOBAL FUNCTION______________________________________________________ */
bool Button_Ok_Pressed();
bool Button_Up_Pressed();
bool Button_Down_Pressed();
bool Button_Back_Pressed();

void atApp_HMI_Task_Func();
/* _____CLASS DEFINITION_____________________________________________________ */
/**
 * This Application class is the application to manage the 
 */
class App_HMI : public Application
{
public:
  	App_HMI();
 	~App_HMI();
  	static void  App_HMI_Pend();
	static void  App_HMI_Start();
	static void  App_HMI_Restart();
	static void  App_HMI_Execute();
	static void  App_HMI_Suspend();
	static void  App_HMI_Resume();	  
	static void  App_HMI_End();
	uint8_t Max_Temperature_Limit = 0;
	uint8_t Max_Temperature = 0;
	uint8_t Temperature_At_Sensor[MAX_TEMPERATURE_SENSOR_NUMBER];
	bool Timer1_1s = 0;
	uint8_t screen_Index = 0;
	uint8_t Alarm = 0;
	uint8_t Alarm_Sensor = 0;
protected:
	static void screen_Monitor();
	static void screen_Setting();
	static void 	screen_Setting_Temperature_High_Limit();
	static void 	screen_Monitoring_All_Sensor();
	static void 		screen_Monitoring_Sensor();
	static void screen_Alarm();
private:
	
} atApp_HMI ;
/**
 * This function will be auto called when a object is created by this class
 */
App_HMI::App_HMI(/* args */)
{
  	_Pend_User 	     = *App_HMI_Pend;
	_Start_User 	 = *App_HMI_Start;
	_Restart_User 	 = *App_HMI_Restart;
	_Execute_User 	 = *App_HMI_Execute;
	_Suspend_User	 = *App_HMI_Suspend;
	_Resume_User	 = *App_HMI_Resume;
	_End_User	     = *App_HMI_End;

	// change the ID of application
	ID_Application = 3;
	// change the application name
	Name_Application = (char*)"HMI Application";
	// change the ID of SNM
}
/**
 * This function will be auto called when the object of class is delete
 */
App_HMI::~App_HMI()
{

}
/**
 * Pend to start is the first task of this application it will do prerequisite condition. In the debug mode, task will send information of application to terminal to start the application.
 */
void  App_HMI::App_HMI_Pend()
{
		// change the set all data to 0
	for (uint8_t i = 0; i < MAX_TEMPERATURE_SENSOR_NUMBER; i++)
	{
		atApp_DS.Temperature_At_Sensor[i] = 0;
	}

	_tm1637.clearDisplay();
	delay(1);
	_tm1637.setupDisplay(true, 1);
}
/**
 * This start function will init some critical function 
 */
void  App_HMI::App_HMI_Start()
{
     // initialize timer1 for interrupt for 1 s
	// noInterrupts();    // disable all interrupts
	// TCCR1A = 0;
	// TCCR1B = 0;

	// TCNT1 = 3036;    // preload timer 65536-16MHz/256/1Hz
	// TCCR1B |= (1 << CS12);    // 256 prescaler 
	// TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
	// interrupts();    // enable all interrupts
}  
/**
 * Restart function of SNM  app
 */
void  App_HMI::App_HMI_Restart()
{

}
/**
 * Execute fuction of SNM app
 */
void  App_HMI::App_HMI_Execute()
{	
	if(atApp_HMI.Alarm == alarm_Executing) atApp_HMI.screen_Index = screen_Index_Alarm;

	switch (atApp_HMI.screen_Index)
	{
	case screen_Index_Monitor:
		atApp_HMI.screen_Monitor();
		break;
	
	case screen_Index_Setting:
		atApp_HMI.screen_Setting();
		break;

	case screen_Index_Setting_Temperature_High_Limit:
		atApp_HMI.screen_Setting_Temperature_High_Limit();
		break;
	
	case screen_Index_Monitoring_All_Sensor:
		atApp_HMI.screen_Monitoring_All_Sensor();
		break;
		
	case screen_Index_Monitoring_Sensor:
		atApp_HMI.screen_Monitoring_Sensor();
		break;
	
	case screen_Index_Alarm:
		atApp_HMI.screen_Alarm();
		break;

	default:
		break;
	}

    if(atApp_HMI.User_Mode == APP_USER_MODE_DEBUG)
    {
		
    }   
}
void  App_HMI::App_HMI_Suspend(){}
void  App_HMI::App_HMI_Resume(){}	  
void  App_HMI::App_HMI_End(){}
void  App_HMI::screen_Monitor(){
	// LED 7seg
	char text[5];
	sprintf(text,"%doC ",atApp_HMI.Max_Temperature);
	_tm1637.setDisplayToString(text);
	delay(1);
	// button
	if (Button_Ok_Pressed())
	{	
		while(Button_Ok_Pressed());
		atApp_HMI.screen_Index = screen_Index_Setting;
	}
	
	
}
int8_t screen_Setting_Count = 0;
void App_HMI::screen_Setting()
{
	// LED 7seg
	char text[5];
	sprintf(text,"P%d  ",screen_Setting_Count);
	_tm1637.setDisplayToString(text);	
	delay(10);
	// button
	switch (_tm1637.getButtons())
	{
		case Up:
			while(Button_Up_Pressed());
			screen_Setting_Count = 1;
			break;

		case Down:
			while(Button_Down_Pressed());
			screen_Setting_Count = 0;
			break;

		case Back:
			while(Button_Back_Pressed());
			atApp_HMI.screen_Index = screen_Index_Monitor;
			break;

		case Ok:
			while(Button_Ok_Pressed());
			if (screen_Setting_Count == 0 ) atApp_HMI.screen_Index = screen_Index_Setting_Temperature_High_Limit;
			if (screen_Setting_Count == 1 ) atApp_HMI.screen_Index = screen_Index_Monitoring_All_Sensor;
			break;
		
		default:
			break;

	}
	
}
void App_HMI::screen_Setting_Temperature_High_Limit(){
	// LED 7seg
	char text[5];
	sprintf(text,"HT%d  ",atApp_HMI.Max_Temperature_Limit);
	_tm1637.setDisplayToString(text);	
	delay(10);
	// button
	switch (_tm1637.getButtons())
	{
		case Up:
			while(Button_Up_Pressed());
			atApp_HMI.Max_Temperature_Limit++;
			if(atApp_HMI.Max_Temperature_Limit > 80) atApp_HMI.Max_Temperature_Limit = 80;
			break;

		case Down:
			while(Button_Down_Pressed());
			atApp_HMI.Max_Temperature_Limit--;
			if(atApp_HMI.Max_Temperature_Limit < 20) atApp_HMI.Max_Temperature_Limit = 20;
			break;

		case Back:
			while(Button_Back_Pressed());
			atApp_HMI.screen_Index = screen_Index_Setting;
			break;

		case Ok:
			break;
		
		default:
			break;
	}

}

uint8_t screen_Monitoring_All_Sensor_Count = 0;
void App_HMI::screen_Monitoring_All_Sensor(){
	// LED 7seg
	char text[5];
	sprintf(text,"Cb%d  ",screen_Monitoring_All_Sensor_Count);
	_tm1637.setDisplayToString(text);	
	delay(1);
	// button
	switch (_tm1637.getButtons())
	{
		case Up:
			while(Button_Up_Pressed());
			screen_Monitoring_All_Sensor_Count ++;
			if (screen_Monitoring_All_Sensor_Count == MAX_TEMPERATURE_SENSOR_NUMBER) screen_Monitoring_All_Sensor_Count --;
			break;

		case Down:
			while(Button_Down_Pressed());
			if(screen_Monitoring_All_Sensor_Count != 0) screen_Monitoring_All_Sensor_Count --;
			break;

		case Back:
			while(Button_Back_Pressed());
			atApp_HMI.screen_Index = screen_Index_Setting;
			break;

		case Ok:
			while(Button_Ok_Pressed());
			atApp_HMI.screen_Index = screen_Index_Monitoring_Sensor;
			break;
		
		default:
			break;

	}
}
void App_HMI::screen_Monitoring_Sensor(){
	// LED 7seg
	char text[5];
	sprintf(text,"%doC ",atApp_HMI.Temperature_At_Sensor[screen_Monitoring_All_Sensor_Count]);
	_tm1637.setDisplayToString(text);	
	delay(1);
	// button
	switch (_tm1637.getButtons())
	{
		case Up:
			break;

		case Down:
			break;

		case Back:
			while(Button_Back_Pressed());
			atApp_HMI.screen_Index = screen_Index_Monitoring_All_Sensor;
			break;

		case Ok:
			break;
		
		default:
			break;

	}
}

void App_HMI::screen_Alarm(){
	// LED 7seg
	
	char text[5];
	if (atApp_HMI.Timer1_1s)
	{
		sprintf(text,"Cb%d ",atApp_HMI.Alarm_Sensor);
		_tm1637.setDisplayToString(text);	
	}
	else{
		sprintf(text,"%doC ",atApp_HMI.Max_Temperature);
		_tm1637.setDisplayToString(text);
	}
	delay(1);

	if(atApp_HMI.Alarm == alarm_Executing) atApp_HMI.Alarm = alarm_Done;
	
	// button
	switch (_tm1637.getButtons())
	{
		case Up:
			while(Button_Up_Pressed());
			atApp_HMI.screen_Index = screen_Index_Monitor;
			digitalWrite(19,LOW);
			break;

		case Down:
			while(Button_Down_Pressed());
			atApp_HMI.screen_Index = screen_Index_Monitor;
			digitalWrite(19,LOW);
			break;

		case Back:
			while(Button_Back_Pressed());
			atApp_HMI.screen_Index = screen_Index_Monitor;
			digitalWrite(19,LOW);
			break;

		case Ok:
			while(Button_Ok_Pressed());
			atApp_HMI.screen_Index = screen_Index_Monitor;
			digitalWrite(19,LOW);
			break;
		
		default:
			break;
	}
}

bool Button_Ok_Pressed(){
	return (_tm1637.getButtons() == Ok);
}
bool Button_Up_Pressed(){
	return (_tm1637.getButtons() == Up);
}
bool Button_Down_Pressed(){
	return (_tm1637.getButtons() == Down);
}
bool Button_Back_Pressed(){
	return (_tm1637.getButtons() == Back);
}
// ISR(TIMER1_OVF_vect)
// {
// 	atApp_HMI.Timer1_1s = !atApp_HMI.Timer1_1s;
// }
void atApp_HMI_Task_Func()
{
  while (1)
  {
    atApp_HMI.Run_Application(APP_RUN_MODE_AUTO);
    delay(50);
  }
}
#endif