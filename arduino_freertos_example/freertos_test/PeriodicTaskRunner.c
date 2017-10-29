#include "PeriodicTaskRunner.h"

void startPeriodicTasks() {
	PeriodicTaskInfo info;
	for(int i = 0; i < TOTAL_NUMBER_OF_TASKS; i++) {
		info = CDH_PeriodicTaskTable[i];
		xTaskCreate(info.taskFunction, info.name, info.stackDepth, info.taskParams, info.priority, info.taskHandle);
	}
}