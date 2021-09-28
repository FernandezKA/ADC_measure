#include "general.h"
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
//This function send formated text with result at UART
void vUART_SendResult(uint8_t u8Channel,uint8_t u8Result){
  double dResult = u8Result*0.013*u8GetPrescaler(u8LastChannel);
  double dFirst, dSecond;
  dSecond = modf(dResult, &dFirst) * 10;
  uint8_t u8FirstDigit =(uint8_t) dFirst + 0x30;
  uint8_t u8SecondDigit =(uint8_t) dSecond + 0x30;
  uint8_t u8aResultArray[17];
  u8aResultArray[0]= 'c';
  u8aResultArray[1]= 'h';
  u8aResultArray[2]= '.';
  u8aResultArray[3]= '#';
  u8aResultArray[4]= u8Channel + 0x30U;
  u8aResultArray[5]= ',';
  u8aResultArray[6]= ' ';
  u8aResultArray[7]= 'v';
  u8aResultArray[8]= 'o';
  u8aResultArray[9]= 'l';
  u8aResultArray[10]= 't';
  u8aResultArray[11]= '.';
  u8aResultArray[12]= ' ';
  u8aResultArray[13]= u8FirstDigit;
  u8aResultArray[14]= ',';
  u8aResultArray[15]= u8SecondDigit;
  u8aResultArray[16]= '\r';
  vUART_ArrayTransmit(u8aResultArray, 17);
  //vUART_Transmit(u8FirstDigit);
}
//uart IRQ HANDLER
INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
  uint8_t u8RecievedCommand = UART1 -> DR;
  switch(u8RecievedCommand){
  case 'm':
    MAIN = select_mode;
    break;
    
  case 'p':
    MAIN = prescaler_mode;
    break;
    
  case 'w':
    MAIN = wait;
    break;
  case 's':
    MAIN = save;
    break;
  default:
    
    break;
  }
  //TODO write FSM for definition
}