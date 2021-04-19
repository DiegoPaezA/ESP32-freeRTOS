#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include <driver/adc.h>
#include "esp_adc_cal.h"
#include <esp_log.h>

#define DEFAULT_VREF    1100
#define NO_OF_SAMPLES   64          //Multisampling
static esp_adc_cal_characteristics_t *adc_chars;
static const adc_channel_t channel = ADC1_CHANNEL_0; //GPIO
static const adc_bits_width_t width = ADC_WIDTH_BIT_12;
static const adc_atten_t atten = ADC_ATTEN_DB_11; //3.3V range

void adc_task(void *pvParameter) {

	while (1) {
		uint32_t adc_reading = 0;
		//Multisampling measure
		for (int i = 0; i < NO_OF_SAMPLES; i++) {
			adc_reading += adc1_get_raw((adc1_channel_t) channel);
		}
		adc_reading /= NO_OF_SAMPLES;
		uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
		printf("Raw: %d\tVoltage: %dmV\n", adc_reading, voltage);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

/*
static void print_char_val_type(esp_adc_cal_value_t val_type)
{
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        printf("Characterized using Two Point Value\n");
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        printf("Characterized using eFuse Vref\n");
    } else {
        printf("Characterized using Default Vref\n");
    }
}
*/
void app_main() {

	adc1_config_width(width);
	adc1_config_channel_atten(channel, atten);
	//Characterize ADC to use esp_adc_cal functions
	adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
	esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, atten,width, DEFAULT_VREF, adc_chars);

//	print_char_val_type(val_type);

	xTaskCreate(&adc_task, "adc_task", 4096, NULL, 5, NULL);
}
