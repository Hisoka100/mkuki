/**************************************************************************
*@file   hw_service.cpp
*@brief  hardware service main file
*@author Peter Gichuhi
**************************************************************************/
#include "hw_service.h"

uint16_t battery_voltage = 0;

unsigned long heartbeat_timer       = 0; // keep track of heartbeat
unsigned long heartbeat_state_time  = 0;
unsigned long vbatt_refresh_timer  = 0;

void hardware_setup(void)
{
    pinMode(INBUILT_LED, OUTPUT);
    
    analogReadResolution(12); // read the full resolution of the ADC on the SAMD M0 MCU

    pinMode(VBAT_SENSE, INPUT);
    heartbeat_state_time = HEART_ON_TIME;
    payload_data_store[SLAVE_BATTERY_LEVEL].id = SLAVE_BATTERY_LEVEL;
    payload_data_store[SLAVE_BATTERY_LEVEL].new_data = false;
    payload_data_store[SLAVE_BATTERY_LEVEL].byte_count = 2;
}

uint16_t get_raw_battery_voltage(void)
{
    return (uint16_t)analogRead(VBAT_SENSE);
}

uint16_t get_battery_mV( void )
{
    battery_voltage = get_raw_battery_voltage();
    int buff = (battery_voltage * BATTERY_MULTIPLIER);
    return (uint16_t)buff;
}

void hardware_heartbeat(void)
{
    if( (millis() - heartbeat_timer) > heartbeat_state_time )
    {
        digitalWrite(INBUILT_LED, !digitalRead(INBUILT_LED));
        heartbeat_state_time = ( digitalRead(INBUILT_LED) ? HEART_ON_TIME : HEART_OFF_TIME );
        heartbeat_timer = millis();
    }
}


void hardware_battery_test_service(void)
{
    battery_voltage = get_raw_battery_voltage();
}

void hardware_battery_service(void)
{
    battery_voltage = get_raw_battery_voltage();
    int buff = (battery_voltage * BATTERY_MULTIPLIER); // this has a heavy toll on RAM
    uint16_t voltage = (uint16_t)buff; // 1.61 is the scaling factor 

    payload_data_store[SLAVE_BATTERY_LEVEL].bytes[0] = voltage & 0xFF;
    payload_data_store[SLAVE_BATTERY_LEVEL].bytes[1] = (voltage >> 8);
    payload_data_store[SLAVE_BATTERY_LEVEL].new_data = true;
}