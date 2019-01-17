#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <stdint.h>
#include <util/delay.h>

volatile uint8_t max;
volatile uint8_t counter;

ISR(ADC_vect) {
    uint16_t value = ADCH;
    value *= 125;
    value /= 255;
    max = value;
}

ISR(TIMER0_COMPA_vect) {
    if ((PINB & (1 << PINB0)) != 0) {
        PORTB &= ~(1 << PORTB1);
        max = 0;
    } else {
        if (++counter > max) {
            PORTB ^= 1 << PORTB1;
            counter = 0;
        }
    }
}

int main(void) {
    DDRB |= (1 << DDB1);
    DDRB &= ~(1 << DDB0);
    PORTB |= (1 << PORTB0);

    DDRC &= ~(1 << DDC3);
    PORTC &= ~(1 << PORTC3);

    ADMUX = (1 << REFS0) | (1 << ADLAR) | (1 << MUX1) | (1 << MUX0);
    ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADATE) | (1 << ADIE) |
             (1 << ADPS1) | (1 << ADPS0);
    ADCSRB = (1 << ADTS1) | (1 << ADTS0);
    DIDR0 |= (1 << ADC3D);

    TIMSK0 = (1 << OCIE0A);
    TCCR0A = (1 << WGM01);
    TCCR0B = (1 << CS02);

    OCR0A = 125;

    sei();

    while (1) {
    }
}