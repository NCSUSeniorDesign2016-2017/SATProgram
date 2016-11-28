#include <msp432p401r.h>
#include "adc.h"



void Init_ADC(void) {
  // adc stores results in ADC14->MEM[N];
  
  // when analog signals are applied to digital pins, there is parasitic current 
  // disable the digital part of the pin to fix this
  // controlled by PySELx bits
  
  // there is a REF module that supplies reference voltages to peripherals
  // 1.2V, 1.45V, 2.5V available
  
  
  ADC14->CTL0 &= ~ADC14_CTL0_ENC; // disable ADC for setup
  ADC14->CTL0 |= ADC14_CTL0_SSEL_4; // ADC14 clock source = SMCLK
  ADC14->CTL1 |= ADC14_CTL1_RES__14BIT; // set to 14 bit resolution
  ADC14->MCTL[0] |= ADC14_MCTLN_VRSEL_14; // V(R-) and V(R+) source selection
  ADC14->MCTL[0] |= ADC14_MCTLN_INCH_0; // input channel select
  ADC14->IER0 |= ADC14_IER0_IE0; // adc interrupt enable 
  ADC14->CTL0 |= ADC14_CTL0_ENC; // enable ADC
  ADC14->CTL0 |= ADC14_CTL0_ON; // turn on adc, removes reset, ready for conversion 
  
  
}
