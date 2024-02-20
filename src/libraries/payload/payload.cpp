/**************************************************************************
*@file   payload.cpp
*@brief  payload main file
*@author Peter Gichuhi
**************************************************************************/
#include "payload.h"

extern QueueHandle_t queue_1;
const uint8_t fw_ver[] = {FIRMWARE_VERSION};

uint8_t resp[] = "<SETTINGS:OK>";

unsigned long payload_tick_count = 0;
unsigned long payload_millis_counter = 0;

/*global variables*/
uint8_t payload_scratchpad[MAX_PAYLOAD_LENGTH];
bool spear_serial_sensor_type_updated;
sensor_data  payload_data_store[NUM_OF_SENSORS];
sensor_data  payload_data_stores;

uint8_t payload_build_pkt( void )
{
    uint8_t i = 0; // data counter
    
    /*clean scratchpad*/ 
    memset(payload_scratchpad, 0, sizeof(payload_scratchpad));

    /*add spear data id*/
    payload_scratchpad[i++] = SLAVE_DATA; 
    i++; /*leave room for length*/

    // add id
    payload_scratchpad[i++] = SLAVE_ID; 
    payload_scratchpad[i++] = sizeof(active_system_settings.serial_number); 
    memcpy(&payload_scratchpad[i], active_system_settings.serial_number, sizeof(active_system_settings.serial_number)); 
    i += sizeof(active_system_settings.serial_number);

    // add spear fw version
    payload_scratchpad[i++] = FIRMWARE_VERSION; 
    payload_scratchpad[i++] = sizeof(fw_ver); 
    memcpy(&payload_scratchpad[i], fw_ver, sizeof(fw_ver)); 
    i += sizeof(fw_ver);

    // add sensor data
    uint8_t sensor_count = 0;
    for( sensor_count; sensor_count < NUM_OF_SENSORS; sensor_count++ )
    {
        if( true == payload_data_store[sensor_count].new_data )
        {
            payload_scratchpad[i++] = payload_data_store[sensor_count].id;
            payload_scratchpad[i++] = payload_data_store[sensor_count].byte_count;
            
            memcpy( &payload_scratchpad[i], 
                    payload_data_store[sensor_count].bytes, 
                    payload_data_store[sensor_count].byte_count);

            i += payload_data_store[sensor_count].byte_count;

            // don't send old data
            payload_data_store[sensor_count].new_data = false;
        }
    }

    // add the pkt length
    payload_scratchpad[1] = i;

    return i;
}

void payload_tick( void )//Can be replaced using rask scheduler
{
    if( (STATE_LIVE == active_system_settings.op_state) &&
        (active_system_settings.battery_low_threshold < get_battery_mV()) )
    {
        if( (millis() - payload_millis_counter) >= ONE_SECOND )
        {
           payload_tick_count++;
#ifdef LOG_IGH_SPEAR_PAYLOAD
            igh_spear_log(".");
#endif
#ifdef DIGITAL_SWITCH_DEBUG
             if(payload_tick == (active_system_setting.data_collection_interval - active_system_setting.sensor_heating_time))
                 {
                    igh_digital_switch_on();
                    //Turn digital switches 5 seconds before collecting data
                     #ifdef IGH_SPEAR_DEBUG
                         Serial.println("Digital switch state : ON");
                     #endif
                 }
#endif
            payload_millis_counter = millis();
         
        }

        if( payload_tick_count >= active_system_settings.data_collection_interval )
        {
            payload_tick_count = 0;
            uint8_t num_bytes_to_send = 0;

            // collect data
             payload_collect_sensor_data();
 
            // build the data
            num_bytes_to_send = payload_build_pkt();
            
            // if it is time, send it into the ether, don't store data on the spear
#ifdef LOG_IGH_SPEAR_PAYLOAD
            igh_spear_log("\nPAYLOAD --> ");
            for( int i = 0; i < num_bytes_to_send; i++ )
            {
                sprintf(debug_buff, "%02X", payload_scratchpad[i]);
                igh_spear_log(debug_buff);
            }
            igh_spear_log("\n");
#endif
            // synnefa_spear_rfm69_send_2_shield( active_system_settings.parent_shield_rf_id, payload_scratchpad, num_bytes_to_send );
         
        }

    }

    // process any new settings messages
     get_new_settings();

    // process_shield_message();
}

void get_new_settings( void )
{
    uint8_t _buffer[MAX_PAYLOAD_LENGTH];
    memset( _buffer, 0, sizeof(_buffer) );

    if ( true == get_serial_hex_data(_buffer, sizeof(_buffer)) ) // via Serial
    {
        // only process seetings from specific rf device
        if( _buffer[0] != SEND_SETTINGS ) return;  // do nothing if first byte is not a rx settings command

        uint8_t len = _buffer[1];

        uint8_t i = 2;

        new_system_settings = active_system_settings;
        while( i < len )
        {
            uint8_t processed_len = payload_parse_new_settings( &_buffer[i]);

            if( 0 == processed_len )
            {
                // drop message because the payload is unknown
                return;
            }
            // else, move on to next message
            i += processed_len;
        }

        uint8_t new_checksum = calculate_settings_checksum( &new_system_settings, sizeof(new_system_settings));
        new_system_settings.checksum = new_checksum;

        if( true == save_settings(new_system_settings) )
        {
            read_settings( &active_system_settings );

        }
    }
}

uint8_t payload_parse_new_settings( uint8_t * data )
{
    uint8_t len = data[1];
    uint8_t pkt_len = 0;
    uint16_t _16bit_buffer = 0;
    switch( data[0] )
    {
        case SLAVE_ID:
            if( len == sizeof(new_system_settings.serial_number) )
            {
                memcpy(new_system_settings.serial_number, &data[2], len);
                pkt_len = sizeof(new_system_settings.serial_number) + SIZE_OF_MSG_HEADER;
            }
            break;
        case OP_STATE:
            if( len == SIZE_OF_OP_STATE )
            {
                new_system_settings.op_state = data[2];
                pkt_len = SIZE_OF_OP_STATE + SIZE_OF_MSG_HEADER;
            }
            break;
        case SLAVE_SERIAL_SENSOR_TYPE:
            if( len == SIZE_OF_SPEAR_SERIAL_SENSOR_TYPE )
            {
                new_system_settings.serial_sensor_type = data[2];
                pkt_len = SIZE_OF_SPEAR_SERIAL_SENSOR_TYPE + SIZE_OF_MSG_HEADER;
                spear_serial_sensor_type_updated = true;
            }
            break;
        case SLAVE_RF_ID :
            if( len == SIZE_OF_RF_ID )
            {
                _16bit_buffer = data[2] | ( data[3] << 8 );
                new_system_settings.spear_rf_id = _16bit_buffer;
                pkt_len = SIZE_OF_RF_ID + SIZE_OF_MSG_HEADER;
            }
            break;
        case MASTER_DEVICE_RF_ID:
            if( len == SIZE_OF_RF_ID )
            {
                _16bit_buffer = data[2] | ( data[3] << 8 );
                new_system_settings.parent_shield_rf_id = _16bit_buffer;
                pkt_len = SIZE_OF_RF_ID + SIZE_OF_MSG_HEADER;
            }
            break;
        case SLAVE_BATT_LOW_THRESHOLD:
            if( len == SIZE_OF_BATT_THRESHOLD )
            {
                _16bit_buffer = data[2] | ( data[3] << 8 );
                new_system_settings.battery_low_threshold = _16bit_buffer;
                pkt_len = SIZE_OF_BATT_THRESHOLD + SIZE_OF_MSG_HEADER;
            }
            break;
        case SEND_INTERVAL:
            if( len == SIZE_OF_SEND_INTERVAL )
            {
                _16bit_buffer = data[2] | ( data[3] << 8 ) | ( data[4] << 16 ) | ( data[5] << 24 );
                new_system_settings.data_collection_interval = _16bit_buffer;
                pkt_len = SIZE_OF_SEND_INTERVAL + SIZE_OF_MSG_HEADER;
            }
            break;

        case HEATING_TIME:
            if( len == SIZE_OF_HEAT_TIME )
            {
               // EEROR
                _16bit_buffer = data[2] | ( data[3] << 8 ) | ( data[4] << 16 ) | ( data[5] << 24 );
                new_system_settings.sensor_heating_time = _16bit_buffer;
                pkt_len = SIZE_OF_HEAT_TIME + SIZE_OF_MSG_HEADER;
            }
            break;
        case NPK_TYPE:
            if( len == SIZE_OF_NPK_TYPE)
            {
                new_system_settings.npk_sensor_type = data[2];
                pkt_len = SIZE_OF_NPK_TYPE + SIZE_OF_MSG_HEADER;
                spear_serial_sensor_type_updated = true;
            }
            break;
        case TTL_TYPE:
            if( len == SIZE_OF_TTL_TYPE)
            {
                new_system_settings.ttl_type_e = data[2];
                pkt_len = SIZE_OF_TTL_TYPE + SIZE_OF_MSG_HEADER;
                spear_serial_sensor_type_updated = true;
            }
            break;
        default:
            // ???
            break;
    }
    return pkt_len;
}

/* data collection routines */
void payload_collect_sensor_data( void )
{
    hardware_battery_service();

    if(SERIAL_SENSOR_CO2 == active_system_settings.serial_sensor_type){
       c02_sensor_service(); 
       soil_moisture_service();    
       delay(500);

    }
    
    else if( (SERIAL_SENSOR_CO2 != active_system_settings.serial_sensor_type) || (true == spear_serial_sensor_type_updated) ){
        soil_moisture_service();
        npk_sensor_service();
        delay(500);
   
    }
}

void TaskStoreSoilMoisture(void * pvParameters){
sensor_data ulRecievedValue;
portBASE_TYPE xStatus;
TickType_t xReceiveDelay = pdMS_TO_TICKS( 11000); 
UBaseType_t xItemsInQueue;

    while(1){
        // if (xSemaphoreTake(sensorSemaphore, portMAX_DELAY) == pdTRUE)
        // {
            
        xItemsInQueue = uxQueueMessagesWaiting(queue_1);
        Serial.print("Items in queue");
        Serial.println(xItemsInQueue);
        if (xItemsInQueue != 0)
        {
			Serial.println("Queue should have been empty!\n");
		}
        uint8_t sensor_cont = 0;

         for( sensor_cont; sensor_cont < NUM_OF_SENSORS; sensor_cont++ )
         {
            Serial.println("inside the for loop");
            xStatus = xQueueReceive(queue_1, &ulRecievedValue, portMAX_DELAY);
            // Serial.println(xReceived);
        	if (xStatus== pdPASS) {
			// Serial.print("Received = ");
			// Serial.println(ulRecievedValue.id);
            switch(ulRecievedValue.id){
                case SOIL_MOISTURE:
                Serial.println("Soil moisture received");
                break;
                case CARBON_DIOXIDE:
                Serial.println("Carbon dioxide received");
                break;
                case 0:
                Serial.println("N0 data");
                break;
                default:
                break;
            }     
        }
		else {
			Serial.println("Could not receive from the queue!\n");
		}
     
        }
        // vTaskResume(xTaskReadSM);
        // vTaskResume(xTaskReadC02);
        vTaskDelay(xReceiveDelay);
    }
//   }
}