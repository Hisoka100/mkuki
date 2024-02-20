/**************************************************************************
*@file  sensor_npk.cpp
*@brief  NPK main file
*@author Peter Gichuhi
**************************************************************************/
#include "sensor_npk.h"

const uint8_t nitro[] = {0x01,0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const uint8_t phos[] = {0x01,0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const uint8_t pota[] = {0x01,0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};

uint8_t values[11];

void npk_sensor_setup(void){
    payload_data_store[SENSOR_SOIL_NITROGEN].id = SOIL_NITROGEN;
    payload_data_store[SENSOR_SOIL_NITROGEN].new_data = false;
    payload_data_store[SENSOR_SOIL_NITROGEN].byte_count = 2;

    payload_data_store[SENSOR_SOIL_POTASSIUM].id = SOIL_POTASSIUM;
    payload_data_store[SENSOR_SOIL_POTASSIUM].new_data = false;
    payload_data_store[SENSOR_SOIL_POTASSIUM].byte_count = 2;

    payload_data_store[SENSOR_SOIL_PHOSPHOROUS].id = SOIL_PHOSPHOROUS;
    payload_data_store[SENSOR_SOIL_PHOSPHOROUS].new_data = false;
    payload_data_store[SENSOR_SOIL_PHOSPHOROUS].byte_count = 2;

    if(HW_97 == active_system_settings.npk_sensor_type){
        pinMode(RE, OUTPUT);
        pinMode(DE, OUTPUT);
    }

    if(LONG_NPK == active_system_settings.npk_sensor_type){
        Serial1.begin(LONG_NPK_BAUD);   
    }
    else if(SHORT_NPK == active_system_settings.npk_sensor_type)
    {
        Serial1.begin(SHORT_NPK_BAUD);
    }

}

void npk_sensor_service(void){
    npk_sensor_setup(); //set up the npk before collecting data
     //Serial.println("Inside NPK read loop");
    uint8_t n, p, k;
    delay(500);
    n = nitrogen();

    delay(500);
    p = phosphorous();
    
    delay(500);
   
    k = potassium();
    
    delay(250);

    payload_data_store[SENSOR_SOIL_NITROGEN].bytes[0] = n;
    payload_data_store[SENSOR_SOIL_NITROGEN].bytes[1] = 0;
    payload_data_store[SENSOR_SOIL_NITROGEN].new_data = true;

    payload_data_store[SENSOR_SOIL_PHOSPHOROUS].bytes[0] = p;
    payload_data_store[SENSOR_SOIL_PHOSPHOROUS].bytes[1] = 0;
    payload_data_store[SENSOR_SOIL_PHOSPHOROUS].new_data = true;

    payload_data_store[SENSOR_SOIL_POTASSIUM].bytes[0] = k;
    payload_data_store[SENSOR_SOIL_POTASSIUM].bytes[1] = 0;
    payload_data_store[SENSOR_SOIL_POTASSIUM].new_data = true;

}

uint8_t potassium(){
    uint8_t soil_pottasium;
    if(HW_726 == active_system_settings.ttl_type_e){
        if(Serial1.write(pota, sizeof(pota))==8){
            for(uint8_t i=0; i<7; i++){
                values[i] = Serial1.read();
            }
        }
        soil_pottasium=values[4];
    }
    else{
        digitalWrite(DE, HIGH);
        digitalWrite(RE, HIGH);
        delay(1);
        /*Write out the message*/
        for(uint8_t i = 0; i< sizeof(pota); i++)Serial1.write(pota[i]);
        /*Wait for the transmission to complete*/
        Serial1.flush();
        digitalWrite(DE, LOW);
        digitalWrite(RE, LOW);

        delay(200);
        /*Read the received bytes*/
        for (byte i=0; i<7; i++){
            values[i] = Serial1.read();
        }
        soil_pottasium = values[4];
    }
return soil_pottasium;
}
uint8_t phosphorous(){
    uint8_t soil_phosphorus;
    if(HW_726 == active_system_settings.ttl_type_e){
        if(Serial1.write(phos, sizeof(phos))==8){
            for(uint8_t i=0; i<7; i++){
                values[i] = Serial1.read();
            }
        }
        soil_phosphorus=values[4];
    }
    else{
        digitalWrite(DE, HIGH);
        digitalWrite(RE, HIGH);
        delay(1);
        /*Write out the message*/
        for(uint8_t i = 0; i< sizeof(phos); i++)Serial1.write(phos[i]);
        /*Wait for the transmission to complete*/
        Serial1.flush();
        digitalWrite(DE, LOW);
        digitalWrite(RE, LOW);

        delay(200);
        /*Read the received bytes*/
        for (byte i=0; i<7; i++){
            values[i] = Serial1.read();
        }
        soil_phosphorus = values[4];
    }
return soil_phosphorus;
}
uint8_t nitrogen(){
    uint8_t soil_nitrogen;
    if(HW_726 == active_system_settings.ttl_type_e){
        if(Serial1.write(nitro, sizeof(nitro))==8){
            for(uint8_t i=0; i<7; i++){
                values[i] = Serial1.read();
            }
        }
        soil_nitrogen=values[4];
    }
    else{
        digitalWrite(DE, HIGH);
        digitalWrite(RE, HIGH);
        delay(1);
        /*Write out the message*/
        for(uint8_t i = 0; i< sizeof(nitro); i++)Serial1.write(nitro[i]);
        /*Wait for the transmission to complete*/
        Serial1.flush();
        digitalWrite(DE, LOW);
        digitalWrite(RE, LOW);

        delay(200);
        /*Read the received bytes*/
        for (byte i=0; i<7; i++){
            values[i] = Serial1.read();
        }
        soil_nitrogen = values[4];
    }
return soil_nitrogen;

}