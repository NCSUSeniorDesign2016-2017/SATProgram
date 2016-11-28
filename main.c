#include <msp432p401r.h>
#include "clocks.h"
#include "uart.h"
#include "driverlib.h"

#define COMPASS_ADDRESS  (0x3C)		//7-bit slave address

const eUSCI_I2C_MasterConfig i2cConfig =
{
        EUSCI_B_I2C_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        3000000,                                // SMCLK = 3MHz
        EUSCI_B_I2C_SET_DATA_RATE_400KBPS,      // Desired I2C Clock of 400khz
        0,                                      // No byte counter threshold
        EUSCI_B_I2C_NO_AUTO_STOP                // No Autostop
};

struct compass_data {
	int x;
	int y;
	int z;
} compass_data;

double direction =0;

int main(void) {
	
  Init_Clocks();
  Init_UART0();
	
	/* Disabling the Watchdog */
    MAP_WDT_A_holdTimer();

    /* Select Port 6 for I2C - Set Pin 4, 5 to input Primary Module Function,
     *   (UCB0SIMO/UCB0SDA, UCB0SOMI/UCB0SCL).
     */
		
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN4 + GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Initializing I2C Master to SMCLK at 400kbs with no autostop */
    MAP_I2C_initMaster(EUSCI_B0_BASE, &i2cConfig);

    /* Specify slave address */
    MAP_I2C_setSlaveAddress(EUSCI_B0_BASE, COMPASS_ADDRESS);

    /* Set Master in receive mode */
    MAP_I2C_setMode(EUSCI_B0_BASE, EUSCI_B_I2C_TRANSMIT_MODE);

    /* Enable I2C Module to start operations */
    MAP_I2C_enableModule(EUSCI_B0_BASE);
    
    /* Enable and clear the interrupt flag */
    MAP_I2C_clearInterruptFlag(EUSCI_B0_BASE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0 + EUSCI_B_I2C_NAK_INTERRUPT);
    //Enable master Receive interrupt
    MAP_I2C_enableInterrupt(EUSCI_B0_BASE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0 + EUSCI_B_I2C_NAK_INTERRUPT);
    MAP_Interrupt_enableInterrupt(INT_EUSCIB0);
  
  while(1) {
    // 1. send 0x3C 0x00 0x71 - 15Hz default, positive self test measurement
		// 2. send 0x3C 0x01 0xA0 - gain = 5
		// 3. send 0x3C 0x02 0x00 - continuous-measurement mode
		// 4. wait 6ms
		// 5. Send 0x3D 0x06, convert hex values to x,y,&z
		// 6. check limts according to gain (see datasheet)
		// 7. find which LED corresponds to heading
		// 8. turn off old LED, turn on new LED
  }
}
