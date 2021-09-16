#include "general.h"
//User procedure
//This procedure configure clocking parameters
void vInitCLK(void){
   CLK->CKDIVR = 0x00;
}
//This procedure init Timer4 for set frequency of sampling
void vInitTIM4(void){
  
}
//This procedure init ADC for measure voltage
void vInitADC(void){
  
}
//This procedure config UART
void vInitUART(void){
  UART1->BRR2 = 0x02;
  UART1->BRR1 = 0x68;
  UART1->CR2 |= UART1_CR2_TEN | UART1_CR2_REN;
}