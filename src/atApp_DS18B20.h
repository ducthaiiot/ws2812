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

#ifndef _Application_atApp_DS_
#define _Application_atApp_DS_
/* _____PROJECT INCLUDES____________________________________________________ */
#include "App.h"
/* _____DEFINITIONS__________________________________________________________ */
#define MAX_TEMPERATURE_SENSOR_NUMBER 20
/**
 * Dallas family thermometers access example (Arduino).
 *
 * Required configuration:
 * - @c CONFIG_SEARCH_ENABLED if @c SINGLE_SENSOR is not defined.
 */
#include "OneWireNg_CurrentPlatform.h"
#include "drivers/DSTherm.h"
#include "utils/Placeholder.h"

#define OW_PIN          18

/*
 * Set to true for parasitically powered sensors.
 */
#define PARASITE_POWER  false

/*
 * Uncomment for single sensor setup.
 *
 * With this scenario only one sensor device is allowed to be connected
 * to the bus. The library may be configured with 1-wire search activity
 * disabled to reduce its footprint.
 */
//#define SINGLE_SENSOR

/*
 * Uncomment for power provided by a switching
 * transistor and controlled by this pin.
 */
//#define PWR_CTRL_PIN    9

/*
 * Uncomment to set permanent, common resolution for all
 * sensors on the bus. Resolution may vary from 9 to 12 bits.
 */
//#define COMMON_RES      (DSTherm::RES_12_BIT)

#if !defined(SINGLE_SENSOR) && !CONFIG_SEARCH_ENABLED
# error "CONFIG_SEARCH_ENABLED is required for non SINGLE_SENSOR setup"
#endif

static Placeholder<OneWireNg_CurrentPlatform> _ow;

/* _____GLOBAL VARIABLES_____________________________________________________ */

///////////////////////////////////////////////Testing part//
/* _____GLOBAL FUNCTION______________________________________________________ */
void atApp_DS_Task_Func();
static void printScratchpad(const DSTherm::Scratchpad& scrpd);
static bool printId(const OneWireNg::Id& id);
/* _____CLASS DEFINITION_____________________________________________________ */
/**
 * This Application class is the application to manage the 
 */
class App_DS : public Application
{
public:
  	App_DS();
 	~App_DS();
  	static void  App_DS_Pend();
	static void  App_DS_Start();
	static void  App_DS_Restart();
	static void  App_DS_Execute();
	static void  App_DS_Suspend();
	static void  App_DS_Resume();
	static void  App_DS_End();
	// for store data for ever sensor
	float Temperature_At_Sensor[MAX_TEMPERATURE_SENSOR_NUMBER]; //in Celsius
	uint8_t Temp_Sen_Index = 0;
	uint8_t Present_Plugging_Sensor_Number = 0;
	float celsius, fahrenheit;
protected:
private:
} atApp_DS ;
/**
 * This function will be auto called when a object is created by this class
 */
App_DS::App_DS(/* args */)
{
  	_Pend_User 	     = *App_DS_Pend;
	_Start_User 	 = *App_DS_Start;
	_Restart_User 	 = *App_DS_Restart;
	_Execute_User 	 = *App_DS_Execute;
	_Suspend_User	 = *App_DS_Suspend;
	_Resume_User	 = *App_DS_Resume;
	_End_User	     = *App_DS_End;

	// change the ID of application
	ID_Application = 1;
	// change the application name
	Name_Application = (char*)"DS Application";
	
	// change the set all data to 0
	for (uint8_t i = 0; i < MAX_TEMPERATURE_SENSOR_NUMBER; i++)
	{
		atApp_DS.Temperature_At_Sensor[i] = 0;
	}
}
/**
 * This function will be auto called when the object of class is delete
 */
App_DS::~App_DS()
{
	
}
/**
 * Pend to start is the first task of this application it will do prerequisite condition. In the debug mode, task will send information of application to terminal to start the application.
 */
void  App_DS::App_DS_Pend()
{

}
/**
 * This start function will init some critical function 
 */
void  App_DS::App_DS_Start()
{

    new (&_ow) OneWireNg_CurrentPlatform(OW_PIN, false);

    DSTherm drv(_ow);

    Serial.begin(115200);

#if (CONFIG_MAX_SEARCH_FILTERS > 0)
    static_assert(CONFIG_MAX_SEARCH_FILTERS >= DSTherm::SUPPORTED_SLAVES_NUM,
        "CONFIG_MAX_SEARCH_FILTERS too small");

    drv.filterSupportedSlaves();
#endif

#ifdef COMMON_RES
    /*
     * Set common resolution for all sensors.
     * Th, Tl (high/low alarm triggers) are set to 0.
     */
    drv.writeScratchpadAll(0, 0, COMMON_RES);

    /*
     * The configuration above is stored in volatile sensors scratchpad
     * memory and will be lost after power unplug. Therefore store the
     * configuration permanently in sensors EEPROM.
     */
    drv.copyScratchpadAll(PARASITE_POWER);
#endif
}  
/**
 * Restart function of SNM  app
 */
void  App_DS::App_DS_Restart()
{

}
/**
 * Execute fuction of SNM app
 */
void  App_DS::App_DS_Execute()
{	
	OneWireNg& ow = _ow;
    DSTherm drv(ow);
    Placeholder<DSTherm::Scratchpad> _scrpd;

    /* convert temperature on all sensors connected... */
    drv.convertTempAll(DSTherm::SCAN_BUS, PARASITE_POWER);

    /* read sensors one-by-one */
    for (const auto& id: ow) {
        if (printId(id)) {
            if (drv.readScratchpad(id, &_scrpd) == OneWireNg::EC_SUCCESS)
                printScratchpad(_scrpd);
            else
                Serial.println("  Read scratchpad error.");
        }
    }
    Serial.println("----------");
	atApp_DS.Present_Plugging_Sensor_Number = atApp_DS.Temp_Sen_Index;
	atApp_DS.Temp_Sen_Index = 0;

	if(atApp_DS.User_Mode == APP_USER_MODE_DEBUG)
	{
		for (uint8_t i = 0; i < atApp_DS.Present_Plugging_Sensor_Number; i++)
		{
			Serial.print("Temperature Sensor ");
			Serial.print(i);
			Serial.print(" : ");
			Serial.print(atApp_DS.Temperature_At_Sensor[i]);
			Serial.println(" oC ");
		}
	}
}
void  App_DS::App_DS_Suspend(){}
void  App_DS::App_DS_Resume(){}	  
void  App_DS::App_DS_End(){}
void atApp_DS_Task_Func()
{
  while (1)
  {
    atApp_DS.Run_Application(APP_RUN_MODE_AUTO);
    delay(1000);
  }
}
/* returns false if not supported */
static bool printId(const OneWireNg::Id& id)
{
    const char *name = DSTherm::getFamilyName(id);

    Serial.print(id[0], HEX);
    for (size_t i = 1; i < sizeof(OneWireNg::Id); i++) {
        Serial.print(':');
        Serial.print(id[i], HEX);
    }
    if (name) {
        Serial.print(" -> ");
        Serial.print(name);
    }
    Serial.println();

    return (name != NULL);
}

static void printScratchpad(const DSTherm::Scratchpad& scrpd)
{
    const uint8_t *scrpd_raw = scrpd.getRaw();

    Serial.print("  Scratchpad:");
    for (size_t i = 0; i < DSTherm::Scratchpad::LENGTH; i++) {
        Serial.print(!i ? ' ' : ':');
        Serial.print(scrpd_raw[i], HEX);
    }

    Serial.print("; Th:");
    Serial.print(scrpd.getTh());

    Serial.print("; Tl:");
    Serial.print(scrpd.getTl());

    Serial.print("; Resolution:");
    Serial.print(9 + (int)(scrpd.getResolution() - DSTherm::RES_9_BIT));

    long temp = scrpd.getTemp();
    Serial.print("; Temp:");
    if (temp < 0) {
        temp = -temp;
        Serial.print('-');
    }
    Serial.print(temp / 1000);
    Serial.print('.');
    Serial.print(temp % 1000);
    Serial.print(" C");
	
    Serial.println();

	atApp_DS.Temperature_At_Sensor[atApp_DS.Temp_Sen_Index] = (float)temp / (float)1000;
	atApp_DS.Temp_Sen_Index ++;
}
#endif