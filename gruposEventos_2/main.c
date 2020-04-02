/**
 * @brief Ejemplo Control de tareas con grupos de eventos 2
 */
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <esp_system.h>
#include <freertos/timers.h>
#include <freertos/semphr.h>
#include <driver/gpio.h>
#include <esp_log.h>

EventGroupHandle_t evt; //Cria o objeto do grupo de eventos
SemaphoreHandle_t smf; //Se crea objeto de semaforo binario
TimerHandle_t tmr; //Crea el objeto del timer
volatile int state = 0;
int count = 0;

/* define event bits */
#define TASK_1_BIT        ( 1 << 0 ) //1
#define TASK_2_BIT        ( 1 << 1 ) //10
#define TASK_3_BIT        ( 1 << 2 ) //100
#define ALL_SYNC_BITS (TASK_1_BIT | TASK_2_BIT | TASK_3_BIT) //111

void ISR(void *z) {
	int aux = 0;
	xSemaphoreGiveFromISR(smf, &aux); //Entrega el semaforo para su uso

	if (aux) {
		portYIELD_FROM_ISR(); //Fuerza un cambio de contexto
	}
}

void tarea1(void *parameter) {
	for (;;) {
		/* wait forever until event bit of task 1 is set */
		EventBits_t xbit = xEventGroupWaitBits(evt, TASK_1_BIT, pdTRUE, pdTRUE,portMAX_DELAY);
		printf("task1 has even bit: ");
		printf("%d",xbit);
		printf("\n");
	}
	vTaskDelete( NULL);
}
/* this task is similar to sendTask1 */
void tarea2(void *parameter) {

	for (;;) {
		/* wait forever until event bit of task 2 is set */
		EventBits_t xbit = xEventGroupWaitBits(evt, TASK_2_BIT, pdTRUE, pdTRUE,portMAX_DELAY);
		printf("task2 has even bit: ");
		printf("%d",xbit);
		printf("\n");
	}
	vTaskDelete( NULL);
}
void tarea3(void *parameter) {
	for (;;) {
		/* wait forever until event bit of task 3 is set */
		EventBits_t xbit = xEventGroupWaitBits(evt, TASK_3_BIT, pdTRUE, pdTRUE,portMAX_DELAY);
		printf("task3 has even bit: ");
		printf("%d",xbit);
		printf("\n");
	}
	vTaskDelete( NULL);
}

void app_main() {
	evt = xEventGroupCreate();        //Crea el grupo de eventos

	smf = xSemaphoreCreateBinary(); //crea el semaforo

	tmr = xTimerCreate("tmr_smf", pdMS_TO_TICKS(1000), true, 0, ISR); //Crea timer con 100ms de respuesta y autorecarga
	xTimerStart(tmr, pdMS_TO_TICKS(100)); //Inicia el timer

	//Selección de los pines a utilizar
	gpio_pad_select_gpio(GPIO_NUM_2);
	//Configuración de los pines como salida
	gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);

	xTaskCreatePinnedToCore(tarea1, "tarea1", 2048, NULL, 1, NULL, 0); //crea tarea que espera los eventos
	xTaskCreatePinnedToCore(tarea2, "tarea2", 2048, NULL, 1, NULL, 0); //crea tarea que espera los eventos
	xTaskCreatePinnedToCore(tarea3, "tarea3", 2048, NULL, 1, NULL, 0); //crea tarea que espera los eventos

	while (1) {
		if (xSemaphoreTake(smf, portMAX_DELAY)) {
			//ESP_LOGI("Timer", "Expiro!"); //Cuando se obtiene el semaforo activará el pin 2 y enviara información al puerto serial.
			gpio_set_level(GPIO_NUM_2, state);
			state = !state;
			BaseType_t xHigherPriorityTaskWoken;
			count++;
			//printf("Count: ");
			//printf("%d",count);
			//printf("\n");

			if (count == 2) {
				/* if counter is equal 2 then set event bit of task1 */
				xEventGroupSetBitsFromISR(evt, TASK_1_BIT,&xHigherPriorityTaskWoken);
			} else if (count == 3) {
				/* if counter is equal 3 then set event bit of task 2 and 3 */
				xEventGroupSetBitsFromISR(evt, TASK_2_BIT | TASK_3_BIT,&xHigherPriorityTaskWoken);
			} else if (count == 4) {
				/* reset counter to start again */
				count = 0;
			}
		}

		vTaskDelay(pdMS_TO_TICKS(10));
	}
}
