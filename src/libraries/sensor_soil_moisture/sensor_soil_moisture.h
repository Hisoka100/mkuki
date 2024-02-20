/**************************************************************************
*@file   sensor_soil_moisture.h
*@brief  Soil moisture logic headers
*@author Peter Gichuhi
**************************************************************************/
#ifndef SYNNEFA_SENSOR_SOIL_MOISTURE
#define SYNNEFA_SENSOR_SOIL_MOISTURE

#ifndef LIBRARIES_LIBRARISE_H_
#include "../libraries.h"
#endif




/*TODO: should come from settings*/

// unsigned long soil_moisture_sensor_inverval = 500; /*500 mS*/

void soil_moisture_sensor_setup(void);
void soil_moisture_test_service(void);
void soil_moisture_service(void);
void TaskSoilMoisture(void * pvParameters);



#endif
