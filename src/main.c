#include "general.h"
#include "messages.h"
//User variables
uint8_t u8CurrentChannel = 0;
enum FSM MAIN = wait;
bool shortOut = TRUE;
//Main function
void SysInit(void)
{
  vInitCLK();
  vInitUART();
  vInitTIM4();
  vInitADC();
  vInitGPIO();
  vExportData();
  vPrintHelp();
  asm("rim");
}

void main(void)
{
  SysInit();
  getRestore(&u8CurrentConfigurateADC, &OutModeVar, &CH1.u8SubChannel, &CH2.u8SubChannel, &CH3.u8SubChannel);
#ifndef DEBUG
  vUART_ArrayTransmit("Enter commands: \n\r", 18);
#endif
  for (;;)
  {
    switch(MAIN){
      case subprescaler: 
        asm("sim");
        vUART_ArrayTransmit("Enter channel number\n\r", 22);
        uint8_t u8Channel, u8Subchannel;
        u8Channel = u8GetDigit();
        vUART_ArrayTransmit("Enter subchannel number\n\r", 25);
        u8Subchannel = u8GetDigit();
        vSelechSub(u8Channel,u8Subchannel);
        getBackup(&u8CurrentConfigurateADC, &OutModeVar, &CH1.u8SubChannel, &CH2.u8SubChannel, &CH3.u8SubChannel);
        MAIN = wait;
        asm("rim");
      break;
      //Multiplexing channel
      case select_mode:
        asm("sim");
        vUART_ArrayTransmit("Enter the number of modes \n\r", 28);
        vUART_ArrayTransmit("-> 1 - CH.1\n\r", 13);
        vUART_ArrayTransmit("-> 2 - CH.2\n\r", 13);
        vUART_ArrayTransmit("-> 3 - CH.3\n\r", 13);
        vUART_ArrayTransmit("-> 4 - CH. 1 + CH. 2 \n\r", 23);
        vUART_ArrayTransmit("-> 5 - CH. 2 + CH. 3 \n\r", 23);
        vUART_ArrayTransmit("-> 6 - CH. 1 + CH. 3 \n\r", 23);
        vUART_ArrayTransmit("-> 7 - CH. 1 + CH. 2 + CH. 3 \n\r", 31);
        vSetRulesMul(&u8CurrentConfigurateADC);
        MAIN = wait;
        getBackup(&u8CurrentConfigurateADC, &OutModeVar, &CH1.u8SubChannel, &CH2.u8SubChannel, &CH3.u8SubChannel);
        asm("rim");
      break;
     
    case wait:
      //MAIN = wait;
    break;
      
    case help:
      asm("sim");
      vPrintHelp();
      vUART_ArrayTransmit("Press any key\n\r", 15);
      u8UART_Recieve();
      MAIN = wait;
      asm("rim");
      break;
    case out:
      asm("sim");
      vUART_ArrayTransmit("Select type output message\n\r", 28);
      vUART_ArrayTransmit("0 - short out, 1 - normal, 2 - all channels\n\r", 45);
      static uint8_t u8SelOut;
      u8SelOut = u8GetDigit();
      if(u8SelOut == 0){
        OutModeVar = short_out;
      }
      else if (u8SelOut == 1){
        OutModeVar = long_out;
      }
      else if(u8SelOut == 2){
        OutModeVar = full_out;
      }
      else{
        vUART_ArrayTransmit("Mistake\n\r",9); 
      }
      MAIN = wait;
      getBackup(&u8CurrentConfigurateADC, &OutModeVar, &CH1.u8SubChannel, &CH2.u8SubChannel, &CH3.u8SubChannel);
      asm("rim");
      break;
      
    case prescalers:
      asm("sim");
      static unsigned char InfoMsg[] = {"0-0-0\n\r"};
      InfoMsg[0] = CH1.u8SubChannel + 48;
      InfoMsg[2] = CH2.u8SubChannel+48;
      InfoMsg[4] = CH3.u8SubChannel+48;
      vUART_ArrayTransmit(InfoMsg, 7);
      vUART_ArrayTransmit("Press any key\n\r", 15);
      u8UART_Recieve();
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
