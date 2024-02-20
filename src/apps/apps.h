/**************************************************************************
*@file apps.h
*@brief Logic for choosingg which app is running
*@author Peter Gichuhi
**************************************************************************/

#ifndef APPS_H
#define APPS_H


/*User Drivers*/
#ifndef LIBRARIES_LIBRARIES_H_
#include "../libraries/libraries.h"
#endif

/*User Apps*/
#ifndef RUN_APP_H
#include "run_app/run_app.h"
#endif

#define APP_MKUKI        1
#define APP_RND          2


extern volatile uint8_t active_app;

void app_init(uint8_t app_name);
void app_run(uint8_t app_name);


#endif