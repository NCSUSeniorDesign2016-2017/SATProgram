#include <msp432p401r.h>
#include "uart.h"



void Init_UART0(void) {
  // From the family reference manual:
  // The recommended eUSCI_A initialization/reconfiguration process is:
  // 1. Set UCSWRST.
  // 2. Initialize all eUSCI_A registers with UCSWRST = 1 (including UCAxCTL1).
  // 3. Configure ports.
  // 4. Clear UCSWRST with software.
  // 5. Enable interrupts (optional) with UCRXIE or UCTXIE.
  
  
  // 1. Set UCSWRST.
  EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;
  
  
  // 2. Initialize all eUSCI_A registers with UCSWRST = 1 (including UCAxCTL1).
  // Baud rate calculations:
  // N = fbrclk/baud rate = 3000000/9600 = 312.5
  // OS16 = 1
  // UCBR0 = INT(N/16) = INT(312.5/16) = 19
  // UCBRF0 = INT([(N/16)-INT(N/16)]*16) = INT([19.53125-19]*16) = 8
  // UCBRS0 = 0xAA
  EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_UCSSEL_2; // Set clock source to SMCLK
  EUSCI_A0->BRW = 19; // UCBR0 Baud rate control word
  EUSCI_A0->MCTLW |= 0xAA << EUSCI_A_MCTLW_BRS_OFS; // UCBRS0
  EUSCI_A0->MCTLW |= 8 << EUSCI_A_MCTLW_BRF_OFS; // UCBRF0
  EUSCI_A0->MCTLW |= EUSCI_A_MCTLW_OS16; // UCOS16
  
  
  // 3. Configure ports.
  // RXD P1.2 
  P1->SEL0 |= UCA0RXD;
  P1->SEL1 &= ~UCA0RXD;
  // TXD P1.3
  P1->SEL0 |= UAC0TXD;
  P1->SEL1 &= ~UAC0TXD;
  
  
  // 4. Clear UCSWRST with software.
  EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;
  
  
  // 5. Enable interrupts (optional) with UCRXIE or UCTXIE.
  //EUSCI_A0->IE |= EUSCI_A_IE_RXIE; // receive interrupt enable
  //NVIC_SetPriority(EUSCIA0_IRQn, 2);
  //NVIC_ClearPendingIRQ(EUSCIA0_IRQn);
  //NVIC_EnableIRQ(EUSCIA0_IRQn);
}



void EUSCIA0_IRQHandler(void) {
  NVIC_ClearPendingIRQ(EUSCIA0_IRQn);
}
