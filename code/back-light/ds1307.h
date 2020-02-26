#ifndef DS1307_H_
#define DS1307_H_

#include <stdint.h>

#include "data_structures.h"

#define DS1307_ADDRESS 0b01101000

#define DS1307_RS0 0
#define DS1307_RS1 1
#define DS1307_SQWE 4
#define DS1307_OUT 7

#define DS1307_12_H 6

#define DS1307_SW_OFF                                             \
    ((0 << DS1307_OUT) | (0 << DS1307_SQWE) | (0 << DS1307_RS1) | \
     (0 << DS1307_RS0))

#define DS1307_12_HOUR_MODE (1 << DS1307_12_H)
#define DS1307_24_HOUR_MODE (0 << DS1307_12_H)

#define SUNDAY "Sunday"
#define MONDAY "Monday"
#define TUESDAY "Tuesday"
#define WEDNESDAY "Wednesday"
#define THURSDAY "Thursday"
#define FRIDAY "Friday"
#define SATURDAY "Saturday"

#define WEEK_DAYS \
    { SUNDAY, MONDAY, TUESDAY, WEDNESDAY, TUESDAY, THURSDAY, FRIDAY, SATURDAY }

uint8_t ds1307_set_time(Time *time);
uint8_t ds1307_request_time();
void ds1307_get_time(Time *time);

#endif