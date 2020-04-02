/**
 * Ejemplo Semaforo Contador
 *
 */
#include <stdio.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <esp_system.h>
#include "sdkconfig.h"

SemaphoreHandle_t xSemaphore; //Objeto de semaforo
volatile int state = 0;
void isr(void *parameter) {

	BaseType_t aux = false; //Variable de control para el cambio de contexto
	xSemaphoreGiveFromISR(xSemaphore, &aux); //Libera el semaforo &aux -> pxHigherPriorityTaskWoken = true
	xSemaphoreGiveFromISR(xSemaphore, &aux); //Libera el semaforo
	xSemaphoreGiveFromISR(xSemaphore, &aux); //Libera el semaforo
	xSemaphoreGiveFromISR(xSemaphore, &aux); //Libera el semaforo
	ets_printf("ISR\n");
	if (aux) {
		portYIELD_FROM_ISR(); //Comando que fuerza el cambio de contexto.
		ets_printf("ISR - Cambio de contexto \n");
	}
}

void Tarea1(void *parameter) {
	printf("Parametro: ");
	printf("%d", (int)parameter);
	printf("\n");
	while (1) {
		xSemaphoreTake(xSemaphore, portMAX_DELAY);
		printf("ISRprocessing is running \n");
		/* toggle the LED now */
		state = !state;
		gpio_set_level(GPIO_NUM_2, state);
		/* elay here to see LED blinky */
		vTaskDelay(pdMS_TO_TICKS(100));
	}
	vTaskDelete( NULL);
}

void app_main() {

	xSemaphore = xSemaphoreCreateCounting(10, 0); //Se crea el semaforo y se asigna al objeto creado

	//Configuracion del GPIO15 y GPIO2 como OUTPUT en LOW
	gpio_pad_select_gpio(GPIO_NUM_15);
	gpio_set_direction(GPIO_NUM_15, GPIO_MODE_OUTPUT);
	gpio_set_level(GPIO_NUM_15, 0);
	gpio_pad_select_gpio(GPIO_NUM_2);
	gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
	gpio_set_level(GPIO_NUM_2, 0);

	//Configuracion  de la interrupcion en modo falling  para el GPIO4
	gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1);
	gpio_intr_enable(GPIO_NUM_5);
	gpio_set_intr_type(GPIO_NUM_5, GPIO_INTR_NEGEDGE);
	gpio_isr_handler_add(GPIO_NUM_5, isr, NULL);

	xTaskCreatePinnedToCore(Tarea1, "tarea1", 4096, NULL, 4, NULL, 0); //se crea una tarea que analiza el semaforo
}
