#include "ds1307.h"

#include <stdint.h>
#include <stdlib.h>

#include "i2c_master.h"

#define DS1307_TIME_BUFFER_SIZE 9

uint8_t ds1307_time_buffer[DS1307_TIME_BUFFER_SIZE];
uint8_t ds1307_tmp_buffer[1];

uint8_t ds1307_set_time(Time *time) {
    ds1307_time_buffer[0] = 0x00;
    ds1307_time_buffer[1] = time->seconds;
    ds1307_time_buffer[2] = time->minutes;
    ds1307_time_buffer[3] = time->hours | DS1307_24_HOUR_MODE;
    ds1307_time_buffer[4] = time->day;
    ds1307_time_buffer[5] = time->date;
    ds1307_time_buffer[6] = time->month;
    ds1307_time_buffer[7] = time->year;

    ds1307_time_buffer[8] = DS1307_SW_OFF;

    if (is_ic2_transmitting()) {
        return 0;
    }

    i2c_send_bytes(ds1307_time_buffer, ds1307_tmp_buffer,
                   DS1307_TIME_BUFFER_SIZE, 0, DS1307_ADDRESS);

    return 1;
}

uint8_t ds1307_request_time() {
    ds1307_tmp_buffer[0] = 0x00;

    if (is_ic2_transmitting()) {
        return 0;
    }

    i2c_send_bytes(ds1307_tmp_buffer, ds1307_time_buffer, 1,
                   DS1307_TIME_BUFFER_SIZE - 1, DS1307_ADDRESS);

    return 1;
}

void ds1307_get_time(Time *time) {
    time->seconds = ds1307_time_buffer[0] & 0x7F;
    time->minutes = ds1307_time_buffer[1] & 0x7F;
    time->hours = ds1307_time_buffer[2] & 0x3F;
    time->day = ds1307_time_buffer[3] & 0x07;
    time->date = ds1307_time_buffer[4] & 0x3F;
    time->month = ds1307_time_buffer[5] & 0x1F;
    time->year = ds1307_time_buffer[6] & 0xFF;
}
