/**
 * @brief Ejemplo Timer por Software
 */
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <freertos/timers.h>
#include <driver/gpio.h>
#include <esp_system.h>
#include <esp_log.h>

SemaphoreHandle_t smf; //Se crea objeto de semaforo binario
TimerHandle_t tmr; //Crea el objeto del timer
volatile int state = 0;

void ISR(void *z) {
	int aux = 0;
	xSemaphoreGiveFromISR(smf, &aux); //Entrega el semaforo para su uso

	if (aux) {
		portYIELD_FROM_ISR(); //Fuerza un cambio de contexto
	}
}

void app_main() {
	smf = xSemaphoreCreateBinary(); //crea el semaforo

	tmr = xTimerCreate("tmr_smf", pdMS_TO_TICKS(100), true, 0, ISR); //Crea timer con 100ms de respuesta y autorecarga
	xTimerStart(tmr, pdMS_TO_TICKS(100)); //Inicia el timer

	//Selección de los pines a utilizar
	gpio_pad_select_gpio(GPIO_NUM_2);
	//Configuración de los pines como salida
	gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);

	while (1) {
		if (xSemaphoreTake(smf, portMAX_DELAY)) {
			ESP_LOGI("Timer", "Expiró!"); //Cuando se obtiene el semaforo activará el pin 2 y enviara información al puerto serial.
			gpio_set_level(GPIO_NUM_2, state);
			state = !state;
		}

		vTaskDelay(pdMS_TO_TICKS(10));
	}
}
