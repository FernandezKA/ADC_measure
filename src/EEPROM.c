#include "general.h"
/******************************************************************************/
//EEPROM
/******************************************************************************/

void getBackup(uint8_t* w_mode, enum OutMode* o_mode, uint8_t* SCH1, uint8_t* SCH2, uint8_t* SCH3){
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  FLASH_ProgramByte(CONFIGURATION, *w_mode);
  FLASH_ProgramByte(OUTPUT, *o_mode);
  FLASH_ProgramByte(PRESCALER_1, *SCH1);
  FLASH_ProgramByte(PRESCALER_2, *SCH2);
  FLASH_ProgramByte(PRESCALER_3, *SCH3);
  FLASH_Lock(FLASH_MEMTYPE_DATA);
}

void getRestore(uint8_t* w_mode, enum OutMode* o_mode, uint8_t* SCH1, uint8_t* SCH2, uint8_t* SCH3){
  *w_mode = FLASH_ReadByte(CONFIGURATION);
  *o_mode = (enum OutMode)  FLASH_ReadByte(OUTPUT);
  *SCH1 =  FLASH_ReadByte(PRESCALER_1);
  *SCH2 =  FLASH_ReadByte(PRESCALER_2);
  *SCH3 =  FLASH_ReadByte(PRESCALER_3);
}