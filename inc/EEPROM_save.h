#ifndef __EEPROM_SAVE_H__
#define __EEPROM_SAVE_H__

void init_eeprom();
void write_eeprom(uint32_t address, int8_t data[], uint8_t length);
int8_t *read_eeprom(uint32_t address, uint8_t length);

#endif