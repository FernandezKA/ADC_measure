#include "general.h"
//User variables
uint8_t u8CountMeasure;
uint8_t u8BuffMeasure[SAMPLES];
uint8_t u8CurrentConfigurateADC = 1;
uint8_t u8LastChannel = 2;
uint8_t u8UsedChannel_1 = 0;
uint8_t u8UsedChannel_2 = 0;
uint8_t u8UsedChannel_3 = 0;
CS CH1, CH2, CH3;
static uint8_t u8LedCounter = 0x00;
//Definitions EEPROM regions for constants
uint32_t PRESCALER =   0x4000;
uint32_t CONFIGURATION = 0x4030;
//User procedure

/******************************************************************************/
//General logic functions of device
/******************************************************************************/
/******************************************************************************/
//IRQ Handlers
/******************************************************************************/
//IRQ handler for TIM4
//This interrupt define sampling frequency and indicate device activity with LED 
INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
{
  //Time call is 1 mS
  TIM4->SR1 &= ~TIM4_SR1_UIF; //Clear IRQ flag
  if(u8LedCounter < 100){
    u8LedCounter += 1;
  }
  else{
    u8LedCounter = 0x00;
    GPIOB->ODR^=(1<<5);
  }
  ADC1->CR1 |= ADC1_CR1_ADON; //Get sample
}

//IRQ handler for ADC1
//This interrupt handled after end of measure ADC value
INTERRUPT_HANDLER(ADC1_IRQHandler, 22)
{
  u8BuffMeasure[u8CountMeasure++] = ADC1->DRH;//Handle only 8 bits
  if (u8CountMeasure == SAMPLES)//We use sum for find mean value
  { //Select mode
        vUART_SendResult(u8LastChannel, u8GetMean(u8BuffMeasure, u8LastChannel));
        u8CountMeasure = 0;
        vGetMultiplex(&u8LastChannel, u8CurrentConfigurateADC);
  }
  ADC1_ClearITPendingBit(ADC1_IT_EOC);
}

void vExportData(void){
  CH1.u8Prescaler_1 = bCalibratingCoefficient[0];
  CH1.u8Prescaler_2 = bCalibratingCoefficient[1];
  CH2.u8Prescaler_1 = bCalibratingCoefficient[2];
  CH2.u8Prescaler_2 = bCalibratingCoefficient[3];
  CH3.u8Prescaler_1 = bCalibratingCoefficient[4];
  CH3.u8Prescaler_2 = bCalibratingCoefficient[5];
}