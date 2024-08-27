#include <Arduino.h>
#include "Scheduler.h"

// Applications 
#include "App.h"
#include "atApp_ABC.h"
#include "atApp_Blink.h"
#include "atApp_DS18B20.h"
#include "atApp_HMI.h"
#include "atApp_DB.h"
#include "atApp_CP.h"

void setup(void) {
  // atApp_ABC.Debug();
  // atApp_DS.Debug();
  atApp_DB.Debug();
  // atHMI.Debug();  
  // atApp_CP.Debug();

  // Scheduler.start(atApp_ABC_Task_Func);
  Scheduler.start(atApp_CP_Task_Func);
  Scheduler.start(atApp_BLINK_Task_Func);
  Scheduler.start(atApp_DS_Task_Func);
  Scheduler.start(atApp_HMI_Task_Func);
  Scheduler.start(atApp_DB_Task_Func);
}

void loop(void) {
}
    
