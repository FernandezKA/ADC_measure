#include "general.h"

/******************************************************************************/
//Init functions
/******************************************************************************/
//This procedure configure clocking parameters
void vInitCLK(void)
{
  CLK->CKDIVR = 0x00;
}
//This procedure init Timer4 for set frequency of sampling
void vInitTIM4(void)
{
  TIM4->IER |= TIM4_IER_UIE;
  TIM4->PSCR = (1 << 2 | 1 << 1);
  TIM4->ARR = 0xFF;
  TIM4->CNTR = 0x00;
  TIM4->CR1 |= TIM4_CR1_CEN;
}
//This procedure init ADC for measure voltage
void vInitADC(void)
{
  ADC1->CSR |= ADC1_CSR_EOCIE | ADC1_CSR_EOC;
  ADC1_DeInit();
  ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_2, ADC1_PRESSEL_FCPU_D8,
            ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL2, DISABLE);
  ADC1_DataBufferCmd(ENABLE);
  ADC1->CR2&=~ADC1_CR2_ALIGN;
  ADC1_ITConfig(ADC1_IT_EOCIE, ENABLE);
}
//This procedure config UART
void vInitUART(void)
{
  UART1->BRR2 = 0x02;
  UART1->BRR1 = 0x68;
  UART1->CR2 |= UART1_CR2_TEN | UART1_CR2_REN;
  UART1->CR2|=UART1_CR2_RIEN;
}
//This procedure config GPIO
void vInitGPIO(void)
{
  //LED at devboard
  GPIOB->DDR |= (1 << 5); //Set as out
  GPIOB->CR2 |= (1 << 5); //Set push-pull
  GPIOB->ODR |= (1 << 5); //Invert at active high
  //Configure input channel for ADC
  GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_IN_FL_NO_IT); //Channel 2
  GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_IN_FL_NO_IT); //Channel 3
  GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_IN_FL_NO_IT); //Channel 4
}