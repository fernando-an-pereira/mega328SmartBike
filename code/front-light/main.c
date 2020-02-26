#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <stdint.h>
#include <util/delay.h>

// #include "data_structures.h"
#include "ds1307.h"
#include "i2c_master.h"
#include "serial.h"

char int_to_char(uint8_t n) { return n + '0'; }

void convert_time_to_string(Time *time, char *time_string) {
    time_string[0] = '2';
    time_string[1] = '0';
    time_string[2] = int_to_char(time->year >> 4);
    time_string[3] = int_to_char(time->year & 0x0F);

    time_string[4] = '-';

    time_string[5] = int_to_char(time->month >> 4);
    time_string[6] = int_to_char(time->month & 0x0F);

    time_string[7] = '-';

    time_string[8] = int_to_char(time->date >> 4);
    time_string[9] = int_to_char(time->date & 0x0F);

    time_string[10] = 'T';

    time_string[11] = int_to_char(time->hours >> 4);
    time_string[12] = int_to_char(time->hours & 0x0F);

    time_string[13] = ':';

    time_string[14] = int_to_char(time->minutes >> 4);
    time_string[15] = int_to_char(time->minutes & 0x0F);

    time_string[16] = ':';

    time_string[17] = int_to_char(time->seconds >> 4);
    time_string[18] = int_to_char(time->seconds & 0x0F);

    time_string[19] = '\r';
    time_string[20] = '\0';
}

int main(void) {
    Time time;

    char string[32];

    DDRB |= (1 << DDB1);
    PORTB = 0;

    init_serial();
    init_i2c_master();

    sei();

    time.seconds = 0x00;
    time.minutes = 0x00;
    time.hours = 0x00;
    time.date = 0x01;
    time.month = 0x01;
    time.year = 0x20;
    time.day = 0x04;

    ds1307_set_time(&time);

    while (1) {
        if (!is_ic2_transmitting()) {
            ds1307_request_time();
        }
        _delay_ms(100);
        if (!is_ic2_transmitting()) {
            ds1307_get_time(&time);
            convert_time_to_string(&time, string);
            safe_transmit_string_serial(string);
        }
        _delay_ms(100);
    }
}