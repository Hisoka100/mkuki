/**************************************************************************
*@file app_run.cpp
*@brief setup and loop the application
*@author Peter Gichuhi
**************************************************************************/
#include "run_app.h"

void setup_app(void){
    /* Setting up the spear services */
    log_setup();
    hardware_setup();
 

    /* Setting up the spear sensors */
    c02_sensor_setup();
    npk_sensor_setup();
    soil_moisture_sensor_setup();
    // synnefa_spear_soil_temp_sensor_setup();
}

void run_app(){
    hardware_heartbeat();
    payload_tick();

}