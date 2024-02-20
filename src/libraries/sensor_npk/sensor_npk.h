/**************************************************************************
*@file   sensor_npk.h
*@brief  NPK header file
*@author Peter Gichuhi
**************************************************************************/
#ifndef SENSOR_NPK
#define SENSOR_NPK

#ifndef LIBRARIES_LIBRARISE_H_
#include "../libraries.h"
#endif

#define LONG_NPK_BAUD   (9600)
#define SHORT_NPK_BAUD  (4800)


void npk_sensor_setup(void);
void npk_sensor_service(void);
uint8_t potassium();
uint8_t phosphorous();
uint8_t nitrogen();

#endif