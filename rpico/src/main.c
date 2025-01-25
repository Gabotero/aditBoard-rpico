#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/bootrom.h>

#include "hardware/pio.h"
#include "uart_rx.pio.h"

//RTOS//
#include <FreeRTOS.h>
#include <task.h>
////

#define LED_PIN 25

#define GPIO_ON 1
#define GPIO_OFF 0

// Async serial config
#define ASYNC_SERIAL_BAUD 3000000
#define ASYNC_SERIAL_RX_PIN 3

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) { // Called when stack is corrupted
	panic("Stack overflow. Task: %s\n", pcTaskName);
}

void vApplicationMallocFailedHook() { // Called when bad dynamic allocation.
	panic("malloc failed");
}


static void LED_Task(void *param) {

	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);

	while (true) {
		gpio_put(LED_PIN, GPIO_ON);
		vTaskDelay(500);
		gpio_put(PICO_DEFAULT_LED_PIN, GPIO_OFF);
		vTaskDelay(500);
	}

}

static void AsyncSerial_Task(void *param) {

	// Setup the state machine to receive data
	PIO pio = pio0;
	uint sm = 0; //State machine number. (There are 4 of them, connected to the same Instruction memory).
	uint offset = pio_add_program(pio, &uart_rx_program);
	uart_rx_program_init(pio, sm, offset, ASYNC_SERIAL_RX_PIN, ASYNC_SERIAL_BAUD);

	// Echo characters
	while(true) {

		char c = uart_rx_program_getc(pio, sm);
		printf("%d\n", ASYNC_SERIAL_BAUD);
		printf("%c", c);
	}

}

int main() {

	stdio_init_all();
	printf("Starting execution...");

	TaskHandle_t LED_Task_Handle = NULL; // Can use it later to stop it, restart it, etc.
	TaskHandle_t Serial_Async_Task_Handle = NULL;

	xTaskCreate(LED_Task, "LED Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, &LED_Task_Handle); //(nameOfFunction, taksName, stackSize, *parameters, priority, task_handle)
	xTaskCreate(AsyncSerial_Task, "Async Serial Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, &Serial_Async_Task_Handle);

	vTaskStartScheduler();

}


