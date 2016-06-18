#include "Atmega88pUtils.h"

#include <avr/io.h>

namespace Atmega88p {

void initializeTimer0(
    TimerPrescalerValue prescalerValue,
    WaveformGenerationMode mode,
    bool topAtOcra
) {
    switch (prescalerValue) {
    case PSV_1:
        TCCR0B |= BV(CS00);
        break;
    case PSV_8:
        TCCR0B |= BV(CS01);
        break;
    case PSV_64:
        TCCR0B |= BV(CS01) | BV(CS00);
        break;
    case PSV_256:
        TCCR0B |= BV(CS02);
        break;
    case PSV_1024:
        TCCR0B |= BV(CS02) | BV(CS00);
        break;
    }

    switch (mode) {
    case NORMAL:
        // The default, nothing to do
        break;
    case PWM_PHASE_CORRECT:
        TCCR0A |= BV(WGM00);
        if (topAtOcra) {
            TCCR0B |= BV(WGM02);
        }
        break;
    case PWM_FAST:
        TCCR0A |= BV(WGM01) | BV(WGM00);
        if (topAtOcra) {
            TCCR0B |= BV(WGM02);
        }
        break;
    case CTC:
        TCCR0A |= BV(WGM01);
        break;
    }
}

void initializeTimer2(
    TimerPrescalerValue prescalerValue,
    WaveformGenerationMode mode,
    bool topAtOcra
) {
    switch (prescalerValue) {
        case PSV_1:
        TCCR2B |= BV(CS20);
        break;
    case PSV_8:
        TCCR2B |= BV(CS21);
        break;
    case PSV_32:
        TCCR2B |= BV(CS21) | BV(CS20);
        break;
    case PSV_64:
        TCCR2B |= BV(CS22);
        break;
    case PSV_128:
        TCCR2B |= BV(CS22) | BV(CS20);
        break;
    case PSV_256:
        TCCR2B |= BV(CS22) | BV(CS21);
        break;
    case PSV_1024:
        TCCR2B |= BV(CS22) | BV(CS21) | BV(CS20);
        break;
    }

    switch (mode) {
    case NORMAL:
        // The default, nothing to do
        break;
    case PWM_PHASE_CORRECT:
        TCCR2A |= BV(WGM20);
        if (topAtOcra) {
            TCCR2B |= BV(WGM22);
        }
        break;
    case PWM_FAST:
        TCCR2A |= BV(WGM21) | BV(WGM20);
        if (topAtOcra) {
            TCCR2B |= BV(WGM22);
        }
        break;
    case CTC:
        TCCR2A |= BV(WGM21);
        break;
    }
}

void setVoltageReference(VoltageReference reference) {
    switch(reference) {
    case VREF_AREF:
        // The default, nothing to do
        break;
    case VREF_VCC:
        ADMUX |= BV(REFS0);
        break;
    case VREF_INTERNAL_1_1V:
        ADMUX |= BV(REFS1) | BV(REFS0);
        break;
    }
}

void leftAdjustAdcResults(bool leftAdjust) {
    if(leftAdjust) {
        ADMUX |= BV(ADLAR);
    } else {
        ADMUX &= ~BV(ADLAR);
  }
}

void setAdcPrescalerValue(AdcPrescalerValue value) {
    switch(value) {
    case ADC_PSV_2:
        // The default, nothing to do
        break;
    case ADC_PSV_4:
        ADCSRA |= BV(ADPS1);
        break;
    case ADC_PSV_8:
        ADCSRA |= BV(ADPS1) | BV(ADPS0);
        break;
    case ADC_PSV_16:
        ADCSRA |= BV(ADPS2);
        break;
    case ADC_PSV_32:
        ADCSRA |= BV(ADPS2) | BV(ADPS0);
        break;
    case ADC_PSV_64:
        ADCSRA |= BV(ADPS2) | BV(ADPS1);
        break;
    case ADC_PSV_128:
        ADCSRA |= BV(ADPS2) | BV(ADPS1) | BV(ADPS0);
        break;
    }
}

} // namespace
