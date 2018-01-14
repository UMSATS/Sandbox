#ifndef PERIODIC_TASK_RUNNER_H
#define PERIODIC_TASK_RUNNER_H

#include "TaskWrite.h"
#include <FreeRTOS.h>
#include <task.h>


// ----------------------- DEFINES ---------------------------------------------------------------
// The power is assumed to be some value between 0 and 100. If a task's "powerPriority" falls below
// one of these threshholds, it will be suspended.
#define ALWAYS_ON    0
#define SOMETIMES_ON 25
#define RARELY_ON    50
#define NEVER_ON     100

// These are typedefs/defines present in FreeRTOS v8.0 but not present in FreeRTOS v7.0. 
// TODO: Use the most recent FreeRTOS for this project OR change this project to use FreeRTOS v7.0 API.
typedef pdTASK_CODE TaskFunction_t;
typedef xTaskHandle TaskHandle_t;
typedef portTickType TickType_t;
typedef unsigned long UBaseType_t;
#define pdMS_TO_TICKS( xTimeInMs ) ( ( TickType_t ) ( ( ( TickType_t ) ( xTimeInMs ) * ( TickType_t ) configTICK_RATE_HZ ) / ( TickType_t ) 1000 ) )


// ----------------------- ENUMERATIONS AND ENUMERATION TYPEDEFS ---------------------------------
// Register your periodic task(s) at the cdh task table in PeriodicTaskRunner.c.
enum CDH_TASK_TABLE_INDICES
{
	HELLO_WORLD_HIGH_IX,
	HELLO_WORLD_LOW_IX,
	CHANGE_MOCK_POWER_IX,
	TOTAL_NUMBER_OF_TASKS,
};

// ----------------------- STRUCTURES AND STRUCTURE TYPEDEFS -------------------------------------
// We will eventually need this to stop tasks when power management wants to.
typedef struct {
	const signed char *name;
	int priority;
	int stackDepth;
	TaskFunction_t taskFunction;
	void *taskParams;
	TaskHandle_t taskHandle;
	int powerPriority;  // http://www.freertos.org/RTOS-task-states.html
} TaskInfo;

// ----------------------- EXTERNS ---------------------------------------------------------------
extern TaskInfo CDH_PeriodicTaskTable[TOTAL_NUMBER_OF_TASKS];
extern int mock_power;

// ----------------------- FUNCTION PROTOTYPES ---------------------------------------------------
void startPeriodicTasks(void);
void changeMockPower(void *pvParams);

#endif