/**
 * Ejemplo cxColas - 1
 *
 */

#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <esp_system.h>
#include <esp_log.h>

QueueHandle_t buffer;//Objeto de la cola

void tarea1(void*arg)
{
    uint32_t snd = 0;
    while(1)
    {
        if (snd < 15)//si es menor que 15
        {
            xQueueSend(buffer, &snd, pdMS_TO_TICKS(0));//Envia la variable a la cola
            snd++;//incrementa la variable
        }
        else//si no, espera 5seg para probar el timout de la otra tarea.
        {
            vTaskDelay(pdMS_TO_TICKS(5000));
            snd = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void tarea2(void*z)
{
    uint32_t rcv = 0;
    while(1)
    {
        if (xQueueReceive(buffer, &rcv, pdMS_TO_TICKS(1000)) == true)//Si recibe el valor dentro de 1 segundo, muestra en pantalla
        {
            ESP_LOGI("Cola", "Item recibido: %u", rcv);//Muestra el valor en pantalla
        }
        else
        {
            ESP_LOGE("Cola", "Item no recibido, timeout expiro!");//Si el timeout expira, muestra error.
        }
    }
}

void app_main()
{
    buffer = xQueueCreate(10, sizeof(uint32_t));//crea la cola *buffer* con 10 slots de 4 Bytes

    xTaskCreatePinnedToCore(tarea1, "tarea1", 4096, NULL, 1, NULL, 0);//Crea tarea que escribe valores en la cola
    xTaskCreatePinnedToCore(tarea2, "tarea2", 4096, NULL, 1, NULL, 0);//Crea tarea que lee valores de la cola.
}
