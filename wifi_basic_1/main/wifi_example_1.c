#include <stdio.h>
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h" //almacenamiento de datos en la memoría de datos
#include "esp_http_client.h"
#include "protocol_examples_common.h" // librería protocolos comunes cx wifi

/*
 @Función que se encarga de manejar los eventos del cliente. 
  En esta función el cliente responderá cuando se reciban datos
  "HTTP_EVENT_ON_DATA"

  Se imprime el tamaño de los datos y el html de la página solicitada. 
*/
esp_err_t clientEvent(esp_http_client_event_t *evt)
{
  switch (evt->event_id)
  {
  case HTTP_EVENT_ON_DATA:
    printf("%.*s\n", evt->data_len, (char *)evt->data);
    break;
  default:
    break;
  }
  return ESP_OK;
}

void app_main(void)
{
  nvs_flash_init();
  tcpip_adapter_init();
  esp_event_loop_create_default();
  example_connect();

  // Configuración del cliente
  esp_http_client_config_t clientConfig = {
      .url = "http://google.com",
      .event_handler = clientEvent};

  esp_http_client_handle_t client = esp_http_client_init(&clientConfig);
  esp_http_client_perform(client);
  esp_http_client_cleanup(client);
}