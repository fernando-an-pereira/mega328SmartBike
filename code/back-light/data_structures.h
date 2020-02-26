#ifndef _DATA_STRUCTURES_H
#define _DATA_STRUCTURES_H

#include <stdint.h>

typedef struct _buffer {
    uint8_t* data;
    uint8_t remaining;
} Buffer;

typedef struct _time {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day;
    uint8_t date;
    uint8_t month;
    uint8_t year;
} Time;

#endif
