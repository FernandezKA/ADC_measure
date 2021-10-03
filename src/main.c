#include "general.h"
#include "messages.h"
//User variables
uint8_t u8CurrentChannel = 0;
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
  vUART_ArrayTransmit("Enter commands: \n\r", 18);
#endif
  for (;;)
  {
    switch(MAIN){
      case subprescaler: 
        asm("sim");
        vUART_ArrayTransmit("Enter channel, subchannel nums\n\r", 32);
        uint8_t u8Channel, u8Subchannel;
        u8Channel = u8GetDigit();
        u8Subchannel = u8GetDigit();
        vSelechSub(u8Channel,u8Subchannel);
        MAIN = wait;
        asm("rim");
      break;
      //Multiplexing channel
      case select_mode:
        asm("sim");
        vUART_ArrayTransmit("Please, enter number of mode \n\r", 31);
        vSetRulesMul(&u8CurrentConfigurateADC);
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
    
    case help:
      asm("sim");
      vPrintHelp();
      vUART_ArrayTransmit("Press any key\n\r", 15);
      u8UART_Recieve();
      MAIN = wait;
      asm("rim");
      break;
    case calibrate:
      asm("sim");
      vUART_ArrayTransmit("Enter number of channel: \n\r", 27);
      uint8_t u8CalibratedChannel = u8GetDigit();
      vGetCalibrate(u8CalibratedChannel);
      vExportData();
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
