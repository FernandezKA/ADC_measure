#include "general.h"
//User variables
uint8_t u8CurrentChannel = 0;
//Main function
//TODO: Set frequency of sampling, get value each IRQ for OVF Tim4
void SysInit(void){
    vInitCLK();
    vInitUART();
    vInitTIM4();
    vInitADC();
    vInitGPIO();
    asm("RIM");
}

void main(void)
{
        SysInit();
        for(;;){
        enum action eCurrentAction = eGetAction();
        switch(eCurrentAction){
        case select:
          u8CurrentConfigurateADC = u8UART_Recieve() - 0x31;
          break;
        case prescaler:
          asm("nop");
          break;
        default:
          asm("nop");//This command for debugging
          break;
          }
        }
        //ADC1->CR1|=ADC1_CR1_ADON;
}

#ifdef USE_FULL_ASSERT
void assert_failed(u8 *file, u32 line)
{
  asm("nop");
  return;
}
#endif
