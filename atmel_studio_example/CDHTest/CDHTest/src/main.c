/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <FreeRTOS.h>
#include <portable.h>
#include "conf_board.h"
#include "PeriodicTaskRunner.h"

static void configureConsole(void);

int main (void) {
	sysclk_init();
	board_init();

	configureConsole();

	startPeriodicTasks();
	vTaskStartScheduler();

	return 0;
}

void TaskWrite(void *params) {
	portTickType lastWakeTime;
	//const portTickType frequency = 1000 / portTICK_RATE_MS; //run every 10 ms
	const portTickType frequency = 1;

	lastWakeTime = xTaskGetTickCount();

	while(1) {
		//vTaskDelay(10);
		//vTaskDelayUntil(&lastWakeTime, frequency);
		printf("Hello world\n");
		printf("Hello again\n");
	}
}



static void configureConsole(void) {
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
#if (defined CONF_UART_CHAR_LENGTH)
		.charlength = CONF_UART_CHAR_LENGTH,
#endif
		.paritytype = CONF_UART_PARITY,
#if (defined CONF_UART_STOP_BITS)
		.stopbits = CONF_UART_STOP_BITS,
#endif
	};

	/* Configure console UART. */
	stdio_serial_init(CONF_UART, &uart_serial_options);

	/* Specify that stdout should not be buffered. */
#if defined(__GNUC__)
	setbuf(stdout, NULL);
#else
	/* Already the case in IAR's Normal DLIB default configuration: printf()
	 * emits one character at a time.
	 */
#endif
}
