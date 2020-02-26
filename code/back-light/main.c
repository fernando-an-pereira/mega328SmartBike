#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <stdint.h>
#include <util/delay.h>

#include "ds1307.h"
#include "i2c_master.h"
#include "serial.h"

char _convert_bcd_to_string(uint8_t hex) { return hex + '0'; }

char _convert_hex_to_string(uint8_t hex) {
    if (hex >= 10) {
        return hex - 10 + 'A';
    }
    return _convert_bcd_to_string(hex);
}

int main(void) {
    uint8_t sec_h, sec_l, min_h, min_l, hour_h, hour_l, day, date_h, date_l,
        month_h, month_l, year_h, year_l;

    char string[22];

    DDRB |= (1 << DDB1);
    PORTB = 0;

    init_serial();

    sei();

    safe_transmit_string_serial("TOP0!\r\n");

    init_i2c_master();

    _delay_ms(500);

    safe_transmit_string_serial("TOP1!\r\n");

    // ds1307_set_time_zero();

    safe_transmit_string_serial("TOP2!\r\n");

    while (1) {
        _delay_ms(100);
        // safe_transmit_string_serial("TOP!\r\n");

        ds1307_read_time(&sec_h, &sec_l, &min_h, &min_l, &hour_h, &hour_l, &day,
                         &date_h, &date_l, &month_h, &month_l, &year_h,
                         &year_l);

        string[0] = '2';
        string[1] = '0';
        string[2] = _convert_bcd_to_string(year_h);
        string[3] = _convert_bcd_to_string(year_l);
        string[4] = '-';
        string[5] = _convert_bcd_to_string(month_h);
        string[6] = _convert_bcd_to_string(month_l);
        string[7] = '-';
        string[8] = _convert_bcd_to_string(date_h);
        string[9] = _convert_bcd_to_string(date_l);
        string[10] = 'T';
        string[11] = _convert_bcd_to_string(hour_h);
        string[12] = _convert_bcd_to_string(hour_l);
        string[13] = ':';
        string[14] = _convert_bcd_to_string(min_h);
        string[15] = _convert_bcd_to_string(min_l);
        string[16] = ':';
        string[17] = _convert_bcd_to_string(sec_h);
        string[18] = _convert_bcd_to_string(sec_l);
        string[19] = '\r';
        string[20] = '\0';
        string[21] = '\0';

        safe_transmit_string_serial(string);
    }
}