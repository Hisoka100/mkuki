/**************************************************************************
*@file  sensor_soil_moisture.cpp
*@brief Soil moisture logic headers
*@author Peter Gichuhi
**************************************************************************/
#include "sensor_soil_moisture.h"

extern TaskHandle_t xTaskReadSM;
extern QueueHandle_t sensor_data_queue;
uint16_t soil_moisture;
unsigned long soil_moisture_sensor_inverval = 500; 
unsigned long soil_moisture_sensor_timer = 0;
TickType_t xDelay500ms = pdMS_TO_TICKS(5000);

void soil_moisture_sensor_setup(void){

    pinMode(SOIL_MOISTURE_SENSOR, INPUT);
    soil_moisture_sensor_timer = millis();

    payload_data_store[SENSOR_SOIL_MOISTURE].id = SOIL_MOISTURE;
    payload_data_store[SENSOR_SOIL_MOISTURE].new_data = false;
    payload_data_store[SENSOR_SOIL_MOISTURE].byte_count = 2;
}

void soil_moisture_test_service(void){
    soil_moisture = 0;
    delay(soil_moisture_sensor_inverval);
    soil_moisture = analogRead(SOIL_MOISTURE_SENSOR);

    if(0 != soil_moisture)
    {
      Serial.println("");
    }
    else{
      Serial.println("");
    }
}

void soil_moisture_service(void){
        soil_moisture_sensor_setup();
        soil_moisture=0;
        soil_moisture = analogRead(SOIL_MOISTURE_SENSOR);
        Serial.print("Soil Moisture: "); Serial.println(soil_moisture);
        payload_data_store[SENSOR_SOIL_MOISTURE].bytes[0] = soil_moisture & 0xFF;
        payload_data_store[SENSOR_SOIL_MOISTURE].bytes[1] = (soil_moisture >> 8);
        payload_data_store[SENSOR_SOIL_MOISTURE].new_data = true;


}

void TaskSoilMoisture(void * pvParameters){
    TickType_t xNextWakeTime;
    portBASE_TYPE xReturned;
    uint16_t sm;

    xNextWakeTime = xTaskGetTickCount();

    while(1){
        Serial.println("Soil Moisture task ");
       
        sm = analogRead(SOIL_MOISTURE_SENSOR);
        // Serial.print("Soil Moisture :"); Serial.println(sm);
        payload_data_stores.id = SOIL_MOISTURE;
        payload_data_stores.byte_count = 2;
        payload_data_stores.bytes[0] = sm & 0xFF;
        payload_data_stores.bytes[1] = (sm >> 8);
        payload_data_stores.new_data = true;
        // Serial.println(payload_data_store[SENSOR_SOIL_MOISTURE].id );
        xReturned = xQueueSendToBack(sensor_data_queue, &payload_data_stores, portMAX_DELAY);
        // Serial.println(xReturned);
        // xSemaphoreGive(sensorSemaphore);
         vTaskDelayUntil(&xNextWakeTime, mainQUEUE_TICK_COUNT_FOR_1S);
        // vTaskSuspend(xTaskReadSM);
    }
}

