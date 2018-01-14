#include "PeriodicTaskRunner.h"
#include "PowerMonitor.h"

// ----------------------- VARIABLES -------------------------------------

  /*  Registering a task in this table means your periodic task should follow this template:
   *  
   *  void TaskToRun(void * params)
   *  {
   *    TickType_t lastWakeTime;
   *    const TickType_t frequency = pdMS_TO_TICKS( 1000 (desired period (ms)) );
   *
   *    for(;;) {
   *      lastWakeTime = xTaskGetTickCount();
   *      // Place your task code here...
   *      vTaskDelayUntil(&lastWakeTime, frequency);
   *    }
   *   }
   *  }
   *   
   *   
   *  According to https://www.freertos.org/RTOS-task-states.html, if a task becomes blocked and then becomes
   *  suspended, when the task is resumed, it enters the "Ready" or "Running" state. When vTaskDelayUntil is used
   *  we specific our current tick count and frequency, and our "lastwaketime" variable gets set to our target wake
   *  tick count. If we suspend for some time, then our target tick count has long passed, so vTaskDelayUntil will
   *  return immediately because we've specified a wake time that is in the past. This will occur until "lastWakeTime"
   *  is specified to a value in the future, at which point the task will delay like normal.
   *  
   *  There are 2 ways around this issue, 1 is to get the tick count on each iteration of the loop, and the other is 
   *  to use vTaskDelay. The first way is chosen so that the task will run as close to "frequency" as possible.
    */

TaskInfo CDH_PeriodicTaskTable[TOTAL_NUMBER_OF_TASKS] =
//				                      name,             priority,  stack depth,               task function,    task parameters,                      task handle,   power priority
{
  { (const signed char *)"Hello World high",     (UBaseType_t) 1,          128,      TaskWrite_high_priority,              NULL,            (TaskHandle_t) NULL,  ALWAYS_ON    },
  { (const signed char *)"Hello World low",      (UBaseType_t) 1,          128,      TaskWrite_low_priority,               NULL,            (TaskHandle_t) NULL,  SOMETIMES_ON },
  { (const signed char *)"Change Mock Power",    (UBaseType_t) 1,          128,      changeMockPower,                      NULL,            (TaskHandle_t) NULL,  ALWAYS_ON    },
};


// ----------------------- FUNCTIONS -------------------------------------
void startPeriodicTasks() {
	for(int i = 0; i < TOTAL_NUMBER_OF_TASKS; i++) {
		xTaskCreate(CDH_PeriodicTaskTable[i].taskFunction, CDH_PeriodicTaskTable[i].name, CDH_PeriodicTaskTable[i].stackDepth, CDH_PeriodicTaskTable[i].taskParams, CDH_PeriodicTaskTable[i].priority, &CDH_PeriodicTaskTable[i].taskHandle);
	}
  PowerMonitorInit();
}
