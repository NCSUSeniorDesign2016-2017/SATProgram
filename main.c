/*----------------------------------------------------------------------------
 *----------------------------------------------------------------------------*/
#include <MKL25Z4.H>
#include <stdio.h>
#include "gpio_defs.h"
#include "UART.h"
#include "LEDs.h"
#include "timers.h"
#include "gps.h"

extern Q_T RxQ, TxQ;
volatile extern uint8_t message_received;
uint8_t input_message[Q_SIZE] ;
extern struct GPS_Message GPS_Message;

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main(void) {
    int i, index= ZERO;
    uint8_t copy_message = FALSE;
    Init_UART0(BAUD_RATE);
    RxQ.Head++;
    printf("Brandon Wiseman bmwisema\r\n");
    NVIC_ClearPendingIRQ(UART0_IRQn);
    NVIC_EnableIRQ(UART0_IRQn);
 while (TRUE) {
        if(message_received){       //if message received
         for(i=ZERO; i<Get_Num_Rx_Chars_Available(); i++){
             if(RxQ.Data[i] == ASCII_$){
                 copy_message = TRUE;
             }
             if(copy_message){
                input_message[index] = RxQ.Data[i];     //copy que to array
                index++;
             }
             RxQ.Data[i] = ZERO;            //empty que
         }
         Q_Init(&RxQ);
         message_received = FALSE;
         copy_message = FALSE;
         index=ZERO;
         Task_NMEA_Decode();
     }
        Task_Report_Drift();
 }
 return ZERO;
}
// *******************************ARM University Program Copyright © ARM Ltd 2013*************************************
