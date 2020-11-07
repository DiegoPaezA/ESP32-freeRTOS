// FreeRTOS includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "string.h"
// UART driver
#include "driver/uart.h"
// Error library
#include "esp_err.h"

/**
 * En este ejemplo se configura el Puerto UART2 y se Envía la palabra OK para evaluarla con
 * Analizador lógico.
 *
 * - Port: UART2
 * - Receive (Rx) buffer: on
 * - Transmit (Tx) buffer: off
 * - Flow control: off
 * - Event queue: off
 * - Pin assignment: see defines below
 */

#define BUF_SIZE (1024)
#define TXD_PIN (GPIO_NUM_17)
#define RXD_PIN (GPIO_NUM_16)

// Main application
void app_main() {

	// configure the UART2 controller
	uart_config_t uart_config = {
			.baud_rate = 9600,
			.data_bits =UART_DATA_8_BITS,
			.parity = UART_PARITY_DISABLE,
			.stop_bits = UART_STOP_BITS_1,
			.flow_ctrl = UART_HW_FLOWCTRL_DISABLE };

	uart_param_config(UART_NUM_2, &uart_config);
	uart_set_pin(UART_NUM_2, TXD_PIN, RXD_PIN,UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
	uart_driver_install(UART_NUM_2, BUF_SIZE * 2, 0, 0, NULL, 0);

	char* test_str = "Ok\n";

	while (1) {
		uart_write_bytes(UART_NUM_2, (const char*)test_str, strlen(test_str));
		vTaskDelay(10/ portTICK_RATE_MS);
	}

}

