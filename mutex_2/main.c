#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h" //

xSemaphoreHandle mutexBus; //handler - flag

void writeToBus(char *message)
{
  printf(message);
}

void task1(void *params)
{
  while (true)
  {
    printf("reading temperature \n");
    //toma el semaforo, espera 1s para hacer el else
    if (xSemaphoreTake(mutexBus, 1000 / portTICK_PERIOD_MS)) 
    {
      writeToBus("temperature is 25c\n");
      xSemaphoreGive(mutexBus); //libera el semaforo
    }
    else
    {
      printf("writing temperature timed out \n");
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void task2(void *params)
{
  while (true)
  {
    printf("reading humidity\n");
    if (xSemaphoreTake(mutexBus, 1000 / portTICK_PERIOD_MS))
    {
      writeToBus("humidity is 50 \n");
      xSemaphoreGive(mutexBus); //libera el semaforo
    }
    else
    {
      printf("writing humidity timed out \n");
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void app_main(void)
{
  mutexBus = xSemaphoreCreateMutex();
  xTaskCreate(&task1, "temperature reading", 2048, NULL, 2, NULL);
  xTaskCreate(&task2, "humidity reading", 2048, NULL, 2, NULL);
}
