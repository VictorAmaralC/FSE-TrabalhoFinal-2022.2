#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "freertos/semphr.h"
#include <inttypes.h>
#include "include/wifi.h"
#include "include/mqtt.h"
#include "include/dht11.h"
#include "include/gpio.h"
#include <time.h>

SemaphoreHandle_t conexaoWifiSemaphore;
SemaphoreHandle_t conexaoMQTTSemaphore;

TaskHandle_t taskHandleWifi;
TaskHandle_t taskHandleFlame;
TaskHandle_t taskHandleDht;
TaskHandle_t taskHandleRgb;
TaskHandle_t taskHandleBuzzer;

int overallTemperature;
int alarmSystem = false;
int stopAlarm = 0;
int flameSensor = false;

void sendInformation(char* info ,int data, char* topic) {
  char msg[200];
  sprintf(msg, info, data);
  mqtt_envia_mensagem(topic, msg);
}

void conectadoWifi(void * params) {
  while(true) {
    if(xSemaphoreTake(conexaoWifiSemaphore, portMAX_DELAY)) {
      mqtt_start();
    }
  }
}

void DHT11(void *params) {
    while (xSemaphoreTake(conexaoMQTTSemaphore, portMAX_DELAY)) {
        while (true) {
            struct dht11_reading data = getInformation();
            if (data.status == 0 || (data.temperature > 0 && data.humidity > 0)) {
              sendInformation("{\"temperature\": %d}", data.temperature, "v1/devices/me/telemetry");
              sendInformation("{\"umidade\": %d}", data.humidity, "v1/devices/me/telemetry");
              sendInformation("{\"alarme\": %d}", alarmSystem, "v1/devices/me/telemetry");
              sendInformation("{\"chama\": %d}", flameSensor, "v1/devices/me/attributes");
              overallTemperature = data.temperature;
            }
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
}

void BUZZER(void *params) {
  configureBuzzer();
  playSound();
}

void rgb(void *params){
  setUpPwm(100.0);
  while(true){
    changeColor(255, 0, 0);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    changeColor(0, 0, 255);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void flame(void *params){
  configureFlame();
  while(true){
    if(flameSensor == false && gpio_get_level(5) == 1){
      printf("Sensor de fogo ativado\n");
      flameSensor = true;
    } else if (flameSensor == true && gpio_get_level(5) == 0){
      printf("Chama apagada\n");
      flameSensor = false;
    }
    vTaskDelay(1000);
    printf("Estado da chama:%d\nEstado do sensor%d\n", flameSensor, gpio_get_level(5));
  }
}

void mqttTask(void *params) {
  xTaskCreate(&conectadoWifi, "Conexão ao MQTT", 4096, NULL, 1, &taskHandleWifi);
}

void dht11Task(void *params) {
  xTaskCreate(&DHT11, "Coleta de temperatura e umidade", 4096, NULL, 1, &taskHandleDht);
}

void buzzerTask(void *params) {
  xTaskCreate(&BUZZER, "Acionamento buzzer", 4096, NULL, 1, &taskHandleBuzzer);
}

void rgbTask(void *params) {
  xTaskCreate(&rgb, "Acionamento do RGB", 4096, NULL, 1, &taskHandleRgb);
}

void flameTask(void *params) {
  xTaskCreate(&flame, "Acionamento do sensor de chama", 4096, NULL, 1, &taskHandleFlame);
}

void app_main(void)
{
    // Inicializa o NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    
    conexaoWifiSemaphore = xSemaphoreCreateBinary();
    conexaoMQTTSemaphore = xSemaphoreCreateBinary();
    wifi_start();

    pthread_t tid[5];
    pthread_create(&tid[0], NULL, (void *)mqttTask, (void *)NULL);
    pthread_create(&tid[1], NULL, (void *)dht11Task, (void *)NULL);
    pthread_create(&tid[4], NULL, (void *)flameTask, (void *)NULL);
    while(true){
      if(overallTemperature >= 31 && alarmSystem == false && flameSensor == true){
        alarmSystem = true;
        pthread_create(&tid[2], NULL, (void *)buzzerTask, (void *)NULL);
        pthread_create(&tid[3], NULL, (void *)rgbTask, (void *)NULL);
      } else if (overallTemperature < 31 && alarmSystem == true && flameSensor == false){
        changeColor(0,0,0);
        stopSound();
        vTaskDelete(taskHandleBuzzer);
        vTaskDelete(taskHandleRgb);
        pthread_join(tid[2], NULL);
        pthread_join(tid[3], NULL);
        alarmSystem = false;
      }
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[2], NULL);
    pthread_join(tid[3], NULL);
    pthread_join(tid[4], NULL);
}
