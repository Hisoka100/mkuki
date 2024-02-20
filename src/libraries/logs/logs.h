#ifndef LOGS
#define LOGS

#ifndef LIBRARIES_LIBRARISE_H_
#include "../libraries.h"
#endif

/* uncomment to enable debug */
// #define LOGS


#ifdef LOGS
#define DEBUG_BUFF_SIZE 255
#else
#define DEBUG_BUFF_SIZE 0
#endif

extern char debug_buff[DEBUG_BUFF_SIZE];

void log(char * _str);
void log_setup(void);
uint8_t get_serial_hex_data( uint8_t * buffer, uint8_t len );


#endif