#include "gps.h"
#include "queue.h"
#include "UART.h"
#include "stdio.h"
#include "stdlib.h"

extern Q_T RxQ, TxQ;
extern uint8_t input_message[Q_SIZE];
int message_type = ZERO;
int given_sum = ZERO;
uint8_t direction = ZERO;

struct GPS_Message GPS_Message;

void Task_NMEA_Decode(void){
        Decode_HeaderGPS(1);
        switch(message_type){       //evaluate what type of message was received
            case FALSE: {
                printf("Checksum doesn't match!\r");
            break;
            }
            case VALID_GPS_MESSAGE: {
                Decode_GPS();
                GPS_Message.new = TRUE;
                break;
            }
            default: break;
        }
        message_type = ZERO;
}

void Task_Report_Drift(void){
    if(GPS_Message.new){
        printf("RMC received. Speed over ground is %fkts with a track of %f. Magnetic variation is %f\r",GPS_Message.Speed_over_ground, GPS_Message.Track_angle_true, GPS_Message.Mag_variation);
    }
    GPS_Message.new = FALSE;
}

void Decode_HeaderGPS(int index){
    int sum;
    if(input_message[index] == ASCII_G){        //verify header
            index++;
            if(input_message[index] == ASCII_P){
                index++;
                if(input_message[index] == ASCII_R){
                    index++;
                    if(input_message[index] == ASCII_M){
                        index++;
                        if(input_message[index] == ASCII_C){
                            index++;
                            message_type = VALID_GPS_MESSAGE;
                        }else return;
                    }else return;
                }else return;
            }else return;
        }
    sum = checksum();       //verify checksum
        if(sum != given_sum){
            message_type = FALSE;
        }
    }


void Decode_GPS(void){
    int i = INDEX_DATA_BEGIN;
    int comma_count, ready_to_compute = ZERO;
    while(comma_count<GPS_COMMA_MAX){
        if(input_message[i] == ASCII_Comma){
            comma_count++;                                                          //increment to next data field
            ready_to_compute = TRUE;
        }
        i++;
        if(ready_to_compute){
            switch(comma_count){
                case CASE_GPS_SPEED:{       // speed over ground
                    GPS_Message.Speed_over_ground = Convert_Number_to_Float(i, TYPE_GPS);
                    break;
                }
                case 3: {
                    //Latitude
                    break;
                }
                case 4: {
                    //Latitude Hemisphere
                    break;
                }
                case 5: {
                    //Longitude
                    break;
                }
                case 6:
                {
                    //Longitude Hemisphere
                    break;
                }
                case CASE_GPS_HEADING: {        // true directional heading
                    GPS_Message.Track_angle_true = Convert_Number_to_Float(i, TYPE_GPS);
                    break;
                }
                case CASE_GPS_MagVar: { //magnetic variation
                    GPS_Message.Mag_variation = Convert_Number_to_Float(i, TYPE_GPS);
                    break;
                }
                case CASE_GPS_Dir: {
                    direction = input_message[i];           // east or west for magnetic variation calc
                    if(direction == ASCII_E){   //positive magnetic variation
                    }else if(direction == ASCII_W){
                        GPS_Message.Mag_variation = -1 * GPS_Message.Mag_variation;
                    } else { printf("Error in direction"); return;}     // not E or W
                    break;
                }
                default: break;
            }
            ready_to_compute = FALSE;
        }
    }
}


float Convert_Number_to_Float(int index, int type){
    float new_number = ZERO;
    int hundreds, tens, ones = ZERO;
    float tenths = ZERO;
    if(input_message[index] != ASCII_Comma){        //guard against empty field
        if(type == TYPE_GPS){
        hundreds = HUNDREDS*(input_message[index] - ASCII_to_Decimal);      //convert to hundreds decimal
        index++;
        tens = TENS*(input_message[index] - ASCII_to_Decimal);
        index++;
        ones = input_message[index]-ASCII_to_Decimal;
        index+=ADVANCE_PAST_DECIMAL;        //move past decimal
        tenths = (float) (input_message[index] - ASCII_to_Decimal)/ TENS;
        new_number = hundreds + tens + ones + tenths;
        } else{
                ones = input_message[index]-ASCII_to_Decimal;
                index+=ADVANCE_PAST_DECIMAL;        //move past decimal
                tenths = (float) (input_message[index] - ASCII_to_Decimal)/ TENS;
                new_number = ones + tenths;
        }
    }
    return new_number;
}

int checksum(void) {
  int index = ZERO, sum = ZERO, i;
    char verify[CHECKSUM_SIZE];
    while(input_message[index] != ASCII_Astrisk){   //find *
        index++;
    }
    index++;        // move to first checksum character
    verify[ZERO] = input_message[index];
    index++;
    verify[CHECKSUM_BEGIN] = input_message[index];
    given_sum = strtol(verify, NULL, HEX_BASE); //convert hex to int
    for(i=CHECKSUM_BEGIN; i < index-CHECKSUM_SIZE; i++){
        sum^= (char) input_message[i];
    }
    return sum;
}
