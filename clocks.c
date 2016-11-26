#include <msp432p401r.h>
#include "clocks.h"



void Init_Clocks(void) {
  // Configure SMCLK signal from DCO clock resource
  // DCO defaults to 3MHz, internal resistor mode
  // DCO will enable when used as a source for SMCLK
  
  
  CS->KEY = CS_KEY_VAL; // unlock clock system
  CS->CTL1 |= CS_CTL1_SELS__DCOCLK; // sets SMCLK and HSMCLK to DCO
  CS->KEY = 0x1234; // lock clock system 
}
