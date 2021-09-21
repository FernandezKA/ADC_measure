#include "general.h"
//User variables
uint8_t u8CountMeasure;
uint8_t u8BuffMeasure[100U];
//User procedure
/******************************************************************************/
//Init functions
/******************************************************************************/
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
  ADC1->CSR|=ADC1_CSR_EOCIE|ADC1_CSR_EOC;
  
ADC1_DeInit();
    ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_2, ADC1_PRESSEL_FCPU_D8,
            ADC1_EXTTRIG_TIM,DISABLE, ADC1_ALIGN_RIGHT,  ADC1_SCHMITTTRIG_CHANNEL2, DISABLE);
    ADC1_DataBufferCmd(ENABLE);
    ADC1_ITConfig(ADC1_IT_EOCIE ,ENABLE);
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
/******************************************************************************/
//ADC1
/******************************************************************************/
//This funnction return mean value from array of samples
uint8_t u8GetMean(uint8_t* data){
  uint32_t u32SumValue = 0;
  for(uint8_t i = 0; i < 100; i++){
    u32SumValue += data[i];
  }
  uint32_t u32Result = u32SumValue/100;
  return 0xFF;
}
//This function configure channel
void vSelectChannel(uint8_t channel){
  if(channel > 15){
    channel = 0;
  }
  ADC1->CSR |= channel;
}
/******************************************************************************/
//UART
/******************************************************************************/
//This function send one byte
bool vUART_Transmit(uint8_t data){
  bool isSend = FALSE;
  if((UART1->SR & UART1_SR_TXE) == UART1_SR_TXE){
    UART1->DR = data;
    isSend = TRUE;
  }
  return isSend;
}
//This function send array of bytes
bool vUART_ArrayTransmit(uint8_t* data, uint8_t size){
  bool isSend = FALSE;
  for(uint8_t i = 0; i < size; ++i){
    bool bSendByte = FALSE;
    while(!bSendByte){
      bSendByte = vUART_Transmit(data[i]);
    }
    isSend = TRUE;
  }
  return isSend;
}
/******************************************************************************/
//IRQ Handlers
/******************************************************************************/
//IRQ handler for TIM4
INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
{
  //Time call is 1 mS
  TIM4->SR1 &=~ TIM4_SR1_UIF;//Clear IRQ flag
  //GPIOB->ODR^=(1<<5);
  ADC1->CR1|=ADC1_CR1_ADON;//Get sample
}
//IRQ handler for ADC1
INTERRUPT_HANDLER(ADC1_IRQHandler, 22)
{
  GPIOB->ODR^=(1<<5);
  u8BuffMeasure[u8CountMeasure++] = ADC1->DRH;
  if(u8CountMeasure == 100){
    u8CountMeasure = 0;
    if(u8CurrentChannel < 15){
      u8CurrentChannel++;
      ADC1->CSR&=~(1<<0|1<<1|1<<2|1<<3);
      ADC1->CSR|=u8CurrentChannel;
      asm("nop");
    }
    else{
      u8CurrentChannel = 0;
    }
  }
  ADC1_ClearITPendingBit(ADC1_IT_EOC);
}