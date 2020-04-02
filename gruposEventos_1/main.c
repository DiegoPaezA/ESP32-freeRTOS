/**
 * @brief Ejemplo Control de tareas con grupos de eventos 1
 */
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <esp_system.h>
#include <esp_log.h>

EventGroupHandle_t evt;//Se crea el objeto de grupo de eventos

#define EV_1SEG (1<<0)//Op1 Definir el BIT del evento 1

/*
 *Op2 y Op3
#define EV_1 (1<<0) //1
#define EV_2 (1<<1) //10
#define EV_3 (1<<2) //100
*/


void tarea1(void*z)
{
    EventBits_t x;//Crea la variable que recibe el valor de los eventos
    while (1)
    {
        //Op1 Se espera por el evento (EV_1SEG) por un maximo de 1000ms
        x = xEventGroupWaitBits(evt, EV_1SEG, true, true, pdMS_TO_TICKS(1000));

        //Op2 - Espera por los tres eventos
        //x = xEventGroupWaitBits(evt, EV_1 | EV_2 | EV_3, true, true, pdMS_TO_TICKS(1000));

        //Op3 - Espera por uno de los eventos
        //x = xEventGroupWaitBits(evt, EV_1 | EV_2 | EV_3, true, false, pdMS_TO_TICKS(1000));

        if (x & EV_1SEG)//Si X & EV_1SEG (True), significa que el evento ocurrio
        {
            printf("T1 - OK \n");
        }
        else
        {
            printf("T1 - Event group TIMEOUT \n");
        }
    }
}

void app_main()
{
    evt = xEventGroupCreate();//Crea grupo de evento

    xTaskCreatePinnedToCore(tarea1, "tarea1", 2048, NULL, 1, NULL, 0);//crea tarea que espera los eventos

    while (1)
    {
        for (uint8_t i = 0; i < 3; i++)//Envia 3 eventos antes del timeout
        {
            vTaskDelay(pdMS_TO_TICKS(333));
            xEventGroupSetBits(evt, EV_1SEG);//Configura el BIT (EV_1SEG) en 1
            //xEventGroupSetBits(evt, EV_1 | EV_2 | EV_3);//
        }

        vTaskDelay(pdMS_TO_TICKS(1000));//Espera el timeout para mostrar el error
    }
}
