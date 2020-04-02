#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
void tarea1(void *pvParameter)
{//Tarea que simula un pwm para observar en el analizador logico
    while (1)
    {
        gpio_set_level(GPIO_NUM_2, 1);
        ets_delay_us(25);
        gpio_set_level(GPIO_NUM_2, 0);
        ets_delay_us(25);
    }
}
void tarea2(void *pvParameter)
{//Tarea que simula un pwm para observar en el analizador logico
    while (1)
    {
        gpio_set_level(GPIO_NUM_4, 1);
        ets_delay_us(25);
        gpio_set_level(GPIO_NUM_4, 0);
        ets_delay_us(25);
    }
}
void tarea3(void *pvParameter)
{//Tarea que simula un pwm para observar en el analizador logico
    while (1)
    {
        for (uint8_t i = 0; i < 200; i++)
        {
            gpio_set_level(GPIO_NUM_15, 1);
            ets_delay_us(500);
            gpio_set_level(GPIO_NUM_15, 0);
            ets_delay_us(500);
        }
        vTaskDelay(pdMS_TO_TICKS(200)); // Delay o bloqueo de la tarea
    }
}

void app_main()
{
    //Selección de los pines a utilizar
    gpio_pad_select_gpio(GPIO_NUM_2);
    gpio_pad_select_gpio(GPIO_NUM_4);
    gpio_pad_select_gpio(GPIO_NUM_15);

    //Configuración de los pines como salida
    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_NUM_15, GPIO_MODE_OUTPUT);


    //Creación de las tareas.
    xTaskCreatePinnedToCore(tarea1, "task1", 2048, NULL, 1, NULL, 0);//Tarea 1 con prioridad (1) en el core 0
    xTaskCreatePinnedToCore(tarea2, "task2", 2048, NULL, 1, NULL, 0);//Tarea 2 con prioridad (1) en el core 0
    //xTaskCreatePinnedToCore(tarea3, "task3", 2048, NULL, 2, NULL, 0);//Tarea 2 con prioridad (2) en el core 0
    xTaskCreatePinnedToCore(tarea3, "task3", 2048, NULL, 2, NULL, 1);//Tarea 2 con prioridad (2) en el core 1
}
