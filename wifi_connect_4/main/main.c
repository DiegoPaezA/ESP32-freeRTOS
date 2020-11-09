#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_event_loop.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"


char *TAG = "CONNECTION";

xSemaphoreHandle connectionSemaphore;

//Manejador de eventos - event task
//https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/wifi.html
static esp_err_t event_handler(void *ctx, system_event_t *event)
{
  switch (event->event_id)
  {
  case SYSTEM_EVENT_STA_START:
    esp_wifi_connect();
    ESP_LOGI(TAG,"connecting...\n");
    break;

  case SYSTEM_EVENT_STA_CONNECTED:
    ESP_LOGI(TAG,"connected\n");
    break;

  case SYSTEM_EVENT_STA_GOT_IP:
    ESP_LOGI(TAG,"got ip\n");
    xSemaphoreGive(connectionSemaphore);
    break;

  case SYSTEM_EVENT_STA_DISCONNECTED:
    ESP_LOGI(TAG,"disconnected\n");
    break;

  default:
    break;
  }
  return ESP_OK;
}

void wifiInit()
{
  ESP_ERROR_CHECK(nvs_flash_init()); //inicialización de memoria flash 
  tcpip_adapter_init();
  //inicialización del Controlador de eventos
  ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL)); 

  wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  wifi_config_t wifi_config =
      {
          .sta = {
              .ssid = CONFIG_WIFI_SSID,
              .password = CONFIG_WIFI_PASSWORD
              }};
  esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
  ESP_ERROR_CHECK(esp_wifi_start());
}

void OnConnected(void *para)
{
  while (true)
  {
    if (xSemaphoreTake(connectionSemaphore, 10000 / portTICK_RATE_MS))
    {
      ESP_LOGI(TAG, "connected");
      // do something
      tcpip_adapter_ip_info_t ip_info;
      ESP_ERROR_CHECK(tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info));
      printf("IP Addresss: %s\n", ip4addr_ntoa(&ip_info.ip));
      printf("Subnet Mask: %s\n", ip4addr_ntoa(&ip_info.netmask));
      printf("Gateway: %s\n", ip4addr_ntoa(&ip_info.ip));

      xSemaphoreTake(connectionSemaphore, portMAX_DELAY);
    }
    else
    {
      ESP_LOGE(TAG, "Failed to connect. Retry in");
      for (int i = 0; i < 5; i++)
      {
        ESP_LOGE(TAG, "...%d", i);
        vTaskDelay(1000 / portTICK_RATE_MS);
      }
      esp_restart(); // si no hay conexion reiniciar el chip.
    }
  }
}

void app_main()
{
  esp_log_level_set(TAG, ESP_LOG_DEBUG);
  connectionSemaphore = xSemaphoreCreateBinary();
  wifiInit();
  xTaskCreate(&OnConnected, "handel comms", 1024 * 3, NULL, 5, NULL);
}