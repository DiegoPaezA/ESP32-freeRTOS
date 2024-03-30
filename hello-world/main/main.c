/* 
 * Hello world Example for the ESP32
 * ESP-IDF version: 5.2
 * ESP32 Board: FireBeetle 2 ESP32-E IoT Microcontroller
 * ESP32 URL: https://www.dfrobot.com/product-2195.html
 */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"

static const char *TAG = "example";

#define BLINK_GPIO GPIO_NUM_2

static uint8_t s_led_state = 0;

void app_main(void)
{
    printf("Hello world!\n");
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    while(1) {

        ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");
        gpio_set_level(BLINK_GPIO, s_led_state);
        s_led_state = !s_led_state;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
