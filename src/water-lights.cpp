// Example code for dc-motor-controller. Adapt this to exact use.
//
// NOTE: Pin descriptions and so on are for 2016-05-30 revision of the printed
// circuit, with manual fixes. Thus, not compatible with the latest revision.
//
// Author: Otto Urpelainen
// Email: oturpe@iki.fi
// Date: 2016-06-18

#include "Atmega8Utils.h"

#include "config.h"

#include <avr/io.h>
#include <util/delay.h>

/// \brief
///    Toggles the indicator led state.
void toggleIndicator() {
    static bool lit;

    if (lit) {
        INDICATOR_DATA &= ~BV(INDICATOR_DATA_PIN);
    }
    else {
        INDICATOR_DATA |= BV(INDICATOR_DATA_PIN);
    }

    lit = !lit;
}

/// \brief
///    Sets the motors to the state they should have on startup.
void initializeMotors() {
    // Set running forwards
    MOTOR_1_FORWARD_OUTPUT_COMPARE = MOTOR_1_SPEED;
    MOTOR_1_REVERSE_OUTPUT_COMPARE = 0x00;

    MOTOR_2_FORWARD_OUTPUT_COMPARE = MOTOR_2_SPEED;
    MOTOR_2_REVERSE_OUTPUT_COMPARE = 0x00;

    MOTOR_3_FORWARD_OUTPUT_COMPARE = MOTOR_3_SPEED;
    MOTOR_3_REVERSE_OUTPUT_COMPARE = 0x00;

    // Enable all outputs
    MOTOR_1_FORWARD_DATA |= BV(MOTOR_1_FORWARD_DATA_PIN);
    MOTOR_2_FORWARD_DATA |= BV(MOTOR_2_FORWARD_DATA_PIN);
    MOTOR_3_FORWARD_DATA |= BV(MOTOR_3_FORWARD_DATA_PIN);
    MOTOR_1_REVERSE_DATA |= BV(MOTOR_1_REVERSE_DATA_PIN);
    MOTOR_2_REVERSE_DATA |= BV(MOTOR_2_REVERSE_DATA_PIN);
    MOTOR_3_REVERSE_DATA |= BV(MOTOR_3_REVERSE_DATA_PIN);
}

/// \brief
///    Calculates the correct acceleration to use when current and target speed
///    are as given.
int16_t acceleration(uint8_t current, uint8_t target, uint8_t maxAcceleration) {
    int16_t acceleration;

    acceleration = target - current;

    if (acceleration > maxAcceleration) {
        acceleration = maxAcceleration;
    }

    return acceleration;
}

int main() {
    INDICATOR_DATA_DIR |= BV(INDICATOR_DATA_DIR_PIN);

    MOTOR_1_FORWARD_DATA_DIR |= BV(MOTOR_1_FORWARD_DATA_DIR_PIN);
    MOTOR_1_REVERSE_DATA_DIR |= BV(MOTOR_1_REVERSE_DATA_DIR_PIN);
    MOTOR_2_FORWARD_DATA_DIR |= BV(MOTOR_2_FORWARD_DATA_DIR_PIN);
    MOTOR_2_REVERSE_DATA_DIR |= BV(MOTOR_2_REVERSE_DATA_DIR_PIN);
    MOTOR_3_FORWARD_DATA_DIR |= BV(MOTOR_3_FORWARD_DATA_DIR_PIN);
    MOTOR_3_REVERSE_DATA_DIR |= BV(MOTOR_3_REVERSE_DATA_DIR_PIN);

    Atmega8::initializeTimer0(
        PRESCALER_VALUE,
        Atmega8::PWM_PHASE_CORRECT,
        Atmega8::TOP_00FF
    );
    Atmega8::initializeTimer1(
        PRESCALER_VALUE,
        Atmega8::PWM_PHASE_CORRECT,
        Atmega8::TOP_00FF
    );
    Atmega8::initializeTimer2(
        PRESCALER_VALUE,
        Atmega8::PWM_PHASE_CORRECT,
        Atmega8::TOP_00FF
    );

    OCR1AH = 0x00;
    OCR1BH = 0x00;

    // Set non-inverting pwm
    TCCR0A |= BV(COM0A1) | BV(COM0B1);
    TCCR1A |= BV(COM1A1) | BV(COM1B1);
    TCCR2A |= BV(COM2A1) | BV(COM2B1);

    initializeMotors();

    uint16_t indicatorCounter = 0;

    while (true) {
        if (indicatorCounter == INDICATOR_HALF_PERIOD) {
            toggleIndicator();
            indicatorCounter = 0;
        }
        else {
            indicatorCounter++;
        }

        _delay_ms(LOOP_DELAY);
    }
}
