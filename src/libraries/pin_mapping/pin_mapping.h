/**************************************************************************
*@file   pin_mapping.h
*@brief  Pin mapping logic headers
*@author Peter Gichuhi
**************************************************************************/
#ifndef SYNNEFA_PIN_MAPPING
#define SYNNEFA_PIN_MAPPING

/*Digital switch pin*/
#define IGH_DIGITAL_SWITCH_PIN 12

/* RFM69 pins */
#define RFM69_CS        8 // must be set as input pullup
#define RFM69_IO0       3
#define RFM69_IRQ       3 // should be the same as IO0 coz of library
#define RFM69_RST       4

/* On board led */
#define INBUILT_LED  13
#define RE             A0
#define DE             A1

/* VBAT Sense */
#define VBAT_SENSE      9// or pin 9

/* Analog Soil Moisture sensor */
#define SOIL_MOISTURE_SENSOR     A2

/* DHT22 Air Temperature and Humidity */
#define DHT_DATA        5 

/* Soil Temperature*/
#define TEMP_SENSOR_ONE_WIRE_BUS 10
#endif