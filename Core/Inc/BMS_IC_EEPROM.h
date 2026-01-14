
#ifndef __BMS_IC_EEPROM_H
#define __BMS_IC_EEPROM_H

#include "main.h"

#define BMS_ADDR 0x10 << 1				//Slave ADDR of BMS IC, must be left shifted by 1 for i2c transmit

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;

const uint8_t OV_CFG = 0xE9; 		/* 4.2V, 0.2V Hysteresis, 2.25s delay, 1110 1001 ->111 01 001*/
const uint8_t UV_CFG = 0x1B; 		/* 2.5V, 0.4V Hysteresis, 2.25s delay, 0001 1011 -> 0 0 01 1011*/
const uint8_t OC_UV_DELAY = 0x1A; 	/* 0.4mV/0.004mOhm=10A, 2s delay, 0001 1010 -> 0001 1010*/
const uint8_t OCD_CFG = 0xA4; 		/* 100ms delay, Recover by SO/OC Release, ZVC=0, CBEN=1, 1010 0100 -> 1 0 1 00100 */
const uint8_t SCD_CFG = 0x0F; 		/* 60mV/0.004mOhm=15A, 900us delay,  0000 1111 -> 0000 1111 */
const uint8_t Pre_Writing = 0x62;
const uint8_t Data_Write_EN = 0x41;

const uint8_t start_reg = 0x06;
const uint8_t VGOOD = 0;
HAL_StatusTypeDef status = HAL_OK;
char *Good_message = "EEPROM match already!\r\n";
char *Bad_message = "EEPROM is Cooked!\r\n";
char *DK_message = "VGOOD is Cooked!\r\n";
char *Done_message = "EEPROM program is Done!\r\n";

uint8_t Read_buffer[5];
char buffer[5][5];

void program_eeprom(void);
void bms_ic_eeprom_check(void);

#endif /* __BMS_IC_EEPROM_H */
