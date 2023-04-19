#include "stm8s.h"
#include "EEPROM_save.h"
#include <stdlib.h>
#include "stm8s.h"
#include <stdio.h>

void init_eeprom()
{
  // Unlock access to eeprom
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  // Set programing time
  FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
}

/**
 * @brief  Write any array of 8 bit numbers
 * @param  address: address to start write 32 bit
 * @retval Readed array
 */
void write_eeprom(uint32_t address, int8_t data[], uint8_t length)
{
  for (uint8_t i = 0; i < length; ++i)
  {
    FLASH_ProgramByte(FLASH_DATA_START_PHYSICAL_ADDRESS + address + i, data[i]);
  }
}

/**
 * @brief  Read any array of 8 bit numbers
 * @param  address: address to start write 32 bit
 * @retval Readed array
 * @note Max length is 99
 */
int8_t *read_eeprom(uint32_t address, uint8_t length)
{
  int8_t *data = (int8_t *)malloc(length * sizeof(int8_t));

  for (uint8_t i = 0; i < length; ++i)
  {
    data[i] = FLASH_ReadByte(FLASH_DATA_START_PHYSICAL_ADDRESS + address + i);
  }

  return data;
}
