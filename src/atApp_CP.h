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

#ifndef _Application_atApp_CP_
#define _Application_atApp_CP_
/* _____PROJECT INCLUDES____________________________________________________ */
#include "App.h"
#include "atApp_DB.h"
#include "atApp_HMI.h"
#include "atApp_DS18B20.h"
#include "atApp_Blink.h"
/* _____DEFINITIONS__________________________________________________________ */

/* _____GLOBAL VARIABLES_____________________________________________________ */

///////////////////////////////////////////////Testing part//
/* _____GLOBAL FUNCTION______________________________________________________ */
void atApp_CP_Task_Func();
/* _____CLASS DEFINITION_____________________________________________________ */
/**
 * This Application class is the application to manage the 
 */
class App_CP : public Application
{
public:
  	App_CP();
 	~App_CP();
  	static void  App_CP_Pend();
	static void  App_CP_Start();
	static void  App_CP_Restart();
	static void  App_CP_Execute();
	static void  App_CP_Suspend();
	static void  App_CP_Resume();	  
	static void  App_CP_End();
protected:
private:
} atApp_CP ;
/**
 * This function will be auto called when a object is created by this class
 */
App_CP::App_CP(/* args */)
{
  	_Pend_User 	     = *App_CP_Pend;
	_Start_User 	 = *App_CP_Start;
	_Restart_User 	 = *App_CP_Restart;
	_Execute_User 	 = *App_CP_Execute;
	_Suspend_User	 = *App_CP_Suspend;
	_Resume_User	 = *App_CP_Resume;
	_End_User	     = *App_CP_End;

	// change the ID of application
	ID_Application = 4;
	// change the application name
	Name_Application = (char*)"Core processing Application";
	// change the ID of SNM
}
/**
 * This function will be auto called when the object of class is delete
 */
App_CP::~App_CP()
{
	
}
/**
 * Pend to start is the first task of this application it will do prerequisite condition. In the debug mode, task will send information of application to terminal to start the application.
 */
void  App_CP::App_CP_Pend()
{

}
/**
 * This start function will init some critical function 
 */
void  App_CP::App_CP_Start()
{
    atApp_HMI.Max_Temperature_Limit = atApp_DB.Temperature_High_Limit;
	pinMode(19,OUTPUT);
}  
/**
 * Restart function of SNM  app
 */
void  App_CP::App_CP_Restart()
{

}
/**
 * Execute fuction of SNM app
 */
void  App_CP::App_CP_Execute()
{
	atApp_HMI.Timer1_1s = !atApp_HMI.Timer1_1s;
	
	atApp_HMI.Max_Temperature = atApp_DS.Temperature_At_Sensor[0];

	// find the highest temperature
	for (uint8_t i = 0; i < MAX_TEMPERATURE_SENSOR_NUMBER; i++)
	{
		if(atApp_HMI.Max_Temperature < atApp_DS.Temperature_At_Sensor[i])
		{
			atApp_HMI.Max_Temperature = atApp_DS.Temperature_At_Sensor[i];
			atApp_HMI.Alarm_Sensor = i;
		}
		atApp_HMI.Temperature_At_Sensor[i] = atApp_DS.Temperature_At_Sensor[i];
	}
	
	// update into database
	atApp_DB.Temperature_High_Limit = atApp_HMI.Max_Temperature_Limit;

	// alarm when over temperature
	if (atApp_HMI.Max_Temperature >=  atApp_HMI.Max_Temperature_Limit)
	{
		if(atApp_HMI.Alarm == alarm_Not_Yet)
		{
			atApp_HMI.Alarm = alarm_Executing;
			digitalWrite(19,HIGH); // turn on alarm executor 
		}
	}
	else
	{
	// 	digitalWrite(15,LOW);
		atApp_HMI.Alarm = alarm_Not_Yet;
	}
	
    if(atApp_CP.User_Mode == APP_USER_MODE_DEBUG)
    {
		
    }   
}
void  App_CP::App_CP_Suspend(){}
void  App_CP::App_CP_Resume(){}	  
void  App_CP::App_CP_End(){}
void atApp_CP_Task_Func()
{
  while (1)
  {
    atApp_CP.Run_Application(APP_RUN_MODE_AUTO);
    delay(1000);
  }
}
#endif