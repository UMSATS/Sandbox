// UMSATS - CDH Scheduler 
//
// File Description:
//   Contains the mock tasks for the scheduler.
//
// History
// 2018-05-12 by Tamkin Rahman
// - Removed power monitor flags.

// -----------------------------------------------------------------------------------------------
// ----------------------- INCLUDES --------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
#include "TaskWrite.h"

#include "PeriodicTaskRunner.h"
#include "PowerMonitor.h"
#include "SerialPrint.h"

// -----------------------------------------------------------------------------------------------
// ----------------------- FUNCTIONS -------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
// Example of a periodic task.
void TaskWrite_low_priority(void *params) {
  UNUSED(params);
  
  TickType_t lastWakeTime;
  const TickType_t frequency = pdMS_TO_TICKS( 5000 ); // Run every 1000 ms.
   
  for(;;) 
  {
    lastWakeTime = xTaskGetTickCount();
	  SerialPrint("low priority hello world\n");
	
	  vTaskDelayUntil(&lastWakeTime, frequency);
  }
}

// --------------------------------------------------------------------------------
void TaskWrite_high_priority(void *params) {
  UNUSED(params);
  
  TickType_t lastWakeTime;
  const TickType_t frequency = pdMS_TO_TICKS( 5000 ); // Run every 1000 ms.

  for(;;) {
    lastWakeTime = xTaskGetTickCount();
	
    SerialPrint("high priority hello world\n");
    vTaskDelayUntil(&lastWakeTime, frequency);
  }
}

// --------------------------------------------------------------------------------
// Mock function to set the power level for the power monitor.
void changeMockPower(void *params) {
  UNUSED(params);
    
  TickType_t lastWakeTime;
  const TickType_t frequency = pdMS_TO_TICKS( 100 ); // Run every 100 ms.
  int valueRead = 0;
  
  while(1) {
	lastWakeTime = xTaskGetTickCount();  
	vTaskDelayUntil(&lastWakeTime, frequency);	 
    
  valueRead = SerialRead(powerReading);
    
  if ( valueRead != powerReading ) {
    if ( (valueRead > POWER_MIN) && (valueRead <= POWER_MAX) ){
        powerReading = valueRead;
        SerialPrint("Changed the power to ");
        SerialPrintInt(powerReading);
	      SerialPrint("\n");
      }
    }
  }
}

