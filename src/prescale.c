#include "general.h"
//This function set prescaler for each of channel
void vSetPrescaler(uint8_t u8Channel, uint8_t u8Prescaler){
  switch (u8Channel)
      {
        //Channel 1
      case 1:
        vUpdateEEPROMChannel(1, u8Prescaler);//Update value from eeprom
        u8Prescaler_1 = u8Prescaler;
        break;
        //Channel 2
      case 2:
        vUpdateEEPROMChannel(2, u8Prescaler);
        u8Prescaler_2 = u8Prescaler;
        break;
        //Channel 3
      case 3:
        vUpdateEEPROMChannel(3, u8Prescaler);
        u8Prescaler_3 = u8Prescaler;
        break;
        //Case for mistake
      default:
        asm("nop");
         //vUpdateEEPROM(2, 1);
        break;
      }
}
//This function return mean value with fact of prescaler