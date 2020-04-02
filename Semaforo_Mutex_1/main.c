/**
 * Ejemplo Semaforo Mutex
 *
 */
#include <stdio.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <esp_system.h>
#include "sdkconfig.h"

SemaphoreHandle_t xMutex;//Objeto de semaforo

void tareaLP(void*arg)
{
	while(1)
	{
		//printf("Task\n");
		xSemaphoreTake( xMutex,portMAX_DELAY);
			for (uint8_t i = 0; i < 10; i++)
			{
				gpio_set_level(GPIO_NUM_2, 1);
				ets_delay_us(150);
				gpio_set_level(GPIO_NUM_2, 0);
				ets_delay_us(150);
			}

		vTaskDelay(pdMS_TO_TICKS(100)); //Tiempo de bloqueo de la tareaHP
		xSemaphoreGive( xMutex );
	}
}
void tareaHP(void*arg){

	while(1)
		{
			//printf("Task\n");
			xSemaphoreTake( xMutex, portMAX_DELAY );
				for (uint8_t i = 0; i < 10; i++)
				{
					gpio_set_level(GPIO_NUM_15, 1);
					ets_delay_us(150);
					gpio_set_level(GPIO_NUM_15, 0);
					ets_delay_us(150);
				}
			xSemaphoreGive( xMutex );
			vTaskDelay(pdMS_TO_TICKS(400)); // tiempo de ejecución para TareaLP
		}
}

void app_main()
{

	xMutex = xSemaphoreCreateMutex();//Se crea el semaforo y se asigna al objeto creado

	//Configuracion del GPIO15 y GPIO2 como OUTPUT en LOW
	gpio_pad_select_gpio(GPIO_NUM_15);
	gpio_set_direction(GPIO_NUM_15, GPIO_MODE_OUTPUT);
	gpio_set_level(GPIO_NUM_15, 0);
	gpio_pad_select_gpio(GPIO_NUM_2);
	gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
	gpio_set_level(GPIO_NUM_2, 0);

	xTaskCreatePinnedToCore(tareaLP, "tareaLP", 4096, NULL, 1, NULL, 0);//Se crea una tarea de baja prioridad
	xTaskCreatePinnedToCore(tareaHP, "tareaHP", 4096, NULL, 4, NULL, 0);//Se crea una tarea de alta prioridad
}
