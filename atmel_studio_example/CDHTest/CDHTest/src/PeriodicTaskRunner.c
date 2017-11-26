#include "PeriodicTaskRunner.h"

const PeriodicTaskInfo CDH_PeriodicTaskTable[TOTAL_NUMBER_OF_TASKS];

void _initializePeriodicTask(PeriodicTaskInfo *info, const signed char *name, int priority, int stackDepth, pdTASK_CODE taskFunction, void *taskParams, xTaskHandle taskHandle) {
	info->name = name;
	info->priority = priority;
	info->stackDepth = stackDepth;
	info->taskFunction = taskFunction;
	info->taskParams = taskParams;
	info->taskHandle = taskHandle;
}

void startPeriodicTasks(void) {
	_initializePeriodicTask(&CDH_PeriodicTaskTable[0], (const signed char *)"Hello World 1", 1, 128, TaskWrite, NULL, (xTaskHandle) NULL);
	//_initializePeriodicTask(&CDH_PeriodicTaskTable[1], (const signed char *)"Hello World 2", 1, 128, TaskWrite, NULL, (xTaskHandle) NULL);

	PeriodicTaskInfo info;
	for(int i = 0; i < TOTAL_NUMBER_OF_TASKS; i++) {
		info = CDH_PeriodicTaskTable[i];
		xTaskCreate(info.taskFunction, info.name, info.stackDepth, info.taskParams, info.priority, info.taskHandle);
	}
}
