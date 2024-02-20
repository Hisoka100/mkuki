/**************************************************************************
*@file settings.cpp
*@brief device settings main life
*@author Peter Gichuhi
**************************************************************************/
#include "settings.h"

// const uint8_t fw_ver_settings[]= {IGH_SPEAR_VERSION};
FlashStorage(settings_store, settings);

/*default system settings*/
uint8_t default_serial_number[]={0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint16_t default_master_rf_id = 0;
uint16_t default_slave_rf_id = 1;
uint8_t default_serial_sensor_type = SERIAL_SENSOR_CO2;
uint8_t default_npk_sensor_type = LONG_NPK;
uint8_t default_ttl_type = HW_726;
uint16_t default_low_bat_threshold = 3000; // 3000mV
unsigned long default_data_collection_interval = 300; // time in seconds
unsigned long default_sensor_heat_time = 10; //Time in seconds

settings active_system_settings;
settings default_system_settings;
settings new_system_settings;

void settings_get_defaults(void){
    memcpy( default_system_settings.serial_number, default_serial_number, sizeof(default_serial_number));
    default_system_settings.op_state = STATE_SHIPPING;
    default_system_settings.parent_shield_rf_id = default_master_rf_id ;
    default_system_settings.spear_rf_id = default_slave_rf_id ;
    default_system_settings.serial_sensor_type = default_serial_sensor_type;
    default_system_settings.battery_low_threshold = default_low_bat_threshold;
    default_system_settings.sensor_heating_time = default_sensor_heat_time;
    default_system_settings.data_collection_interval = default_data_collection_interval;
    default_system_settings.npk_sensor_type = default_npk_sensor_type;
    default_system_settings.ttl_type_e = default_ttl_type;
}
uint8_t calculate_settings_checksum(void * p_struct, size_t total_bytes){
    uint8_t length = (uint8_t)total_bytes;
    uint8_t *data;
    int sum = 0;
    uint8_t checksum = 0;

    data = (uint8_t*)p_struct;
    data +1;

    for(int i = 1;i<length;i++){
        sum +=(0xFF &*data++);
    }

    checksum = (uint8_t)(sum%256);
    return checksum;
}
void settings_setup(void){
     settings_get_defaults();
    /*Get settings from memory*/
    settings settings_in_memory;
    read_settings(&settings_in_memory);

    /*Compare the checksums*/
    uint8_t valid_checksum = 0;
    valid_checksum = calculate_settings_checksum(&settings_in_memory, sizeof(settings_in_memory));
    if((settings_in_memory.checksum != valid_checksum) || (0 == settings_in_memory.checksum)){
        /*Revert to default */
            active_system_settings = default_system_settings;
    }
    else{
        /*proceed with the system settings*/
        active_system_settings = settings_in_memory;
    }
}

void settings_test_service(void){
    /*create settings insatnce*/
    settings current_settings;
    // populate settings
    current_settings.checksum = 99;
    uint8_t sn[] = {0xa7,0xa7,0xa7,0xa7,0xa7,0xa7,0xa7,0xa7,0xa7,0xa7,0xa7,0xa7};
    memcpy(current_settings.serial_number, sn, sizeof(sn));
    if( true == save_settings(current_settings) )
    {
#ifdef SYNNEFA_LOG_SETTINGS
        igh_spear_log("SETTINGS...............OK\n");
#endif
    }
    else
    {
#ifdef SYNNEFA_LOG_SETTINGS
        igh_spear_log("SETTINGS...............ERROR\n");
#endif
    }
}

void read_settings(settings * settings_buffer ){
     *settings_buffer = settings_store.read();
}

bool save_settings(settings settings_read){
        uint8_t ret = 0;
    settings _read;

    settings_store.write(settings_read);

     read_settings(&_read);

    if ( _read.checksum == settings_read.checksum )
    {
        ret = 1;
    }
    return ret;
}

