#ifndef I2C_H_
#define I2C_H_

#include <avr/io.h>
#include <stdint.h>

#define I2C_MODE_MASTER 0
#define I2C_MODE_SLAVE 1

// Transmitter mode
#define I2C_STATUS_START_T 0x08
#define I2C_STATUS_RE_START_T 0x10
#define I2C_STATUS_SLA_W_T_ACK 0x18
#define I2C_STATUS_SLA_W_T_NOT_ACK 0x20
#define I2C_STATUS_DATA_T_ACK 0x28
#define I2C_STATUS_DATA_T_NOT_ACK 0x30
#define I2C_STATUS_ARB_LOST 0x38

// Receiver mode
#define I2C_STATUS_SLA_R_T_ACK 0x40
#define I2C_STATUS_SLA_R_T_NOT_ACK 0x48
#define I2C_STATUS_DATA_R_ACK 0x50
#define I2C_STATUS_DATA_R_NOT_ACK 0x58

#define I2C_STATUS (TWSR & 0xF8)

#define I2C_ENABLE_NO_INTERRUPT ((1 << TWINT) | (1 << TWEN))
#define I2C_ENABLE (I2C_ENABLE_NO_INTERRUPT | (1 << TWIE))
#define I2C_STOP (I2C_ENABLE_NO_INTERRUPT | (1 << TWSTO))
#define I2C_START (I2C_ENABLE | (1 << TWSTA))
#define I2C_ACK (I2C_ENABLE | (1 << TWEA))

#ifndef I2C_SCL_FREQ
#define I2C_SCL_FREQ 10000
#endif

#ifndef I2C_PRESCALER
#define I2C_PRESCALER 16
#endif

#define I2C_BIT_RATE                                                       \
    ((uint8_t)((((float)(F_CPU)) / I2C_SCL_FREQ / 2 - 8) / I2C_PRESCALER + \
               0.5f))

#if I2C_PRESCALER == 64
#define IC2_BIT_PRESCALER ((1 << TWPS1) | (1 << TWPS0))
#elif I2C_PRESCALER == 16
#define IC2_BIT_PRESCALER ((1 << TWPS1) | (0 << TWPS0))
#elif I2C_PRESCALER == 4
#define IC2_BIT_PRESCALER ((0 << TWPS1) | (1 << TWPS0))
#else
#define IC2_BIT_PRESCALER ((0 << TWPS1) | (0 << TWPS0))
#endif

#endif