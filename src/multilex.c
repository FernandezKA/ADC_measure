#include "general.h"
//This function make edit next sampling channel with defined rules
void vGetMultiplex(uint8_t* u8LastChannel, uint8_t u8MultiplexRule){
  switch (u8MultiplexRule)
    {
    case 0x01:
      *u8LastChannel = 0x02;
      vSelectChannel(*u8LastChannel);
      break;
    case 0x02:
      *u8LastChannel = 0x03;
      vSelectChannel(*u8LastChannel);
      break;
    case 0x03:
      *u8LastChannel = 0x04;
      vSelectChannel(*u8LastChannel);
      break;
    case 0x04:
      if (*u8LastChannel == 2)
      {
      *u8LastChannel = 0x03;
      vSelectChannel(*u8LastChannel);
      }
      else
      {
      *u8LastChannel = 0x02;
      vSelectChannel(*u8LastChannel);
      }
      break;
    case 0x05:
      if (*u8LastChannel == 3)
      {
      *u8LastChannel = 0x04;
      vSelectChannel(*u8LastChannel);
      }
      else
      {
      *u8LastChannel = 0x03;
      vSelectChannel(*u8LastChannel);
      }
      break;
    case 0x06:
      if (*u8LastChannel == 0x02)
      {
      *u8LastChannel = 0x04;
      vSelectChannel(*u8LastChannel);
      }
      else
      {
      *u8LastChannel = 0x02;
      vSelectChannel(*u8LastChannel);
      }
      break;
    case 0x07:
      switch(*u8LastChannel){
      case 0x02:
        *u8LastChannel = 0x03;
        vSelectChannel(*u8LastChannel);
        break;
      case 0x03:
      *u8LastChannel = 0x04;
      vSelectChannel(*u8LastChannel);
        break;
      case 0x04: 
      *u8LastChannel = 0x02;
      vSelectChannel(*u8LastChannel);
        break;
      }
      break;
    }
}
//This function configurate rules of multiplexing
void vSetRulesMul(uint8_t* pu8RulseVariable){
  *pu8RulseVariable = u8GetDigit();
}