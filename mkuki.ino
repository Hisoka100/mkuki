/**************************************************************************
*@file mkuki.ino
*@brief Learning FreeRTOS
*@author Peter Gichuhi
**************************************************************************/

#include "src/apps/apps.h"
#include <FreeRTOS_SAMD21.h>
#include <queue.h>
uint8_t app_to_run = APP_MKUKI;
TaskHandle_t xTaskReadSM;
TaskHandle_t xTaskReadC02;
TaskHandle_t xTaskCueSensors;
TaskHandle_t xTaskCollectSensorData;

void setup() {

  Serial.begin(115200);
   soil_moisture_sensor_setup();
   c02_sensor_setup();
   payload_setup();

  xTaskCreate(TaskSoilMoisture, "sm_task", 128, NULL,2, &xTaskReadSM);
  xTaskCreate(TaskReadC02, "c02_task", 128, NULL,2, &xTaskReadC02);
  xTaskCreate(TaskCueSensors, "cue_task", 128, NULL,1, &xTaskCueSensors);
  xTaskCreate(TaskCollectSensorData, "sm2_task", 128, NULL,3,&xTaskCollectSensorData);
  vTaskStartScheduler();
  // app_init(app_to_run);

}

void loop() {
  //  app_run(app_to_run);
}
