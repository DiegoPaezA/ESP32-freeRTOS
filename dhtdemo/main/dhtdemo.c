#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include <dht.h>

static const dht_sensor_type_t sensor_type = DHT_TYPE_DHT11;
static const gpio_num_t dht_gpio = 18;

void dht_task(void *pvParameter) {
	int16_t temperature = 0;
	int16_t humidity = 0;

    // DHT sensors that come mounted on a PCB generally have
    // pull-up resistors on the data pin.  It is recommended
    // to provide an external pull-up resistor otherwise...

    //gpio_set_pull_mode(dht_gpio, GPIO_PULLUP_ONLY);
	while (1) {
		if (dht_read_data(sensor_type, dht_gpio, &humidity, &temperature)== ESP_OK){
			printf("Humidity: %d%% Temp: %d^C\n", humidity / 10,temperature / 10);
		}
		else{
			printf("Could not read data from sensor\n");
		}
		vTaskDelay(5000 / portTICK_PERIOD_MS);
	}
}

void app_main() {
	xTaskCreate(&dht_task, "dht_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
}
