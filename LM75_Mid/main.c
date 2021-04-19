#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_types.h"

#define SDA_GPIO 21
#define SCL_GPIO 22
#define LM75A_ADDRESS 0X48
#define LM75A_TEMP 0X00
#define LM75A_CONF 0X01
#define MODE_SHUTDOWN 0X01
#define MODE_NORMAL 0X00
/**
 * Iniciliaza el puerto de comunicación I2C
 * @param[in] pinSDA: Pin de datos I2C Seleccionado por el usuario. 
 * @param[in] pinSCL: Pin de reloj I2C Seleccionado por el usuario. 
 */
void i2cinit(int pinSDA, int pinSCL)
{
  i2c_config_t i2c_config = {
      .mode = I2C_MODE_MASTER,
      .sda_io_num = pinSDA,
      .scl_io_num = pinSCL,
      .sda_pullup_en = GPIO_PULLUP_ENABLE,
      .scl_pullup_en = GPIO_PULLUP_ENABLE,
      .master.clk_speed = 100000};
  i2c_param_config(I2C_NUM_0, &i2c_config);
  i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
}
/**
 * Cambia el modo de operación del sensor LM75A
 * @param[in] mode : modo de operación  MODE_SHUTDOWN o MODE_NORMAL

 */
void setMode(int mode)
{
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (LM75A_ADDRESS << 1) | I2C_MASTER_WRITE, true);
  i2c_master_write_byte(cmd, LM75A_CONF, true);
  i2c_master_write_byte(cmd, mode, true);
  i2c_master_stop(cmd);
  if (i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS) == ESP_OK)
  {
    if (mode == 0x00)
    {
      printf("-> Modo de operación es normal\n");
    }
    else if (mode == 0x01)
    {
      printf("-> Modo de operación es shutdown\n");
    }
  }
  i2c_cmd_link_delete(cmd);
}

float getTemp()
{
  uint8_t raw[2];
  i2c_cmd_handle_t cmd_handle = i2c_cmd_link_create();
  i2c_master_start(cmd_handle);
  i2c_master_write_byte(cmd_handle, (LM75A_ADDRESS << 1) | I2C_MASTER_WRITE, true);
  i2c_master_write_byte(cmd_handle,LM75A_TEMP,true);
	i2c_master_start(cmd_handle);
  i2c_master_write_byte(cmd_handle, (LM75A_ADDRESS << 1) | I2C_MASTER_READ, true);
  i2c_master_read(cmd_handle, (uint8_t *)&raw, 2, I2C_MASTER_ACK);
  i2c_master_stop(cmd_handle);
  i2c_master_cmd_begin(I2C_NUM_0, cmd_handle, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd_handle);

  bool isNeg = false;
  if (raw[0] & 0x80)
  {
    isNeg = true;
    raw[0] = raw[0] & 0x7f;
  }
  int16_t data = (raw[0] << 8 | raw[1]) >> 5;
  float temperature = (data * 0.125) * (isNeg ? -1 : 1);
  return temperature;
}

void app_main()
{
  i2cinit(SDA_GPIO,SCL_GPIO);
  setMode(MODE_NORMAL);
  vTaskDelay(100/portTICK_PERIOD_MS);
  printf("temperature %f\n", getTemp());
  setMode(MODE_SHUTDOWN);
}
