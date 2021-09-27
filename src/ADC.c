#include "general.h"

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
