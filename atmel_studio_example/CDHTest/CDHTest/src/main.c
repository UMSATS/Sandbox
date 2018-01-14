#include <asf.h>
#include <FreeRTOS.h>
#include <portable.h>
#include "stdio_serial.h"
#include "conf_board.h"
#include "PeriodicTaskRunner.h"
#include "partest.h"

#define BLINK_LED 0

static void configureConsole(void);

int count;
int main (void) {
	sysclk_init();
	board_init();
	configureConsole();
	
	count = 0;
	puts("Starting\r\n");
	
	vParTestInitialise();
	startPeriodicTasks();
	vTaskStartScheduler();

	return 0;
}

static void configureConsole(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.paritytype = CONF_UART_PARITY
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

void TaskCount(void *params) {
	portTickType lastWakeTime;
	const portTickType frequency = 1 / portTICK_RATE_MS;

	lastWakeTime = xTaskGetTickCount();
	
	while(1) {
		vTaskDelayUntil(&lastWakeTime, frequency);
		count++;
	}
}

void TaskWrite(void *params) {
	portTickType lastWakeTime;
	const portTickType frequency = 1000 / portTICK_RATE_MS;

	lastWakeTime = xTaskGetTickCount();
	
	while(1) {
		vTaskDelayUntil(&lastWakeTime, frequency);
		printf("Count: %d\r\n", count);
	}
}

void TaskBlink(void *params) {
	portTickType lastWakeTime;
	const portTickType frequency = 500 / portTICK_RATE_MS;

	lastWakeTime = xTaskGetTickCount();

	while(1) {
		vTaskDelayUntil(&lastWakeTime, frequency);
		vParTestToggleLED(BLINK_LED);
	}
}
