#include <asf.h>
#include <FreeRTOS.h>
#include <portable.h>
#include "conf_board.h"
#include "PeriodicTaskRunner.h"
#include "partest.h"

#define BLINK_LED 0

int main (void) {
	sysclk_init();
	board_init();

	vParTestInitialise();
	startPeriodicTasks();
	vTaskStartScheduler();

	return 0;
}

void TaskWrite(void *params) {
	portTickType lastWakeTime;
	const portTickType frequency = 500 / portTICK_RATE_MS;

	lastWakeTime = xTaskGetTickCount();

	while(1) {
		vTaskDelayUntil(&lastWakeTime, frequency);
		vParTestToggleLED(BLINK_LED);
	}
}
