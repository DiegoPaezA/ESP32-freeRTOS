/**
 * @file main.c
 * @author Learn Esp32 Course
 * @date 30/032021
 * @brief High Resolution Timer
 *
 * Timer de alta resolución (recomendado) - la menor resolución es 100us

 * @see https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/esp_timer.html
 */
#include <stdio.h>
#include "esp_timer.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

//isr
void timer_callback(void *args)
{
    static bool on;
    on = !on;
    gpio_set_level(GPIO_NUM_4, on);
}

void app_main(void)
{
    gpio_pad_select_gpio(GPIO_NUM_4);
    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);

    //caracteristicas del timer
    const esp_timer_create_args_t esp_timer_create_args = {
        .callback = timer_callback,
        .name = "My timer"};
    esp_timer_handle_t esp_timer_handle; //handler
    esp_timer_create(&esp_timer_create_args, &esp_timer_handle);
    esp_timer_start_periodic(esp_timer_handle, 4000); //Inicia el timer en microsegundos

    int x = 0;
    while(true)
    {
        //Dump the list of timers to a stream.
        esp_timer_dump(stdout);
        vTaskDelay(pdMS_TO_TICKS(1000));
        if(x++ == 5)
        {
            esp_timer_stop(esp_timer_handle);
            esp_timer_delete(esp_timer_handle);
        }
    }
}
