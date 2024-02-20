/**************************************************************************
*@file   hw_service.h
*@brief  hardware service 
*@author Peter Gichuhi
**************************************************************************/
#ifndef HARDWARE_SERVICE
#define HARDWARE_SERVICE

#ifndef LIBRARIES_LIBRARISE_H_
#include"../libraries.h"
#endif

extern uint16_t battery_voltage;

/* Onboard led */
#define BATTERY_MULTIPLIER (1.6117)
#define HEART_PERIOD       5000 /*ms*/
#define HEART_ON_TIME      50   /*ms*/
#define HEART_OFF_TIME     (HEART_PERIOD - HEART_ON_TIME)

// extern unsigned long heartbeat_timer       = 0; /*keep track of heartbeat*/
// extern unsigned long heartbeat_state_time  = 0;

/* battery */
#define BATTERY_REFRESH_FREQUENCY 1000 /*check battery voltage every 5 seconds*/


uint16_t get_raw_battery_voltage(void);
uint16_t get_battery_mV( void );
void hardware_heartbeat(void);
void hardware_setup(void);
void hardware_battery_test_service(void);
void hardware_battery_service(void);

#endif