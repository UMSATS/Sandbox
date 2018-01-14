#include "PeriodicTaskRunner.h"
#include "SerialPrint.h"
#include "TaskWrite.h"


// Example of a periodic task.
void TaskWrite_low_priority(void *params) {
  TickType_t lastWakeTime;
  const TickType_t frequency = pdMS_TO_TICKS( 1000 ); // Run every 1000 ms.
   
  for(;;) {
    lastWakeTime = xTaskGetTickCount();
    SerialPrint("low priority hello world");
    vTaskDelayUntil(&lastWakeTime, frequency);
  }
}

void TaskWrite_high_priority(void *params) {
  TickType_t lastWakeTime;
  const TickType_t frequency = pdMS_TO_TICKS( 1000 ); // Run every 1000 ms.

  for(;;) {
    lastWakeTime = xTaskGetTickCount();
    SerialPrint("high priority hello world");
    vTaskDelayUntil(&lastWakeTime, frequency);
  }
}

// Mock function to set the power level for the power monitor.
void changeMockPower(void *params) {
  TickType_t lastWakeTime;
  const TickType_t frequency = pdMS_TO_TICKS( 100 ); // Run every 100 ms.
  int valueRead = 0;
  
  while(1) {
    lastWakeTime = xTaskGetTickCount();  
    
    valueRead = SerialRead(mock_power);
    
    if ( valueRead != mock_power ) {
      if (   (valueRead > 0) 
          && (valueRead <= 100)){
        mock_power = valueRead;
        SerialPrint("Changed the mock power to ");
        SerialPrintInt(mock_power);
		SerialPrint("\n");
      }
    }
    vTaskDelayUntil(&lastWakeTime, frequency);
  }
}
