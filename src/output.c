#include "general.h"
/******************************************************************************/
// UART
/******************************************************************************/
// This function send one byte
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
// This function send array of bytes
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
// This function recieve one UART bytes
uint8_t u8UART_Recieve(void)
{
  while ((UART1->SR & UART1_SR_RXNE) != UART1_SR_RXNE)
  {
    asm("nop");
  }
  return UART1->DR;
}
// This function send formated text with result at UART
void vUART_SendResult(uint8_t u8Channel, uint8_t u8Result)
{
  double Result = (3.3 * u8Result)/0xFF;
  double dCoeff = 0;
  uint8_t u8UsedChannel = u8Channel - 1;
  uint8_t u8UsedSubChannel;
  switch (u8Channel)
  {
  case 2:
    u8UsedSubChannel = CH1.u8SubChannel;
    if (CH1.u8SubChannel == 0)
    {
      dCoeff = CH1.u8Prescaler_1;
    }
    else
    {
      dCoeff = CH1.u8Prescaler_2;
    }
    break;

  case 3:
    u8UsedSubChannel = CH2.u8SubChannel;
    if (CH2.u8SubChannel == 0)
    {
      dCoeff = CH2.u8Prescaler_1;
    }
    else
    {
      dCoeff = CH2.u8Prescaler_2;
    }
    break;

  case 4:
    u8UsedSubChannel = CH3.u8SubChannel;
    if (CH3.u8SubChannel == 0)
    {
      dCoeff = CH3.u8Prescaler_1;
    }
    else
    {
      dCoeff = CH3.u8Prescaler_2;
    }
    break;
  }
  double dResult = Result * dCoeff;
  switch (u8Channel){
case 2:
    CH1_Last = dResult;
  break;
  
case 3:
  CH2_Last = dResult;
  break;
  
case 4:
  CH3_Last = dResult;
  break;
}
  double dFirst, dSecond;
  dSecond = modf(dResult, &dFirst) * 10;
  uint8_t u8FirstDigit = (uint8_t)dFirst;
  uint8_t u8SecondDigit = (uint8_t)dSecond + 0x30;
  uint8_t u8aResultArrayL[17];
  uint8_t u8aResultArray[10];
  uint8_t Out[] = {"00,0:00,0:00,0\n\r"};
  switch(OutModeVar){
  case short_out:
    u8aResultArray[0] = '#';
    u8aResultArray[1] = u8UsedChannel + 0x30;
    u8aResultArray[2] = '-';
    u8aResultArray[3] = u8UsedSubChannel + 0x30;
    u8aResultArray[4] = ' ';
    u8aResultArray[5] = u8FirstDigit / 10 + 0x30;
    u8aResultArray[6] = u8FirstDigit % 10 + 0x30;
    u8aResultArray[7] = ',';
    u8aResultArray[8] = u8SecondDigit;
    u8aResultArray[9] = '\r';
    vUART_ArrayTransmit(u8aResultArray, 10);
    break;
    
  case long_out:
    u8aResultArrayL[0] = 'c';
    u8aResultArrayL[1] = 'h';
    u8aResultArrayL[2] = '.';
    u8aResultArrayL[3] = '#';
    u8aResultArrayL[4] = u8UsedChannel + 0x30;
    u8aResultArrayL[5] = '-';
    u8aResultArrayL[6] = u8UsedSubChannel + 0x30;
    u8aResultArrayL[7] = ' ';
    u8aResultArrayL[8] = 'v';
    u8aResultArrayL[9] = 'o';
    u8aResultArrayL[10] = 'l';
    u8aResultArrayL[11] = '.';
    u8aResultArrayL[12] = u8FirstDigit / 10 + 0x30;
    u8aResultArrayL[13] = u8FirstDigit % 10 + 0x30;
    u8aResultArrayL[14] = ',';
    u8aResultArrayL[15] = u8SecondDigit;
    u8aResultArrayL[16] = '\r';
    vUART_ArrayTransmit(u8aResultArrayL, 17);
    break;
    
  case full_out:
  dSecond = modf(CH1_Last, &dFirst) * 10;
  u8FirstDigit = (uint8_t)dFirst;
  u8SecondDigit = (uint8_t)dSecond + 0x30;
  Out[0] = u8FirstDigit/10 + 0x30;
  Out[1] = u8FirstDigit%10 + 0x30;
  Out[3] = u8SecondDigit;
  dSecond = modf(CH2_Last, &dFirst) * 10;
  u8FirstDigit = (uint8_t)dFirst;
  u8SecondDigit = (uint8_t)dSecond + 0x30;
  Out[5] = u8FirstDigit/10 + 0x30;
  Out[6] = u8FirstDigit%10 + 0x30;
  Out[8] = u8SecondDigit;
  dSecond = modf(CH3_Last, &dFirst) * 10;
  u8FirstDigit = (uint8_t)dFirst;
  u8SecondDigit = (uint8_t)dSecond + 0x30;
  Out[10] = u8FirstDigit/10 + 0x30;
  Out[11] = u8FirstDigit%10 + 0x30;
  Out[13] = u8SecondDigit;
  vUART_ArrayTransmit(Out, 16);
    break;
  }
}
// uart IRQ HANDLER
INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
  uint8_t u8RecievedCommand = UART1->DR;
  switch (u8RecievedCommand)
  {
  case 'm':
    MAIN = select_mode;
    break;
  case 'q':
    MAIN = wait;
    break;
  case 'b':
    MAIN = save;
    break;
  case 'p':
   MAIN = prescalers;
   break;
  case 's':
    MAIN = subprescaler;
    break;
  case 'h':
    MAIN = help;
    break;
  case 'o':
    MAIN = out;
    break;
  default:
    MAIN = wait;
    break;
  }
}
// This function return enter digit
uint8_t u8GetDigit(void)
{
  uint8_t u8EnteredChar = u8UART_Recieve();
  while (u8EnteredChar < 0x30 || u8EnteredChar > 0x39)
  {
    vUART_ArrayTransmit("Wrong symbol\n\r", 14);
    u8EnteredChar = u8UART_Recieve();
  }
  return u8EnteredChar - 0x30;
}
// This function print help
void vPrintHelp(void)
{
  vUART_ArrayTransmit("This is help page\n\r", 19);
  vUART_ArrayTransmit("Available commands:m-set mode,s-set subprescaler,o-set output,h-help,b-backup\n\r", 79);
}