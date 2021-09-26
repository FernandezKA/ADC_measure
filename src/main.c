#include "general.h"
//User variables
uint8_t u8CurrentChannel = 0;
static uint8_t u8ChannelPrescale;
static uint8_t u8Prescaler;
//User message
#ifndef DEBUG
static uint8_t u8aStartMessage[28U] = {'p', '-', 's', 'e','t',' ', 'p', 'r', 'e', 's', 'k', 'a', 'l', 'e', '\n', '\r', 
'm', '-', 's', 'e', 't', ' ', 'm', 'o', 'd', 'e', '\n', '\t'};
#endif
//Main function
void SysInit(void)
{
  vInitCLK();
  vInitUART();
  vInitTIM4();
  vInitADC();
  vInitGPIO();
  vUploadValueEEPROM(&u8Prescaler_1, &u8Prescaler_2, &u8Prescaler_3, &u8CurrentConfigurateADC);//Load old value from EEPROM
  asm("rim");
}

void main(void)
{
  SysInit();
#ifndef DEBUG
  vUART_ArrayTransmit(u8aStartMessage, 28);
#endif
  for (;;)
  {
    enum action eCurrentAction = eGetAction();//Select next action
    switch (eCurrentAction)
    {
    case select://Configure mode of work
      u8CurrentConfigurateADC = u8UART_Recieve() - 0x30; //Checked
      vUpdateEEPROMConfig(u8CurrentConfigurateADC);
      break;
      
    case prescaler://Set prescaler at format ch. number -> first digit of prescale -> second gigit of prescale
      u8ChannelPrescale = u8UART_Recieve() - 0x30U;//What of channel must be configured
      u8Prescaler = (u8UART_Recieve() - 0x30U) * 10;//Recieve first of value prescaler |first * 10 + second|
      u8Prescaler =  u8Prescaler + (u8UART_Recieve() - 0x30U);
      switch (u8ChannelPrescale)
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
      break;
    default:
      asm("nop"); //This command for debugging
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
