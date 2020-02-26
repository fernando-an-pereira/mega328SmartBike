#ifndef DS1307_H_
#define DS1307_H_

#include <stdint.h>

#define DS1307_ADDRESS 0b01101000

void ds1307_request_time();
void ds1307_get_time(uint8_t *sec_h, uint8_t *sec_l, uint8_t *min_h, uint8_t *min_l, uint8_t *hour_h, uint8_t *hour_l, uint8_t *day, uint8_t *date_h, uint8_t *date_l, uint8_t *month_h, uint8_t *month_l, uint8_t *year_h, uint8_t *year_l);
void ds1307_read_time(uint8_t *sec_h, uint8_t *sec_l, uint8_t *min_h, uint8_t *min_l, uint8_t *hour_h, uint8_t *hour_l, uint8_t *day, uint8_t *date_h, uint8_t *date_l, uint8_t *month_h, uint8_t *month_l, uint8_t *year_h, uint8_t *year_l);
void ds1307_set_time_zero();

#endif