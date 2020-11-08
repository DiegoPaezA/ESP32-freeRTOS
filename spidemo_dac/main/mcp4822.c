/*
 * mcp4822.c
 *
 *  Created on: 10/04/2020
 *      Author: DiegoPaez
 */

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <driver/spi_master.h>
#include <driver/gpio.h>
#include "esp_log.h"
#include "mcp4822.h"




#define HSPI_HOST   SPI2_HOST
#define VSPI_HOST   SPI3_HOST
#define TAG_SPI  "DAC_MCP4822"
static const int SPI_Frequency = SPI_MASTER_FREQ_20M;

void spi_master_init(MPC4822_t * DAC, int8_t GPIO_MOSI, int8_t GPIO_SCLK, int8_t GPIO_CS)
{
	/*
	 * @param MPC4822_t: Data structure
	 * @param GPIO_MOSI: PIN_MOSI = 23;
	 * @param GPIO_SCLK: PIN_CLK  = 18;
	 * @param GPIO_CS: PIN_CS = 3;
	 */
		gpio_pad_select_gpio( GPIO_CS );
		gpio_set_direction( GPIO_CS, GPIO_MODE_OUTPUT );
		gpio_set_level( GPIO_CS, 1 );

		ESP_LOGI(TAG_SPI, "GPIO_CS=%d",GPIO_CS);
		esp_err_t ret;
		spi_device_handle_t spi;
		spi_bus_config_t buscfg = {
				.miso_io_num = -1,
				.mosi_io_num = GPIO_MOSI,
				.sclk_io_num = GPIO_SCLK,
				.quadwp_io_num = -1,
				.quadhd_io_num = -1,
				.max_transfer_sz = 16, };
		spi_device_interface_config_t devcfg = {
				.clock_speed_hz = SPI_Frequency,  //Clock out at 20 MHz
				.mode = 0,                        //SPI mode 0 rising edge of the clock pulse
				.spics_io_num = -1,               //CS pin
				.queue_size = 7, //We want to be able to queue 1 transactions at a time
				};
		//Initialize the SPI bus
		ret = spi_bus_initialize( VSPI_HOST, &buscfg, 2 );
		ESP_LOGI(TAG_SPI, "spi_bus_initialize=%d",ret);
		assert(ret==ESP_OK);
		ret = spi_bus_add_device( VSPI_HOST, &devcfg, &spi);
		ESP_LOGI(TAG_SPI, "spi_bus_add_device=%d",ret);
		assert(ret==ESP_OK);
		DAC->_CS = GPIO_CS;
		DAC->_SPIHandle = spi;
}

bool dacInit(MPC4822_t * DAC, bool channel,  bool gain, bool onoff)
{
	/*
	 * @param MPC4822_t: Data structure
	 * @param channel: DAC_CHANEL_B = 1; DAC_CHANEL_A = 0;
	 * @param gain: DAC_GAIN_1x = 1; DAC_GAIN_2x = 0 (Vref=2.043);
	 * @param onoff: DAC_ON = 1; DAC_OFF = 0;
	 *
	 */
	DAC->_channel = channel;
	DAC->_gain = gain;
	DAC->_onoff = onoff;
	return true;
}

bool dacSetVoltage(MPC4822_t * DAC, int voltage)
{
	/*
	 * @param MPC4822_t: Data structure
	 * @param voltage: 0-4095 (0-3.3v);
	 *
	 */

	uint16_t command = 0;
	command = command | ((DAC->_channel) << REG_CHANNEL);
	command = command | ((DAC->_gain) << REG_GAIN);
	command = command | ((DAC->_onoff) <<  REG_ONOFF);
	command = command | (voltage &  0x0FFF);

	gpio_set_level( DAC->_CS, 0 ); // Enable to Write
	spi_master_write_data_word(DAC, command);
	gpio_set_level( DAC->_CS, 1 ); // Disable to Write

	ESP_LOGI(TAG_SPI, "DAC Voltage Update: %d", voltage);

	return true;
}

bool spi_master_write_byte(spi_device_handle_t SPIHandle, const uint8_t* Data, size_t DataLength)
{
	/*
	 * @param spi_device_handle_t: SPI Handle
	 * @param Data: 1 bytes word (unit8_t);
	 * @param DataLength(size_t);
	 *
	 */
	spi_transaction_t SPITransaction;
	esp_err_t ret;

	if ( DataLength > 0 ) {
		memset( &SPITransaction, 0, sizeof( spi_transaction_t ) );
		SPITransaction.length = DataLength * 8;
		SPITransaction.tx_buffer = Data;
	#if 1
		ret = spi_device_transmit( SPIHandle, &SPITransaction );
	#endif
	#if 0
		ret = spi_device_polling_transmit( SPIHandle, &SPITransaction );
	#endif
		assert(ret==ESP_OK);
	}

	return true;
}

bool spi_master_write_data_word(MPC4822_t * DAC, uint16_t data)
{
	/*
	 * @param MPC4822_t: Data structure
	 * @param data: 2 bytes word (unit16_t);
	 *
	 */
	static uint8_t Byte[2];
	Byte[0] = (data >> 8) & 0xFF;
	Byte[1] = data & 0xFF;

	return spi_master_write_byte( DAC->_SPIHandle, Byte, 2);
}
