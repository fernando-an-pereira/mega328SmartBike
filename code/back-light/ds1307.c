#include "ds1307.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <stdint.h>
#include <util/delay.h>

#include "i2c_master.h"

volatile uint8_t ds1307_data[9];
volatile uint8_t ds1307_tmp[1];

void ds1307_request_time() {
    ds1307_tmp[0] = 0x00;
    i2c_send_bytes(ds1307_tmp, ds1307_data, 1, 8, DS1307_ADDRESS);
}

void ds1307_get_time(uint8_t *sec_h, uint8_t *sec_l, uint8_t *min_h,
                     uint8_t *min_l, uint8_t *hour_h, uint8_t *hour_l,
                     uint8_t *day, uint8_t *date_h, uint8_t *date_l,
                     uint8_t *month_h, uint8_t *month_l, uint8_t *year_h,
                     uint8_t *year_l) {
    *sec_h = (ds1307_data[0] & 0x70) >> 4;
    *sec_l = ds1307_data[0] & 0x0F;
    *min_h = (ds1307_data[1] & 0x70) >> 4;
    *min_l = ds1307_data[1] & 0x0F;
    *hour_h = (ds1307_data[2] & 0x30) >> 4;
    *hour_l = ds1307_data[2] & 0x0F;
    *day = ds1307_data[3] & 0x07;
    *date_h = (ds1307_data[4] & 0x30) >> 4;
    *date_l = ds1307_data[4] & 0x0F;
    *month_h = (ds1307_data[5] & 0x10) >> 4;
    *month_l = ds1307_data[5] & 0x0F;
    *year_h = (ds1307_data[6] & 0xF0) >> 4;
    *year_l = ds1307_data[6] & 0x0F;
}

void ds1307_read_time(uint8_t *sec_h, uint8_t *sec_l, uint8_t *min_h,
                      uint8_t *min_l, uint8_t *hour_h, uint8_t *hour_l,
                      uint8_t *day, uint8_t *date_h, uint8_t *date_l,
                      uint8_t *month_h, uint8_t *month_l, uint8_t *year_h,
                      uint8_t *year_l) {
    ds1307_request_time();
    // _delay_ms(100);
    i2c_wait();
    ds1307_get_time(sec_h, sec_l, min_h, min_l, hour_h, hour_l, day, date_h,
                    date_l, month_h, month_l, year_h, year_l);
}

void ds1307_set_time_zero() {
    ds1307_data[0] = 0x00;
    ds1307_data[1] = 0x00;
    ds1307_data[2] = 0x00;
    ds1307_data[3] = 0x00;
    ds1307_data[4] = 0x01;
    ds1307_data[5] = 0x04;
    ds1307_data[6] = 0x11;
    ds1307_data[7] = 0x12;
    ds1307_data[8] = 0x10;
    i2c_send_bytes(ds1307_data, ds1307_tmp, 9, 0, DS1307_ADDRESS);
}