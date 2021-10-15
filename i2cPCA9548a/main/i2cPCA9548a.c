/*
 * Ejemplo I2C: Se utiliza el multiplexador I2C PCA9548 y Para
 * mostrar el funcionamiento del protocolo I2C.
 *
 */

// FreeRTOS includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "esp_err.h"

#define PCA9548ADDR   0x70  // W   ADDRESS MUX
// PCA9548 CHANNELS
#define  PCA9548_OFF  0x00  // W   CHANNELS OFF
#define  PCA9548_CH1  0x01  // W   ACTIVATION OF CHANNEL 1
#define  PCA9548_CH2  0x02  // W   ACTIVATION OF CHANNEL 2
#define  PCA9548_CH3  0x04  // W   ACTIVATION OF CHANNEL 3
#define  PCA9548_CH4  0x08  // W   ACTIVATION OF CHANNEL 4
#define  PCA9548_CH5  0x10  // W   ACTIVATION OF CHANNEL 5
#define  PCA9548_CH6  0x20  // W   ACTIVATION OF CHANNEL 6
#define  PCA9548_CH7  0x40  // W   ACTIVATION OF CHANNEL 7
#define  PCA9548_CH8  0x80  // W   ACTIVATION OF CHANNEL 8


#define ACK_VAL    0x0
#define NACK_VAL   0x1
#define I2C_MASTER_FREQ_HZ 100000

void selectI2CChannel(int muxADD, int channelADD) {

	// create and execute the command link
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd,(muxADD << 1) | I2C_MASTER_WRITE,true);
	i2c_master_write_byte(cmd,channelADD,true);
	i2c_master_stop(cmd);
	if(i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS) == ESP_OK) {
		printf("-> Seleccionado Canal 0x%02x del Mux I2C \r\n", channelADD);
	}
	i2c_cmd_link_delete(cmd);

}

void i2c_scan(void){
	int devices_found = 0;
	printf("Iniciando el Multiplexado \r\n\r\n");
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
		printf("\r...scan completed!\r\n");
}
// loop task
void loop_task(void *pvParameter)
{
	esp_log_level_set("I2C", ESP_LOG_INFO);
	i2c_scan();
 	selectI2CChannel(PCA9548ADDR,PCA9548_CH1);
 	selectI2CChannel(PCA9548ADDR,PCA9548_CH2);
 	//
    while(1) {
    	i2c_scan();
	vTaskDelay(1000 / portTICK_RATE_MS);
    }
}
// Main application
void app_main() {

	printf("i2c scanner\r\n\r\n");
// https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html
	// configure the i2c controller 0 in master mode, normal speed
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
	ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0));
	printf("- i2c driver installed\r\n\r\n");

	// start the loop task
	xTaskCreate(&loop_task, "loop_task", 2048, NULL, 5, NULL);
}
