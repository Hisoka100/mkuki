/**************************************************************************
*@file apps.cpp
*@brief App initialization
*@author Peter Gichuhi
**************************************************************************/
#include "apps.h"

volatile uint8_t active_app = 0;

void app_init(uint8_t app_name){
    if(app_name == APP_MKUKI){
        setup_app();
    }

}

void app_run(uint8_t app_name){
        if(app_name == APP_MKUKI){
        run_app();
    }

}