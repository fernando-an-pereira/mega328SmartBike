#include "i2c_master.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <stdint.h>
#include <util/delay.h>

#include "data_structures.h"
#include "i2c.h"

volatile Buffer _i2c_tx_buffer;
volatile Buffer _i2c_rx_buffer;

volatile uint8_t _i2c_slave_address;

void _i2c_transmit_next_byte() {
    if (_i2c_tx_buffer.remaining > 0) {
        _i2c_tx_buffer.remaining--;
        TWDR = *(_i2c_tx_buffer.data++);
        TWCR = I2C_ENABLE;
    } else if (_i2c_rx_buffer.remaining > 0) {
        TWCR = I2C_START | I2C_ACK;
    } else {
        TWCR = I2C_STOP;
    }
}

void _i2c_receive_next_byte() {
    if (_i2c_rx_buffer.remaining > 0) {
        _i2c_rx_buffer.remaining--;
        *(_i2c_rx_buffer.data++) = TWDR;
        if (_i2c_rx_buffer.remaining >= 1) {
            TWCR = I2C_ACK;
        } else {
            TWCR = I2C_ENABLE;
        }
    }
}

ISR(TWI_vect) {
    uint8_t status = TWSR & 0xF8;
    switch (status) {
        case I2C_STATUS_START_T:
            TWDR = _i2c_slave_address << 1;
            TWCR = I2C_ENABLE;
            break;
        case I2C_STATUS_RE_START_T:
            TWDR = (_i2c_slave_address << 1) | 1;
            TWCR = I2C_ENABLE;
            break;
        case I2C_STATUS_SLA_W_T_ACK:
            _i2c_transmit_next_byte();
            break;
        case I2C_STATUS_SLA_W_T_NOT_ACK:
            TWCR = I2C_STOP;
            break;
        case I2C_STATUS_DATA_T_ACK:
            _i2c_transmit_next_byte();
            break;
        case I2C_STATUS_DATA_T_NOT_ACK:
            TWCR = I2C_STOP;
            break;
        case I2C_STATUS_ARB_LOST:
            TWCR = I2C_STOP;
            break;
        case I2C_STATUS_SLA_R_T_ACK:
            TWCR = I2C_ENABLE | (1 << TWEA);
            break;
        case I2C_STATUS_SLA_R_T_NOT_ACK:
            TWCR = I2C_STOP;
            break;
        case I2C_STATUS_DATA_R_ACK:
            _i2c_receive_next_byte();
            break;
        case I2C_STATUS_DATA_R_NOT_ACK:
            TWCR = I2C_STOP;
            break;
        default:
            TWCR = I2C_STOP;
            break;
    }
}

void init_i2c_master() {
    TWBR = I2C_BIT_RATE;
    TWSR = IC2_BIT_PRESCALER;
}

void i2c_send_bytes(uint8_t *tx_buffer, uint8_t *rx_buffer, uint8_t tx_size,
                    uint8_t rx_size, uint8_t slave_address) {
    _i2c_tx_buffer.data = tx_buffer;
    _i2c_rx_buffer.data = rx_buffer;
    _i2c_tx_buffer.remaining = tx_size;
    _i2c_rx_buffer.remaining = rx_size;
    _i2c_slave_address = slave_address;
    TWCR = I2C_START;
}

uint8_t is_ic2_transmitting() {
    return (_i2c_tx_buffer.remaining != 0 || _i2c_rx_buffer.remaining != 0);
}

void i2c_wait() {
    while (is_ic2_transmitting())
        ;
}