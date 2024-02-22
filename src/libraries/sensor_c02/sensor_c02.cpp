/**************************************************************************
*@file  sensor_c02.cpp
*@brief C02 main file
*@author Peter Gichuhi
**************************************************************************/
#include "sensor_c02.h"
SemaphoreHandle_t sensorSemaphore = xSemaphoreCreateBinary();
extern TaskHandle_t xTaskReadC02;
extern TaskHandle_t xTaskCollectSensorData;
MHZ19 myMHZ19;

uint16_t mhz19_co2;

unsigned long mhz19_timer = 0;
/*TODO: Should come from settings*/
unsigned long mhz19_read_interval = 2000; /*minimum 2 seconds for accuracy*/ 
// extern QueueHandle_t queue_1;
void c02_sensor_setup(void){
    payload_data_store[SENSOR_CARBON_DIOXIDE].id = CARBON_DIOXIDE;
    payload_data_store[SENSOR_CARBON_DIOXIDE].new_data = false;
    payload_data_store[SENSOR_CARBON_DIOXIDE].byte_count = 4;

    Serial1.begin(MHZ19_BAUD);
    myMHZ19.begin(Serial1);

    mhz19_timer = millis();
}


void c02_sensor_test_service(void){
    c02_sensor_setup();
    delay(mhz19_read_interval);

    mhz19_co2 = 0;
    mhz19_co2 = (uint16_t)myMHZ19.getCO2Raw();

    if(mhz19_co2 != 0){

        Serial.println("MHZ19..................OK\n");
    }
    else{

        Serial.println("MHZ19..................ERROR\n");

    }

}

void c02_sensor_service(void){
    c02_sensor_setup();
    uint32_t mhz19_co2_ppm = 0;
    mhz19_co2_ppm = (uint32_t)myMHZ19.getCO2();
    Serial.print("C02: "); Serial.print(mhz19_co2_ppm);Serial.println("ppm");
    
    payload_data_store[SENSOR_CARBON_DIOXIDE].bytes[0] = mhz19_co2_ppm & 0xFF;
    payload_data_store[SENSOR_CARBON_DIOXIDE].bytes[1] = (mhz19_co2_ppm >> 8);
    payload_data_store[SENSOR_CARBON_DIOXIDE].bytes[2] = (mhz19_co2_ppm >> 16);
    payload_data_store[SENSOR_CARBON_DIOXIDE].bytes[3] = (mhz19_co2_ppm >> 24);
    payload_data_store[SENSOR_CARBON_DIOXIDE].new_data = true;

    mhz19_timer = millis();
}

void TaskReadC02(void * pvParameters){
    TickType_t xNextWakeTime;
    portBASE_TYPE xReturned;
    uint32_t mhz19_co2_ppm = 0;

    xNextWakeTime = xTaskGetTickCount();
   
    while(1){
         Serial.println("C02 task");
        mhz19_co2_ppm = (uint32_t)myMHZ19.getCO2();

        payload_data_store[CARBON_DIOXIDE].id = CARBON_DIOXIDE;
        payload_data_store[CARBON_DIOXIDE].byte_count = 2;
        payload_data_store[CARBON_DIOXIDE].bytes[0] = mhz19_co2_ppm & 0xFF;
        payload_data_store[CARBON_DIOXIDE].bytes[1] = (mhz19_co2_ppm >> 8);
        payload_data_store[CARBON_DIOXIDE].new_data = true;  
        xReturned = xQueueSendToBack(sensor_data_queue, &payload_data_store[CARBON_DIOXIDE], portMAX_DELAY);

        if(xReturned == pdTRUE){
         vTaskDelayUntil(&xNextWakeTime, mainQUEUE_TICK_COUNT_FOR_1S);
         vTaskResume(xTaskCollectSensorData);
         vTaskSuspend(xTaskReadC02);
        }        
         

    }
}