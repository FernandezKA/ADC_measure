#include "general.h"
#include "messages.h"
//User variables
uint8_t u8CurrentChannel = 0;
static uint8_t u8ChannelPrescale;
static uint8_t u8Prescaler;
enum FSM MAIN = wait;
//Main function
void SysInit(void)
{
  vInitCLK();
  vInitUART();
  vInitTIM4();
  vInitADC();
  vInitGPIO();
  //vUploadValueEEPROM(&u8Prescaler_1, &u8Prescaler_2, &u8Prescaler_3, &u8CurrentConfigurateADC);//Load old value from EEPROM
  asm("rim");
}

void main(void)
{
  SysInit();
#ifndef DEBUG
  vUART_ArrayTransmit(u8aStartMessage, 29);
#endif
  for (;;)
  {
    switch(MAIN){
      //Multiplexing channel
      case select_mode:
        asm("sim");
        vUART_ArrayTransmit("Please, enter number of mode \n\r", 31);
        vSetRulesMul(&u8CurrentConfigurateADC);
      MAIN = wait;
      asm("rim");
      break;
     
    case prescaler_mode:
      asm("sim");
      vUART_ArrayTransmit("Please, enter prescaler at form \n\r number of channel, 1 digit, 2 digit\n\r", 73);
      u8ChannelPrescale = u8UART_Recieve() - 0x30U;//What of channel must be configured
      //u8Prescaler = (u8UART_Recieve() - 0x30U) * 10;//Recieve first of value prescaler |first * 10 + second|
      u8Prescaler = (u8UART_Recieve() - 0x30U);
      vSetPrescaler(u8ChannelPrescale, u8Prescaler);
      MAIN = wait;
      asm("rim");
      break;
      
    case wait:
      //MAIN = wait;
      break;
    case save:
      asm("nop");
      MAIN = wait;
      break;
      
    case calibrate:
      asm("sim");
      vUART_ArrayTransmit("Enter number of channel: \n\r", 27);
      uint8_t u8CalibratedChannel = u8UART_Recieve() - 0x30;
      vGetCalibrate(u8CalibratedChannel);
      MAIN = wait;
      asm("rim");
      break;
    }
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(u8 *file, u32 line)
{
  asm("nop");
  return;
}
#endif
