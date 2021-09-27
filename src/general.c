#include "general.h"
//User variables
uint8_t u8CountMeasure;
uint8_t u8BuffMeasure[100U];
uint8_t u8CurrentConfigurateADC = 1;
uint8_t u8LastChannel = 2;
uint8_t u8Prescaler_1 = 1;
uint8_t u8Prescaler_2 = 1;
uint8_t u8Prescaler_3 = 1;
//Definitions EEPROM regions for constants
uint32_t PRESCALER_1 =   0x4000;
uint32_t PRESCALER_2 =   0x4010;
uint32_t PRESCALER_3 =   0x4020;
uint32_t CONFIGURATION = 0x4030;
//User procedure

/******************************************************************************/
//General logic functions of device
/******************************************************************************/
//This function select next action of device
enum action eGetAction(void)
{
  uint8_t u8Request = u8UART_Recieve();
  switch (u8Request)
  {
  case 'p':
    return (enum action) prescaler;
    break;
  case 'm':
    return (enum action) select;
    break;
  default:
    return (enum action) select;
    break;
  }
}

    
/******************************************************************************/
//IRQ Handlers
/******************************************************************************/
//IRQ handler for TIM4
INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
{
  //Time call is 1 mS
  TIM4->SR1 &= ~TIM4_SR1_UIF; //Clear IRQ flag
  ADC1->CR1 |= ADC1_CR1_ADON; //Get sample
}

//IRQ handler for ADC1
INTERRUPT_HANDLER(ADC1_IRQHandler, 22)
{
  u8BuffMeasure[u8CountMeasure++] = ADC1->DRL;
  if (u8CountMeasure == 100)
  { //Select mode
    vUART_SendResult(u8LastChannel, u8GetMean(u8BuffMeasure, u8LastChannel));
        u8CountMeasure = 0;
        vGetMultiplex(&u8LastChannel, u8CurrentConfigurateADC);
  }
  ADC1_ClearITPendingBit(ADC1_IT_EOC);
}