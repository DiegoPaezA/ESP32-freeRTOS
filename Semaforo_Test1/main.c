/**
 * Ejemplo Semaforo Binario_1
 *
 */
#include <stdio.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <esp_system.h>
#include "sdkconfig.h"

SemaphoreHandle_t SMF;//Objeto de semaforo

void isr(void*arg)
{
	BaseType_t aux = false;//Variable de control para el cambio de contexto
	xSemaphoreGiveFromISR(SMF, &aux);//Libera el semaforo
	ets_printf("ISR\n");

	if (aux)
	{
		portYIELD_FROM_ISR();//Comando que fuerza el cambio de contexto.
	}

}
void tarea1(void*arg)
{
	while(1)
	{
		//Intenta obtener el semaforo durante 200ms (Timeout).
		//en caso que el semaforo no este disponible en 200ms, se retorna falso.
		//printf("Task\n");

		if (xSemaphoreTake(SMF, pdMS_TO_TICKS(200)) == true)
		{
			//Se obtuvo el semaforo entre los 200ms de espera, se hara el toggle del pin 23

			for (uint8_t i = 0; i < 10; i++)
			{
				gpio_set_level(GPIO_NUM_2, 1);
				ets_delay_us(150);
				gpio_set_level(GPIO_NUM_2, 0);
				ets_delay_us(150);
			}
		}
		else
		{
			//Si no obtiene el semaforo en los 200ms de espera, hara el toggle del pin 22

			for (uint8_t i = 0; i < 10; i++)
			{
				gpio_set_level(GPIO_NUM_15, 1);
				ets_delay_us(150);
				gpio_set_level(GPIO_NUM_15, 0);
				ets_delay_us(150);
			}
		}

		vTaskDelay(pdMS_TO_TICKS(100));
	}
}
void app_main()
{

	SMF = xSemaphoreCreateBinary();//Se crea el semaforo y se asigna al objeto creado
	//Configuracion del GPIO15 y GPIO2 como OUTPUT en LOW
	gpio_pad_select_gpio(GPIO_NUM_15);
	gpio_set_direction(GPIO_NUM_15, GPIO_MODE_OUTPUT);
	gpio_set_level(GPIO_NUM_15, 0);
	gpio_pad_select_gpio(GPIO_NUM_2);
	gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
	gpio_set_level(GPIO_NUM_2, 0);

	//Configuracion del GPIO5 como INPUT y PULL UP
	gpio_pad_select_gpio(GPIO_NUM_5);
	gpio_set_direction(GPIO_NUM_5, GPIO_MODE_INPUT);
	gpio_pad_pullup(GPIO_NUM_5);
	//Configuracion  de la interrupcion en modo falling  para el GPIO4
	gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1);
	gpio_intr_enable(GPIO_NUM_5);
	gpio_set_intr_type(GPIO_NUM_5, GPIO_INTR_NEGEDGE);
	gpio_isr_handler_add(GPIO_NUM_5, isr, NULL);

	xTaskCreatePinnedToCore(tarea1, "tarea1", 4096, NULL, 1, NULL, 0);//se crea una tarea que analiza el semaforo
}
