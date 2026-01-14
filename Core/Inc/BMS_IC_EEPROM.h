
#ifndef __BMS_IC_EEPROM_H
#define __BMS_IC_EEPROM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

void program_eeprom(void);
#define BMS_ADDR 0x10 << 1				//Slave ADDR of BMS IC, must be left shifted by 1 for i2c transmit
#ifdef __cplusplus
}
#endif

#endif /* __BMS_IC_EEPROM_H */
