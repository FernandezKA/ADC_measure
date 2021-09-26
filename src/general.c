#include "general.h"
//User variables
uint8_t u8CountMeasure;
uint8_t u8BuffMeasure[100U];
uint8_t u8CurrentConfigurateADC = 1;
uint8_t u8LastChannel = 2;
uint8_t u8Prescaler_1;
uint8_t u8Prescaler_2;
uint8_t u8Prescaler_3;
//Definitions EEPROM regions for constants
uint32_t PRESCALER_1 =   0x4000;
uint32_t PRESCALER_2 =   0x4010;
uint32_t PRESCALER_3 =   0x4020;
uint32_t CONFIGURATION = 0x4030;
//User procedure
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
  ADC1_ITConfig(ADC1_IT_EOCIE, ENABLE);
}
//This procedure config UART
void vInitUART(void)
{
  UART1->BRR2 = 0x02;
  UART1->BRR1 = 0x68;
  UART1->CR2 |= UART1_CR2_TEN | UART1_CR2_REN;
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
//ADC1
/******************************************************************************/
//This funnction return mean value from array of samples
uint8_t u8GetMean(uint8_t *data, uint8_t u8Channel)
{
  uint32_t u32SumValue = 0;
  for (uint8_t i = 0; i < 100; i++)
  {
    u32SumValue += data[i];
  }
  uint8_t u8Result = u32SumValue / 100 * u8GetPrescaler(u8Channel);
  return u8Result;
}
//This function configure channel
void vSelectChannel(uint8_t channel)
{
  ADC1->CSR &= ~(1 << 3 | 1 << 2 | 1 << 1 | 1 << 0); //Clear current configurate
  ADC1->CSR |= channel;
}
/******************************************************************************/
//UART
/******************************************************************************/
//This function send one byte
bool vUART_Transmit(uint8_t data)
{
  bool isSend = FALSE;
  if ((UART1->SR & UART1_SR_TXE) == UART1_SR_TXE)
  {
    UART1->DR = data;
    isSend = TRUE;
  }
  return isSend;
}
//This function send array of bytes
bool vUART_ArrayTransmit(uint8_t *data, uint8_t size)
{
  bool isSend = FALSE;
  for (uint8_t i = 0; i < size; ++i)
  {
    bool bSendByte = FALSE;
    while (!bSendByte)
    {
      bSendByte = vUART_Transmit(data[i]);
    }
    isSend = TRUE;
  }
  return isSend;
}
//This function recieve one UART bytes
uint8_t u8UART_Recieve(void)
{
  while ((UART1->SR & UART1_SR_RXNE) != UART1_SR_RXNE)
  {
    asm("nop");
  }
  return UART1->DR;
}
/******************************************************************************/
//EEPROM
/******************************************************************************/
//This function update EEPROM value for channel prescaler
void vUpdateEEPROMChannel(uint8_t u8Channel, uint8_t u8Prescaler)
{
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  switch(u8Channel){
  case 1:
    FLASH_ProgramByte(PRESCALER_1, u8Prescaler);
    FLASH_Lock(FLASH_MEMTYPE_DATA);
    break;
  case 2:
    FLASH_ProgramByte(PRESCALER_2, u8Prescaler);
    FLASH_Lock(FLASH_MEMTYPE_DATA);
    break;
  case 3:
    FLASH_ProgramByte(PRESCALER_3, u8Prescaler);
    FLASH_Lock(FLASH_MEMTYPE_DATA);
    break;
  default:
    FLASH_Lock(FLASH_MEMTYPE_DATA);
    break;
  }
}

//This function upfate EEPROM value for configurated mode
void vUpdateEEPROMConfig(uint8_t u8NumberConfig){
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  FLASH_ProgramByte(CONFIGURATION, u8NumberConfig);
  FLASH_Lock(FLASH_MEMTYPE_DATA);
}

//This function return value of current channel
uint8_t u8GetPrescaler(uint8_t u8Channel)
{
  switch (u8Channel)
  {
  case 1:
    return u8Prescaler_1;//FLASH_ReadByte(PRESCALER_1);
    break;
  case 2:
    return u8Prescaler_2;//FLASH_ReadByte(PRESCALER_2);
    break;
  case 3:
    return u8Prescaler_3;//FLASH_ReadByte(PRESCALER_3);
    break;
  default:
    return 0x01;
    break;
  }
}
//This function load after power on previous setting
void    vUploadValueEEPROM(uint8_t* pPrescaler_1, uint8_t* pPrescaler_2, uint8_t* pPrescaler_3, uint8_t* pMode){
  if(FLASH_ReadByte(PRESCALER_1) != 0x00){
    *pPrescaler_1 = FLASH_ReadByte(PRESCALER_1);
  }
  if(FLASH_ReadByte(PRESCALER_2) != 0x00){
    *pPrescaler_2 = FLASH_ReadByte(PRESCALER_2);
  }
  if(FLASH_ReadByte(PRESCALER_3) != 0x00){
    *pPrescaler_3 = FLASH_ReadByte(PRESCALER_3);
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
    vUART_Transmit(u8GetMean(u8BuffMeasure, u8LastChannel));//Get mean value of voltage
    switch (u8CurrentConfigurateADC)
    {
    case 1:
      vSelectChannel(2);
      break;
    case 2:
      vSelectChannel(3);
      break;
    case 3:
      vSelectChannel(4);
      break;
    case 4:
      if (u8LastChannel == 2)
      {
        u8LastChannel = 3;
        vSelectChannel(3);
      }
      else
      {
        u8LastChannel = 2;
        vSelectChannel(2);
      }
      break;
    case 5:
      if (u8LastChannel == 3)
      {
        u8LastChannel = 4;
        vSelectChannel(4);
      }
      else
      {
        u8LastChannel = 3;
        vSelectChannel(3);
      }
      break;
    case 6:
      if (u8LastChannel == 2)
      {
        u8LastChannel = 4;
        vSelectChannel(4);
      }
      else
      {
        u8LastChannel = 4;
        vSelectChannel(4);
      }
      break;
    case 7:
      switch(u8LastChannel){
      case 2:
        u8LastChannel = 3;
        vSelectChannel(3);
        break;
      case 3:
        u8LastChannel = 4;
        vSelectChannel(4); 
        break;
      case 4: 
        u8LastChannel = 2;
        vSelectChannel(2);
        break;
      }
      break;
    }
    u8CountMeasure = 0;
  }
  ADC1_ClearITPendingBit(ADC1_IT_EOC);
}