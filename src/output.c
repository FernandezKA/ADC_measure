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
  double dCoeff = 0;
  uint8_t u8UsedChannel = u8Channel - 1;
  uint8_t u8UsedSubChannel;
  switch(u8Channel){
  case 2:
    u8UsedSubChannel = CH1.u8SubChannel;
    if(CH1.u8SubChannel == 0) { 
      dCoeff = CH1.u8Prescaler_1;
    }
    else{ 
      dCoeff = CH1.u8Prescaler_2;
    }
    break;
    
  case 3:
    u8UsedSubChannel = CH2.u8SubChannel; 
    if(CH2.u8SubChannel == 0) {
      dCoeff = CH2.u8Prescaler_1;
    }
    else{
      dCoeff = CH2.u8Prescaler_2;
    }
    break;
    
  case 4:
    u8UsedSubChannel = CH3.u8SubChannel;
    if(CH3.u8SubChannel == 0) {
      dCoeff = CH3.u8Prescaler_1;
    }
    else{
      dCoeff = CH3.u8Prescaler_2;
    }
    break;
  }
  double dResult = u8Result*dCoeff;
  double dFirst, dSecond;
  dSecond = modf(dResult, &dFirst) * 10;
  uint8_t u8FirstDigit =(uint8_t) dFirst + 0x30;
  uint8_t u8SecondDigit =(uint8_t) dSecond + 0x30;
  uint8_t u8aResultArray[17];
  u8aResultArray[0]= 'c';
  u8aResultArray[1]= 'h';
  u8aResultArray[2]= '.';
  u8aResultArray[3]= '#';
  u8aResultArray[4]= u8UsedChannel + 0x30;
  u8aResultArray[5]= '-';
  u8aResultArray[6]= u8UsedSubChannel + 0x30;
  u8aResultArray[7]= ' ';
  u8aResultArray[8]= 'v';
  u8aResultArray[9]= 'o';
  u8aResultArray[10]= 'l';
  u8aResultArray[11]= 't';
  u8aResultArray[12]= '.';
  u8aResultArray[13]= u8FirstDigit;
  u8aResultArray[14]= ',';
  u8aResultArray[15]= u8SecondDigit;
  u8aResultArray[16]= '\r';
  vUART_ArrayTransmit(u8aResultArray, 17);
}
//uart IRQ HANDLER
INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
  uint8_t u8RecievedCommand = UART1 -> DR;
  switch(u8RecievedCommand){
  case 'm':
    MAIN = select_mode;
    break;
  case 'w':
    MAIN = wait;
    break;
  case 'b':
    MAIN = save;
    break;
  case 'c':
    MAIN = calibrate;
    break;
  case 's':
    MAIN = subprescaler;
    break;
  case 'h':
    MAIN = help;
    break;
  default:
    MAIN = wait;
    break;
  }
  //TODO write FSM for definition
}
//This function return enter digit
uint8_t u8GetDigit(void){
  uint8_t u8EnteredChar = u8UART_Recieve();
  while(u8EnteredChar < 0x30 || u8EnteredChar > 0x39){
    vUART_ArrayTransmit("Wrong symbol\n\r", 14);
    u8EnteredChar = u8UART_Recieve();
  }
  return u8EnteredChar - 0x30;
}
//This function print help
void vPrintHelp(void){
  vUART_ArrayTransmit("This is help page\n\r", 19);
  vUART_ArrayTransmit("Available commands:c-calibrate,m-set mode,s-set prescaler,h-help,b-backup\n\r", 75);
}