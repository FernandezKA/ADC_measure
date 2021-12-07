#include "general.h"
double bCalibratingCoefficient[6]  = {3, 14, 3, 14, 3, 14};
//This function get calibrating device
void vGetCalibrate(uint8_t u8ChannelNum){
  //Get info
   vUART_ArrayTransmit("Enter subnumber\n\r", 17);
  //Enter channel number
  uint8_t u8Subnumber = u8GetDigit();
  //Enter voltage
  vUART_ArrayTransmit("Enter measured voltage\n\r", 24);
  vUART_ArrayTransmit("In form xx, x \n\r", 16);
  //2 gidit of whole, 1 digit of fraq
  //Recieve voltage
  uint8_t u8WholePart = (u8GetDigit()) * 10;
  u8WholePart = u8WholePart + (u8GetDigit());
  uint8_t u8FraqtionalPart = u8GetDigit();
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
  uint8_t u8DebugIndex = (u8ChannelNum * 2) - 2 + u8Subnumber;
  bCalibratingCoefficient[u8DebugIndex] =  dEthValue / u8Mean;
}
//This function return calibrating value for current channel
double dGetCalibratingCoefficient(uint8_t u8Channel, uint8_t u8Subnumber){
  return bCalibratingCoefficient[(u8Channel *2) - 2 + u8Subnumber];
}
//This function select subchannel
void vSelechSub(uint8_t u8Channel, uint8_t u8Subchannel){
  switch(u8Channel){
  case 1:
    CH1.u8SubChannel = u8Subchannel;
    break;
    
  case 2:
    CH2.u8SubChannel = u8Subchannel;
    break;
    
  case 3:
    CH3.u8SubChannel = u8Subchannel;
    break;
  }
}