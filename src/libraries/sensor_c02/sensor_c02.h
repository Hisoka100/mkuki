/**************************************************************************
*@file   sensor_c02.h
*@brief  C02 header file
*@author Peter Gichuhi
**************************************************************************/
#ifndef SENSOR_C02
#define SENSOR_C02

#ifndef LIBRARIES_LIBRARISE_H_
#include "../libraries.h"
#endif


/*hardware baud rate, should not change*/
#define MHZ19_BAUD      9600 



void c02_sensor_setup(void);
void c02_sensor_test_service(void);
void c02_sensor_service(void);
void TaskReadC02(void * pvParameters);

#endif