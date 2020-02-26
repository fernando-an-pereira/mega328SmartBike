#ifndef _SERIAL_H
#define _SERIAL_H

#ifndef SERIAL_BAUD
#define SERIAL_BAUD 9600
#endif

#define SERIAL_PRESCALER ((uint16_t)(((float)F_CPU) / SERIAL_BAUD / 8 - 0.5f))

#include <avr/io.h>
#include <stdint.h>

#define TRANSMISSION_SPEED_X_1 (0 << U2X0)
#define TRANSMISSION_SPEED_X_2 (1 << U2X0)
#define MULTIPROCESSOR_COMMUNICATION_DISABLED (0 << MPCM0)

#define RX_COMPLETE_INTERRUPT_ENABLED (1 << RXCIE0)
#define TX_COMPLETE_INTERRUPT_ENABLED (1 << TXCIE0)
#define DATA_REGISTER_EMPTY_INTERRUPT_ENABLED (1 << UDRIE0)
#define RX_ENABLED (1 << RXEN0)
#define TX_ENABLED (1 << TXEN0)
#define CHARACTER_SIZE_8_REGISTER_B (0 << UCSZ02)

#define ASYNCHRONOUS_USART ((0 << UMSEL01) | (0 << UMSEL00))
#define PARITY_EVEN ((1 << UPM01) | (0 << UPM00))
#define STOP_BIT_1 (0 << USBS0)
#define CHARACTER_SIZE_8_REGISTER_C ((1 << UCSZ01) | (1 << UCSZ00))
#define CLOCK_POLARITY_ASYNCHRONOUS (0 << UCPOL0)

#define is_serial_transmitting() (UCSR0B & TX_ENABLED)

void init_serial();
void transmit_string_serial(char* string);
void safe_transmit_string_serial(char* string);

#undef _SERIAL_H
#endif