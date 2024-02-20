/**************************************************************************
*@file   payload.h
*@brief  payload header file
*@author Peter Gichuhi
**************************************************************************/
#ifndef PAYLOAD 
#define PAYLOAD

// #ifndef LIBRARIES_LIBRARISE_H_
#include "../libraries.h"
// #endif

typedef enum igh_pkt_id
{
    MSG_ACK_TUPLE             = 0x00,
    SLAVE_ID                  = 0x01,
    STORE_TIMESTAMP           = 0x02,
    SEND_TIMESTAMP            = 0x03,
    SOIL_MOISTURE             = 0x04,
    AIR_HUMIDITY              = 0x05,
    HEATING_TIME              = 0x06,
    WATER_DISPENSED           = 0x07,
    CARBON_DIOXIDE            = 0x08,
    AIR_TEMPERATURE           = 0x09,
    SOIL_TEMPERATURE          = 0x0A,
    SOIL_NITROGEN             = 0x0B,
    LIGHT_INTENSITY           = 0x0C,
    MASTER_BATTERY_LEVEL      = 0x0D,
    SLAVE_BATTERY_LEVEL       = 0x0E,
    VALVE_POSITION            = 0x0F,
    SEND_SETTINGS             = 0x10,
    READ_SETTINGS             = 0x11,
    SLAVE_DATA                = 0x12,
    SLAVE_RF_ID               = 0x13,
    MASTER_DEVICE_RF_ID       = 0x14,
    SEND_INTERVAL             = 0x15,
    OP_STATE                  = 0x16,
    SHIELD_ID                 = 0x17,
    SLAVE_BATT_LOW_THRESHOLD  =0x18,
    MASTER_BATT_LOW_THRESHOLD = 0x19,
    BUTTON_PRESS              = 0x1A,
    SOIL_POTASSIUM            = 0x1C,
    SOIL_PHOSPHOROUS          = 0x1D,
    SLAVE_SERIAL_SENSOR_TYPE  = 0x1E,
    SLAVE_FW_VERSION          = 0x1F,
    NPK_TYPE                  = 0x20,
    TTL_TYPE                  = 0x21,
    EVENT                     = 0xFC,
    RESTART                   = 0xFD,
    DATA_PKT                  = 0xFE,
    END_OF_PKT_ID             = 0xFF
} pkt_id;

enum sensors
{
    BATTERY_LEVEL,
    SENSOR_SOIL_MOISTURE,
    SENSOR_CARBON_DIOXIDE,
    SENSOR_SOIL_NITROGEN,
    SENSOR_SOIL_PHOSPHOROUS,
    SENSOR_SOIL_POTASSIUM,
    NUM_OF_SENSORS
};

typedef struct sensor_data
{
    pkt_id id;
    uint8_t byte_count; // min is one, max is 4
    uint8_t new_data;
    uint8_t bytes[4]; // bytes in little endian
} sensor_data; 



/**
 *  this is limited by the maximum 
 * bytes the RFM module can transmit 
 * with encryption
 * */
#define FIRMWARE_VERSION                0,0,3
#define SIZE_OF_MSG_HEADER               2
#define SIZE_OF_RF_ID                    2
#define SIZE_OF_BATT_THRESHOLD           2
#define SIZE_OF_OP_STATE                 1
#define SIZE_OF_SPEAR_SERIAL_SENSOR_TYPE 1
#define SIZE_OF_NPK_TYPE                 1
#define SIZE_OF_TTL_TYPE                 1
#define SIZE_OF_SEND_INTERVAL            4
#define SIZE_OF_HEAT_TIME                4
#define MAX_PAYLOAD_LENGTH               60 
#define ONE_SECOND                       1000
#define SETTINGS_CLIENT                  1984


extern sensor_data  payload_data_store[NUM_OF_SENSORS];
extern sensor_data  payload_data_stores;
extern bool sensor_type_updated;
// extern QueueHandle_t queue_1;

void payload_tick(void);
void payload_collect_sensor_data( void );
uint8_t payload_parse_new_settings( uint8_t * data );
void get_new_settings( void );
void TaskStoreSoilMoisture(void * pvParameters);

#endif