// FreeRTOS includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
// UART driver
#include "driver/uart.h"

// Error library
#include "esp_err.h"

/**
 * Todos los datos recibidos en el UART2 se envían al puerto serial principal.
 * with hardware flow control turned off. It does not use UART driver event queue.
 *
 * - Port: UART2
 * - Receive (Rx) buffer: on
 * - Transmit (Tx) buffer: off
 * - Flow control: off
 * - Event queue: off
 * - Pin assignment: see defines below
 */
#define BUF_SIZE (1024 * 2)
#define TXD_PIN (GPIO_NUM_17)
#define RXD_PIN (GPIO_NUM_16)

// Main application
void app_main() {

	printf("UART echo\r\n\r\n");

	// configure the UART2 controller
	uart_config_t uart_config = {
			.baud_rate = 115200,
			.data_bits =UART_DATA_8_BITS,
			.parity = UART_PARITY_DISABLE,
			.stop_bits = UART_STOP_BITS_1,
			.flow_ctrl = UART_HW_FLOWCTRL_DISABLE };

	uart_param_config(UART_NUM_2, &uart_config);
	uart_set_pin(UART_NUM_2, TXD_PIN, RXD_PIN,UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
	uart_driver_install(UART_NUM_2, BUF_SIZE, 0, 0, NULL, 0);

	uart_write_bytes(UART_NUM_2, "Ready!\r\n", 8);
	// Configure a temporary buffer for the incoming data
	uint8_t *data = (uint8_t*) malloc(1024);
	while (1) {
		// Read data from the UART
	        int len = uart_read_bytes(UART_NUM_2, data, BUF_SIZE, (20/portTICK_RATE_MS));
			// Write data back to the UART
	        if(len > 0) {
				data[len] = '\0';
				printf("%s", data);
				fflush(stdout);
			}
	}
}

