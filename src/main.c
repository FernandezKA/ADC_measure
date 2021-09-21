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
        ADC1->CR1|=ADC1_CR1_ADON;
	while (1){
          for(uint16_t i = 0; i < 0xFFFF; ++i){
          asm("nop");
          }
          //UART1->DR = 0x64;
          //GPIOB->ODR^=(1<<5);
          //ADC1->CR1|=ADC1_CR1_ADON;
        };
}

#ifdef USE_FULL_ASSERT
void assert_failed(u8 *file, u32 line)
{
  asm("nop");
  return;
}
#endif
