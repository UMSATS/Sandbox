#ifndef PERIODIC_TASK_RUNNER_H
#define PERIODIC_TASK_RUNNER_H

#include "TaskWrite.h"
#include <FreeRTOS.h>
#include <task.h>

enum CDH_TASK_TABLE_INDICES
{
    BLINK_TASK,
    WRITE_TASK,
	COUNT_TASK,
    TOTAL_NUMBER_OF_TASKS,
};

//we will eventually need this to stop tasks when power management wants to
typedef struct PeriodicTaskInfo {
	const signed char *name;
	int priority;
	int stackDepth;
	pdTASK_CODE taskFunction;
	void *taskParams;
	xTaskHandle taskHandle;
} PeriodicTaskInfo;

void startPeriodicTasks(void);

#endif
