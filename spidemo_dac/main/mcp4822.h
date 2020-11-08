/*
 * mcp4822.h
 *
 *  Created on: 10/04/2020
 *      Author: DiegoPaez
 */

#ifndef MAIN_MCP4822_H_
#define MAIN_MCP4822_H_

#include "driver/spi_master.h"

#define PIN_MISO 19
#define PIN_MOSI 23
#define PIN_CLK  18
#define PIN_CS   3

#define DAC_CHANEL_A  		0
#define DAC_CHANEL_B  		1
#define DAC_GAIN_1x   		1
#define DAC_GAIN_2x   		0 //Vref 2.048 MCP4822
#define DAC_ON   			1
#define DAC_OFF	  			0
#define REG_CHANNEL 		15
#define REG_GAIN     		13
#define REG_ONOFF			12

typedef struct {
	bool _channel;
	bool _gain;
	bool _onoff;
	uint8_t _CS;
	spi_device_handle_t _SPIHandle;
} MPC4822_t;

void spi_master_init(MPC4822_t * DAC, int8_t GPIO_MOSI, int8_t GPIO_SCLK, int8_t GPIO_CS);
bool dacInit(MPC4822_t * DAC, bool channel,  bool gain, bool onoff);
bool dacSetVoltage(MPC4822_t * DAC, int voltage);
bool spi_master_write_byte(spi_device_handle_t SPIHandle, const uint8_t* Data, size_t DataLength);
bool spi_master_write_data_word(MPC4822_t * DAC, uint16_t data);

#endif /* MAIN_MCP4822_H_ */
