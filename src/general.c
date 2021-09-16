#include "general.h"
//User procedure
//This procedure configure clocking parameters
void vInitCLK(void){
   CLK->CKDIVR = 0x00;
}
//This procedure init Timer4 for set frequency of sampling
void vInitTIM4(void){
 TIM4->IER |= TIM4_IER_UIE; 
 TIM4->PSCR = (1<<2|1<<1);
 TIM4->ARR = 0xFF;
 TIM4->CNTR = 0x00;
 TIM4->CR1|=TIM4_CR1_CEN;
}
//This procedure init ADC for measure voltage
void vInitADC(void){
    asm("nop");
    ADC1->CR1|=ADC1_CR1_ADON;
    while((ADC1->CSR & ADC1_CSR_EOC) != ADC1_CSR_EOC) asm("nop");
    asm("nop");
}
//This procedure config UART
void vInitUART(void){
  UART1->BRR2 = 0x02;
  UART1->BRR1 = 0x68;
  UART1->CR2 |= UART1_CR2_TEN | UART1_CR2_REN;
}
//This procedure config GPIO
void vInitGPIO(void){
  GPIOB->DDR|=(1<<5);//Set as out
  GPIOB->CR2|=(1<<5);//Set push-pull
  GPIOB->ODR|=(1<<5);//Invert at active high
}
//IRQ handler for TIM4
INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
{
  //Time call is 1 mS
  TIM4->SR1 &=~ TIM4_SR1_UIF;//Clear IRQ flag
  GPIOB->ODR^=(1<<5);
}