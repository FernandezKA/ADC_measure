#include "general.h"
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