#include "serial.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>

volatile char* _serial_tx_buffer;

ISR(USART_UDRE_vect) {
    _serial_tx_buffer++;
    if (*_serial_tx_buffer == '\0') {
        UCSR0B &= (~(DATA_REGISTER_EMPTY_INTERRUPT_ENABLED) & ~(TX_ENABLED));
    } else {
        UDR0 = *_serial_tx_buffer;
    }
}

void init_serial() {
    UCSR0A = TRANSMISSION_SPEED_X_2 | MULTIPROCESSOR_COMMUNICATION_DISABLED;
    UCSR0B = CHARACTER_SIZE_8_REGISTER_B | CLOCK_POLARITY_ASYNCHRONOUS;
    UCSR0C = ASYNCHRONOUS_USART | STOP_BIT_1 | CHARACTER_SIZE_8_REGISTER_C;

    UBRR0H = SERIAL_PRESCALER >> 8;
    UBRR0L = SERIAL_PRESCALER & 0xFF;
}

void transmit_string_serial(char* string) {
    _serial_tx_buffer = string;
    UDR0 = *_serial_tx_buffer;
    UCSR0B |= (DATA_REGISTER_EMPTY_INTERRUPT_ENABLED | TX_ENABLED);
}

void safe_transmit_string_serial(char* string) {
    while (is_serial_transmitting())
        ;
    transmit_string_serial(string);
}
