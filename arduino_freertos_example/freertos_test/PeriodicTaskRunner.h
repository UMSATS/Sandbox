#ifndef PERIODIC_TASK_RUNNER_H
#define PERIODIC_TASK_RUNNER_H

#include <Arduino_FreeRTOS.h>
#include "TaskWrite.h"

enum CDH_TASK_TABLE_INDICES
{
    HELLO_WORLD_1_IX,
    HELLO_WORLD_2_IX,
    TOTAL_NUMBER_OF_TASKS,
};

//we will eventually need this to stop tasks when power management wants to
typedef struct PeriodicTaskInfo {
	char *name;
	int priority;
	int stackDepth;
	TaskFunction_t taskFunction;
	void *taskParams;
	TaskHandle_t taskHandle;
} PeriodicTaskInfo;

const PeriodicTaskInfo CDH_PeriodicTaskTable[TOTAL_NUMBER_OF_TASKS] =
//  name,              priority,        stack depth,     task function,    task parameters, 		    task handle
{
  { (char *)"Hello World 1",  (UBaseType_t) 1,          128,   TaskWrite,               NULL,            	(TaskHandle_t) NULL  },
  { (char *)"Hello World 2",  (UBaseType_t) 1,          128,   TaskWrite,               NULL,             (TaskHandle_t) NULL  },
};

void startPeriodicTasks();

#endif
