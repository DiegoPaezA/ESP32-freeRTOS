// FreeRTOS includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// I2C driver
#include "driver/i2c.h"

// Error library
#include "esp_err.h"

#define I2C_MASTER_RX_BUF_DISABLE 0
#define I2C_MASTER_TX_BUF_DISABLE 0
#define I2C_MASTER_FREQ_HZ 100000

// loop task
void loop_task(void *pvParameter)
{
    while(1) {
		vTaskDelay(1000 / portTICK_RATE_MS);
		    }
}


// Main application
void app_main() {

	printf("i2c scanner\r\n\r\n");

	// configure the i2c controller 0 in master mode, normal speed
	// https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html
	i2c_config_t conf;
	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = 21;
	conf.scl_io_num = 22;
	conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
	conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
	conf.master.clk_speed = I2C_MASTER_FREQ_HZ; //100000
	conf.clk_flags = 0; //(V4.4)  is 0, the clock allocator will select only according to the desired frequency.
	ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &conf));
	printf("- i2c controller configured\r\n");

	// install the driver
	ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0));
	printf("- i2c driver installed\r\n\r\n");

	printf("scanning the bus...\r\n\r\n");
	int devices_found = 0;

	for(int address = 1; address < 127; address++) {

		// create and execute the command link
		i2c_cmd_handle_t cmd = i2c_cmd_link_create();
		i2c_master_start(cmd);
		i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE, true);
		i2c_master_stop(cmd);
		if(i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS) == ESP_OK) {
			printf("-> found device with address 0x%02x\r\n", address);
			devices_found++;
		}
		i2c_cmd_link_delete(cmd);
	}
	if(devices_found == 0) printf("\r\n-> no devices found\r\n");
	printf("\r\n...scan completed!\r\n");


	// start the loop task
	xTaskCreate(&loop_task, "loop_task", 2048, NULL, 5, NULL);
}
