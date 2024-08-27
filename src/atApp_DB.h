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

#ifndef _Application_DataBase_
#define _Application_DataBase_
/* _____PROJECT INCLUDES____________________________________________________ */
#include "App.h"
extern "C" {
  #include <hardware/sync.h>
  #include <hardware/flash.h>
};
/* _____DEFINITIONS__________________________________________________________ */

// Set the target offset to the last sector of flash


#define EEPROM_FIRST_BOOT   0
#define EEPROM_MODBUS_ID    1
#define EEPROM_Temperature_High_Limit 2

// Set the target offset to the last sector of flash
#define FLASH_TARGET_OFFSET             (PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE) // last sector of last block
#define FLASH_AD_Modbus_ID              (XIP_BASE + FLASH_TARGET_OFFSET + 0* FLASH_PAGE_SIZE)
#define FLASH_AD_Temperature_High_Limit (XIP_BASE + FLASH_TARGET_OFFSET + 1* FLASH_PAGE_SIZE)

/* _____GLOBAL VARIABLES_____________________________________________________ */


///////////////////////////////////////////////Testing part//
/* _____GLOBAL FUNCTION______________________________________________________ */
void atApp_DB_Task_Func();


/* _____CLASS DEFINITION_____________________________________________________ */
/**
 * This Application class is the application to manage the 
 */
class App_DB : public Application
{
public:
  	App_DB();
 	~App_DB();
  	static void  App_DB_Pend();
	static void  App_DB_Start();
	static void  App_DB_Restart();
	static void  App_DB_Execute();
	static void  App_DB_Suspend();
	static void  App_DB_Resume();	  
	static void  App_DB_End();

    int count = 0;
    int  Temperature_High_Limit   = 0;
    int Modbus_ID = 1;
protected:
    uint8_t  First_Boot             = 0;
    
private:
    int buf[FLASH_PAGE_SIZE/sizeof(int)] = {0};  // One page buffer of ints
    int *p, page, addr;
    int first_empty_page = -1;
} atApp_DB ;
/**
 * This function will be automaticaly called when a object is created by this class
 */
App_DB::App_DB(/* args */)
{
  	_Pend_User 	     = *App_DB_Pend;
	_Start_User 	 = *App_DB_Start;
	_Restart_User 	 = *App_DB_Restart;
	_Execute_User 	 = *App_DB_Execute;
	_Suspend_User	 = *App_DB_Suspend;
	_Resume_User	 = *App_DB_Resume;
	_End_User	     = *App_DB_End;

	// change the ID of application
	ID_Application = 2;
	// change the application name
	Name_Application = (char*)"Data Base";
	// change the ID of SNM
}
/**
 * This function will be automaticaly called when the object of class is delete
 */
App_DB::~App_DB()
{
	
}
/**
 * Pend to start is the first task of this application it will do prerequisite condition. In the debug mode, task will send information of application to terminal to start the application.
 */
void  App_DB::App_DB_Pend()
{
	
}
/**
 * This start function will init some critical function 
 */
void  App_DB::App_DB_Start(){
    
    //restore modbus ID
    atApp_DB.addr = FLASH_AD_Modbus_ID;
    atApp_DB.p = (int *)atApp_DB.addr;
    atApp_DB.Modbus_ID = *atApp_DB.p;

    // restore 
    atApp_DB.addr = FLASH_AD_Temperature_High_Limit;
    atApp_DB.p = (int *)atApp_DB.addr;
    atApp_DB.Temperature_High_Limit = *atApp_DB.p;
}
/**
 * Restart function of SNM  app
 */
void  App_DB::App_DB_Restart()
{
    
}
/**
 * Execute fuction of SNM app
 */

void  App_DB::App_DB_Execute()
{
    delay(2000);
    bool somethings_changed = 0;
    uint32_t ints ;
    // check change modbus ID
    atApp_DB.addr = FLASH_AD_Modbus_ID;
    atApp_DB.p = (int *)atApp_DB.addr;

    if (atApp_DB.Modbus_ID != *atApp_DB.p)
    {
        // write to buf
        *atApp_DB.buf = atApp_DB.Modbus_ID;
        // write buf to flash
        ints = save_and_disable_interrupts();
        flash_range_erase(FLASH_TARGET_OFFSET , FLASH_SECTOR_SIZE);
        restore_interrupts (ints);
        ints = save_and_disable_interrupts();
        flash_range_program(FLASH_TARGET_OFFSET + (0*FLASH_PAGE_SIZE), (uint8_t *)atApp_DB.buf, FLASH_PAGE_SIZE);
        restore_interrupts (ints);

        somethings_changed = 1;
    }
    // check change Temperature_High_Limit
    atApp_DB.addr = FLASH_AD_Temperature_High_Limit;
    atApp_DB.p = (int *)atApp_DB.addr;
    if (atApp_DB.Temperature_High_Limit != *atApp_DB.p)
    {
        // write to buf
        *atApp_DB.buf = atApp_DB.Temperature_High_Limit;
        // write buf to flash
        ints = save_and_disable_interrupts();
        flash_range_erase(FLASH_TARGET_OFFSET , FLASH_SECTOR_SIZE);
        restore_interrupts (ints);

        ints = save_and_disable_interrupts();
        flash_range_program(FLASH_TARGET_OFFSET + (1*FLASH_PAGE_SIZE), (uint8_t *)atApp_DB.buf, FLASH_PAGE_SIZE);
        restore_interrupts (ints);

        somethings_changed = 1;
    }

    // debug
    if (atApp_DB.User_Mode == APP_USER_MODE_DEBUG)
    {
        // Serial.println("    FLASH_PAGE_SIZE = " + String(FLASH_PAGE_SIZE, DEC));
        // Serial.println("    FLASH_SECTOR_SIZE = " + String(FLASH_SECTOR_SIZE,DEC));
        // Serial.println("    FLASH_BLOCK_SIZE = " + String(FLASH_BLOCK_SIZE, DEC));
        // Serial.println("    PICO_FLASH_SIZE_BYTES = " + String(PICO_FLASH_SIZE_BYTES, DEC));
        // Serial.println("    XIP_BASE = 0x" + String(XIP_BASE, HEX));

        if (somethings_changed) 
            Serial.println("    Something is changed");
        
        Serial.println("    Modbus ID : " + String(atApp_DB.Modbus_ID,DEC));
        Serial.println("    Temperature high limit  : " + String(atApp_DB.Temperature_High_Limit,DEC));
    }
}
void  App_DB::App_DB_Suspend(){}
void  App_DB::App_DB_Resume(){}	  
void  App_DB::App_DB_End(){}
void atApp_DB_Task_Func()
{
  while (1)
  {
    atApp_DB.Run_Application(APP_RUN_MODE_AUTO);
    delay(1);
  }
}
#endif