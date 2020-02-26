#ifndef _I2C_MASTER_H
#define _I2C_MASTER_H

#include <avr/io.h>
#include <stdint.h>

#include "i2c.h"

void init_i2c_master();
void i2c_send_bytes(uint8_t* tx_buffer, uint8_t* rx_buffer, uint8_t tx_size,
                    uint8_t rx_size, uint8_t slave_address);
uint8_t is_ic2_transmitting();

void i2c_wait();

#undef _I2C_MASTER_H
#endif