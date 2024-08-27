/**
@file

*/
/*
  Application example t

  Library:: 

  This version is

  Copyright:: 2021 nguyentrinhtuan1996@gmail.com
*/

#ifndef _Application_atApp_BLINK_
#define _Application_atApp_BLINK_
/* _____PROJECT INCLUDES____________________________________________________ */
#include "App.h"
/* _____DEFINITONS__________________________________________________________ */
#define LED_PIN 25
/* _____GLOBAL VARIABLES_____________________________________________________ */ 
void atApp_BLINK_Task_Func();
///////////////////////////////////////////////Testing part//
/* _____GLOBAL FUNCTION______________________________________________________ */

/* _____CLASS DEFINITION_____________________________________________________ */

/**
 * This Application class is the application to manage the 
 */
class App_BLINK : public Application
{
public:
  	App_BLINK();
 	~App_BLINK();
	bool Led_State = 0;
protected:
private:
  	static void  App_BLINK_Pend();
	static void  App_BLINK_Start();
	static void  App_BLINK_Restart();
	static void  App_BLINK_Execute();
	static void  App_BLINK_Suspend();
	static void  App_BLINK_Resume();	  
	static void  App_BLINK_End();
} atApp_BLINK ;
/**
 * This function will be automaticaly called when a object is created by this class
 */
App_BLINK::App_BLINK(/* args */)
{
  	_Pend_User 	     = *App_BLINK_Pend;
	_Start_User 	 = *App_BLINK_Start;
	_Restart_User 	 = *App_BLINK_Restart;
	_Execute_User 	 = *App_BLINK_Execute;
	_Suspend_User	 = *App_BLINK_Suspend;
	_Resume_User	 = *App_BLINK_Resume;
	_End_User	     = *App_BLINK_End;

	// change the ID of application
	ID_Application = 1;
	// change the application name
	Name_Application = (char*)"BLINK Application";
	// change the ID of SNM
}
/**
 * This function will be automaticaly called when the object of class is delete
 */
App_BLINK::~App_BLINK()
{
	
}
/**
 * Pend to start is the first task of this application it will do prerequisite condition. In the debit mode, task will send information of application to terminal to start the application.
 */
void  App_BLINK::App_BLINK_Pend()
{
    // atService_XYZ.Debug();
}
/**
 * This start function will init some critical function 
 */
void  App_BLINK::App_BLINK_Start()
{
	pinMode(LED_PIN,OUTPUT);
}  
/**
 * Restart function of SNM  app
 */
void  App_BLINK::App_BLINK_Restart()
{

}
/**
 * Execute fuction of SNM app
 */
void  App_BLINK::App_BLINK_Execute()
{	
	// digitalWrite(LED_PIN,atApp_BLINK.Led_State++);
	if(atApp_BLINK.Led_State == 0)
	{
		digitalWrite(LED_PIN,HIGH);
		atApp_BLINK.Led_State = 1;
	}
	else
	{
		digitalWrite(LED_PIN,LOW);
		atApp_BLINK.Led_State = 0;
	}
    if(atApp_BLINK.User_Mode == APP_USER_MODE_DEBUG)
    {
		
    }   
}
void  App_BLINK::App_BLINK_Suspend(){}
void  App_BLINK::App_BLINK_Resume(){}	  
void  App_BLINK::App_BLINK_End(){}
void atApp_BLINK_Task_Func()
{
  while (1)
  {
    atApp_BLINK.Run_Application(APP_RUN_MODE_AUTO);
    delay(1000);
  }
}
#endif