#include "general.h"
double bCalibratingCoefficient[3];
//This function get calibrating device
void vGetCalibrate(uint8_t u8ChannelNum){
  vUART_ArrayTransmit("Enter measured voltage\n\r", 24);
  vUART_ArrayTransmit("In form xx, x \n\r", 16);
  //2 gidit of whole, 1 digit of fraq
  //Recieve voltage
  uint8_t u8WholePart = (u8UART_Recieve() - 0x30) * 10;
  u8WholePart = u8WholePart + (u8UART_Recieve() - 0x30);
  uint8_t u8FraqtionalPart = u8UART_Recieve() - 0x30;
  //This is ethalon voltage
  double dEthValue = u8WholePart + u8FraqtionalPart * 0.1;
  //Measure current channel 
  uint8_t u8LastChannelLocal = u8LastChannel;
  vSelectChannel(u8ChannelNum);
  uint32_t u32LocalSum = 0x00;
  for(uint8_t i = 0; i < 0x40; ++i){
    u32LocalSum += ADC1->DRL;
  }
  float u8Mean = u32LocalSum/0x40U;
  vSelectChannel(u8LastChannelLocal);
  bCalibratingCoefficient[u8ChannelNum - 1] =  dEthValue / u8Mean;
}
//This function return calibrating value for current channel
double dGetCalibratingCoefficient(uint8_t u8Channel){
  if(u8Channel < 0x03){
    return bCalibratingCoefficient[u8Channel - 1];
  }
  else{
    return 1;
  }
}