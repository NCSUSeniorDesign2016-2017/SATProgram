#include <msp432p401r.h>
#include "clocks.h"
#include "uart.h"



int main(void) {
  Init_Clocks();
  Init_UART0();
  
  while(1) {
    
  }
}
