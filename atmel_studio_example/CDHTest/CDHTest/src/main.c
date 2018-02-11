#include <asf.h>
#include <FreeRTOS.h>
#include <portable.h>

#include "conf_board.h"
#include "partest.h"
#include "stdio_serial.h"

#include "PeriodicTaskRunner.h"
#include "SerialPrint.h"

#define BLINK_LED 0

xSemaphoreHandle printLock;
static void configure_console(void);

int main (void) {
	sysclk_init();
	board_init();
	configure_console();
		
	printLock = xSemaphoreCreateMutex();
	
	if (printLock != NULL) {

		vParTestInitialise();
	
		startPeriodicTasks();
		vTaskStartScheduler();
		
		printf("Insufficient RAM.");
	} else {
		
		printf("Error: Semaphore is NULL.");
	}
	return 0;
}

static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
			.baudrate = CONF_UART_BAUDRATE,
			.paritytype = CONF_UART_PARITY
			};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

int SerialRead(int default_value) {
	int result = default_value;
	char c = getchar();
	if (c == 'A') {
		result = 10;
	} else if (c == 'B') {
		result = 90;
	}
	return result;
}
void SerialPrint(const char * text) {
	if( xSemaphoreTake( printLock, ( TickType_t ) 1000 ) == pdTRUE )
    {
		printf(text);
		printf("\n\r");
        xSemaphoreGive( printLock );
    }
}
void SerialPrintInt(int text) {
	if( xSemaphoreTake( printLock, ( TickType_t ) 1000 ) == pdTRUE )
    {
		printf("%d", text);
        xSemaphoreGive( printLock );
    }
}

#if 0
void TaskWrite(void *params) {
	portTickType lastWakeTime;
	const portTickType frequency = 500 / portTICK_RATE_MS;

	lastWakeTime = xTaskGetTickCount();

	while(1) {
		vTaskDelayUntil(&lastWakeTime, frequency);
		vParTestToggleLED(BLINK_LED);
	}
}
#endif