/**
 * Ejemplo cxColas - 2
 *
 */
#include <stdio.h>
#include <string.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <esp_system.h>
#include <esp_log.h>

QueueHandle_t buffer; //Objeto de la cola

/* structure that hold data*/
typedef  struct{
	int sender;
	int counter;
}Data;

void sendTask1(void *arg) {
	/* keep the status of sending data */
	BaseType_t xStatus;
	/* create data to send */
	Data data1;
	/* sender 1 has id is 1 */
	data1.sender = 1;
	data1.counter = 1;
	while (1) {
		/* send data to front of the queue */
		printf("sendTask1 is sending data \n");
		xStatus = xQueueSendToFront(buffer, &data1, pdMS_TO_TICKS(100));
		/* check whether sending is ok or not */
		if (xStatus == pdPASS) {
			/* increase counter of sender 1 */
			data1.counter = data1.counter + 1;
		}
		/* we delay here so that receiveTask has chance to receive data */
		vTaskDelay(pdMS_TO_TICKS(1000)); //Tiempo de bloqueo
	}
	vTaskDelete( NULL);
}

void sendTask2(void *arg) {/* keep the status of sending data */
	BaseType_t xStatus;
	/* create data to send */
	Data data2;
	/* sender 1 has id is 1 */
	data2.sender = 2;
	data2.counter = 1;

	while (1) {
		printf("sendTask2 is sending data \n");
		/* send data to front of the queue */
		xStatus = xQueueSendToFront(buffer, &data2, pdMS_TO_TICKS(100));
		/* check whether sending is ok or not */
		if (xStatus == pdPASS) {
			/* increase counter of sender 1 */
			data2.counter = data2.counter + 1;
		}
		/* we delay here so that receiveTask has chance to receive data */
		vTaskDelay(pdMS_TO_TICKS(1000)); //Tiempo de bloqueo
	}
	vTaskDelete( NULL);
}

void receiveTask(void *arg) {
	/* keep the status of sending data */
	BaseType_t xStatus;
	/* create data to send */
	Data data;

	while (1) {

		/* send data to front of the queue */
		xStatus = xQueueReceive(buffer, &data, pdMS_TO_TICKS(100));
		/* check whether sending is ok or not */
		if (xStatus == pdPASS) {
			printf("receiveTask got data: ");
			printf("sender = ");
			printf("%d",data.sender);
			printf(" counter = ");
			printf("%d",data.counter);
			printf("\n");
		}

	}
	vTaskDelete( NULL);
}


void app_main() {

	buffer = xQueueCreate(5, sizeof(Data)+1); //crea la cola *buffer* con 10 slots de 4 Bytes
	xTaskCreatePinnedToCore(sendTask1, "sendTask1", 10000, NULL, 2, NULL, 0); //Crea tarea que escribe valores en la cola
	xTaskCreatePinnedToCore(sendTask2, "sendTask2", 10000, NULL, 2, NULL, 0); //Crea tarea que lee valores de la cola.
	xTaskCreatePinnedToCore(receiveTask, "receiveTask", 10000, NULL, 1, NULL,0); //Crea tarea que lee valores de la cola.

}
