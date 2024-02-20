#ifndef LIBRARIES_LIBRARISE_H_
#define LIBRARIES_LIBRARISE_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <SPI.h>
#include<DallasTemperature.h>
#include<OneWire.h>
#include <FreeRTOS_SAMD21.h>
#include <queue.h>


#include "Arduino.h"
#include "../arduino_libs/mhz19/MHZ19.h"
#include "../arduino_libs/dht22/DHT.h"
#include "../arduino_libs/lux_meter/BH1750.h"
// #include "../arduino_libs/OneWire/OneWire.h"
// #include "../arduino_libs/DallasTemperature/DallasTemperature.h"
#include "../arduino_libs/flash/FlashStorage.h"
#include "../arduino_libs/rfm69_hal/RFM69.h"



#ifndef HARDWARE_SERVICE
#include "hw_service/hw_service.h"
#endif

#ifndef PAYLOAD
#include "payload/payload.h"
#endif

#ifndef PIN_MAPPING
#include "pin_mapping/pin_mapping.h"
#endif


#ifndef SENSOR_C02
#include "sensor_c02/sensor_c02.h"
#endif


#ifndef SENSOR_NPK
#include "sensor_npk/sensor_npk.h"
#endif

#ifndef SENSOR_SOIL_MOISTURE
#include "sensor_soil_moisture/sensor_soil_moisture.h"
#endif


#ifndef LOGS
#include "logs/logs.h"
#endif

#ifndef SETTINGS
#include "settings/settings.h"
#endif

#define  WAIT_MS   10000
#define  WAIT_MSS   10500
#define mainQUEUE_TICK_COUNT_FOR_1S			pdMS_TO_TICKS( WAIT_MS )
#define mainQUEUE_TICK_COUNT_FOR_2S			pdMS_TO_TICKS( WAIT_MSS )

extern QueueHandle_t queue_1;
extern SemaphoreHandle_t sensorSemaphore;

extern TaskHandle_t xTaskReadSM;
extern TaskHandle_t xTaskReadC02;
#endif