#ifndef PERIODIC_TASK_RUNNER_H
#define PERIODIC_TASK_RUNNER_H
// UMSATS - CDH Scheduler 
//
// File Description:
//   Contains definitions and prototypes for implementing periodic tasks.
//
// History
// 2018-05-12 by Tamkin Rahman
// - Added CAN and Cpu monitoring tasks.
// 2018-06-03 by Tamkin Rahman
// - Added Time Delay Task manager task.

// -----------------------------------------------------------------------------------------------
// ----------------------- INCLUDES --------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
#include "Generic_FreeRTOS.h"
#include "TaskWrite.h"

// -----------------------------------------------------------------------------------------------
// ----------------------- DEFINES ---------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
// The power is assumed to be some value between 0 and 100. If a task's "powerPriority" falls below
// one of these thresholds, it will be suspended.
#define ALWAYS_ON    0
#define SOMETIMES_ON 25
#define RARELY_ON    50
#define NEVER_ON     100

// Used if this file is using Atmel Studio.
#ifdef ATMEL_STUDIO
  // These are typedefs/defines present in FreeRTOS v8.0 but not present in FreeRTOS v7.0. 
  // TODO: Use the most recent FreeRTOS for this project OR change this project to use FreeRTOS v7.0 API.
  typedef pdTASK_CODE TaskFunction_t;
  typedef xTaskHandle TaskHandle_t;
  typedef portTickType TickType_t;
  typedef unsigned long UBaseType_t;
  #define pdMS_TO_TICKS( xTimeInMs ) ( ( TickType_t ) ( ( ( TickType_t ) ( xTimeInMs ) * ( TickType_t ) configTICK_RATE_HZ ) / ( TickType_t ) 1000 ) )
#endif

#define UNUSED(x) (void)(x)

// -----------------------------------------------------------------------------------------------
// ----------------------- ENUMERATIONS AND ENUMERATION TYPEDEFS ---------------------------------
// -----------------------------------------------------------------------------------------------
// Register your periodic task(s) at the cdh task table in PeriodicTaskRunner.c.
enum CDH_TASK_TABLE_INDICES
{
	HELLO_WORLD_HIGH_IX,
	HELLO_WORLD_LOW_IX,
	CHANGE_MOCK_POWER_IX,
  CAN_MANAGER_IX,
  CAN_MONITOR_IX,
  TIME_DELAYED_TASK_MANAGER_IX,
  CPU_MONITOR_IX,
	TOTAL_NUMBER_OF_TASKS,
};

// -----------------------------------------------------------------------------------------------
// ----------------------- STRUCTURES AND STRUCTURE TYPEDEFS -------------------------------------
// -----------------------------------------------------------------------------------------------
// We will eventually need this to stop tasks when power management wants to.
typedef struct {
	const char *name;
	int priority;
	int stackDepth;
	TaskFunction_t taskFunction;
	void *taskParams;
	TaskHandle_t taskHandle;
	int powerPriority;  // http://www.freertos.org/RTOS-task-states.html
} TaskInfo;

// -----------------------------------------------------------------------------------------------
// ----------------------- EXTERNS ---------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
extern TaskInfo CDH_PeriodicTaskTable[TOTAL_NUMBER_OF_TASKS];

// -----------------------------------------------------------------------------------------------
// ----------------------- FUNCTION PROTOTYPES ---------------------------------------------------
// -----------------------------------------------------------------------------------------------
void startPeriodicTasks(void);
void changeMockPower(void *pvParams);

#endif
