#include "eeprom.h"
#include <stdint.h>
#include <string.h>


#define EEPROM_MAX_SIZE 1024
static uint8_t g_eeprom_arr [EEPROM_MAX_SIZE];


unsigned char eeprom_get_char (unsigned int addr) {
  if (addr >= EEPROM_MAX_SIZE) return 0;
  return g_eeprom_arr [addr];
}

void eeprom_put_char(unsigned int addr, unsigned char new_value) {
  if (addr >= EEPROM_MAX_SIZE) return 0;
  g_eeprom_arr [addr] = new_value;
}

void memcpy_to_eeprom_with_checksum (
  unsigned int destination,
  char *source,
  unsigned int size
) {
  if (destination >= EEPROM_MAX_SIZE) return;
  if (size > EEPROM_MAX_SIZE - destination) {
    size = EEPROM_MAX_SIZE - destination;
  }
  memcpy (&g_eeprom_arr [destination], source, size);
}

int memcpy_from_eeprom_with_checksum (
  char *destination,
  unsigned int source,
  unsigned int size
) {
  if (source >= EEPROM_MAX_SIZE) return;
  if (size > EEPROM_MAX_SIZE - source) {
    size = EEPROM_MAX_SIZE - source;
  }
  memcpy (destination, &g_eeprom_arr [source], size);
  return 1;
}

