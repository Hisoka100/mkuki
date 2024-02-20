/**************************************************************************
*@file   sensor_temp.h
*@brief  spear settings header file
*@author Peter Gichuhi
**************************************************************************/
#ifndef SETTINGS
#define SETTINGS

#ifndef LIBRARIES_LIBRARISE_H_
#include "../libraries.h"
#endif

typedef enum _operation_state_e
{
    STATE_SHIPPING = 0x00,
    STATE_LIVE = 0x42
}operation_state_e;

typedef enum _serial_sensor_type_e
{
    SERIAL_SENSOR_NPK = 0x01,
    SERIAL_SENSOR_CO2 = 0x02
}serial_sensor_type_e;

typedef enum _npk_sensor_type_e
{
    SHORT_NPK = 0x01,
    LONG_NPK = 0x02
}npk_sensor_type_e;

typedef enum _ttl_type_e
{
    HW_97 = 0x01,
    HW_726 = 0x02
}ttl_type_e;

typedef struct settings
{
    uint8_t checksum;
    uint8_t op_state;
    uint8_t serial_number[12];
    uint16_t parent_shield_rf_id;
    uint16_t spear_rf_id;
    uint16_t battery_low_threshold;
    unsigned long data_collection_interval;
    unsigned long sensor_heating_time;
    uint8_t serial_sensor_type;
    uint8_t npk_sensor_type;
    uint8_t ttl_type_e;
}settings;

extern settings active_system_settings;
extern settings new_system_settings;
extern const uint8_t fw_ver[];




void settings_get_defaults( void );
void settings_setup(void);
void settings_test_service(void);
void read_settings(settings * settings_buffer );
bool save_settings(settings settings_read);
uint8_t calculate_settings_checksum(void * p_struct, size_t total_bytes);

#endif