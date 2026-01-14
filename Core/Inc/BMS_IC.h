
#ifndef __BMS_IC_H
#define __BMS_IC_H

#include "main.h"

#define BMS_ADDR 0x10 << 1				//Slave ADDR of BMS IC, must be left shifted by 1 for i2c transmit
#define NUM_CELLS 6
#define NUM_THEMISTOR 2
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;
extern ADC_HandleTypeDef hadc1;


//Cell info buffers
typedef struct batt_info{
	uint16_t voltage_buffer[NUM_CELLS];
	uint16_t temp_buffer[NUM_THEMISTOR];
	uint16_t cell_volt_lowest;
	uint16_t cell_volt_highest;
	uint16_t cell_volt_avg;
	uint16_t cell_volt_diff;
	uint16_t cell_volt_sum;
	uint16_t temp_avg;
}batt_info;


#endif /* __BMS_IC_H */
