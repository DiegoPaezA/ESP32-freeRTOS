#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include <driver/adc.h>
#include "esp_adc_cal.h"
#include "esp_log.h"
#include "mcp4822.h"

#define DEFAULT_VREF    1100
#define NO_OF_SAMPLES   64          //Multisampling
static esp_adc_cal_characteristics_t *adc_chars;
static const adc_channel_t channel = ADC1_CHANNEL_7; //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_bits_width_t width = ADC_WIDTH_BIT_12;
static const adc_atten_t atten = ADC_ATTEN_DB_11;

uint32_t adc_reading = 0;

void adcRead_task(void *pvParameter) {
	adc1_config_width(width);
	adc1_config_channel_atten(channel, atten);
	//Characterize ADC
	adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
	esp_adc_cal_characterize(ADC_UNIT_1, atten,width, DEFAULT_VREF, adc_chars);

	while (1) {
		adc_reading = 0;
		//Multisampling
		for (int i = 0; i < NO_OF_SAMPLES; i++) {
			adc_reading += adc1_get_raw((adc1_channel_t) channel);
		}
		adc_reading /= NO_OF_SAMPLES;
		uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
		printf("Raw: %d\tVoltage: %dmV\n", adc_reading, voltage);
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}

void app_main(void) {

	MPC4822_t DAC1;
	
	spi_master_init(&DAC1, PIN_MOSI, PIN_CLK, PIN_CS);
	dacInit(&DAC1,DAC_CHANEL_A, DAC_GAIN_2x, DAC_ON);

	xTaskCreate(&adcRead_task, "adcRead_task",4096, NULL, 5, NULL);

	while(1){

		dacSetVoltage(&DAC1,adc_reading);
		vTaskDelay(pdMS_TO_TICKS(100));
	}

}
