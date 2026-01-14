
#ifndef __BMS_IC_H
#define __BMS_IC_H

#include "main.h"

#define BMS_ADDR 0x10 << 1				//Slave ADDR of BMS IC, must be left shifted by 1 for i2c transmit
#define NUM_CELLS 6
#define NUM_THEMISTOR 2

//Cell info buffers
typedef struct batt_info{
	float voltage_buffer[NUM_CELLS];
	uint16_t temp_buffer[NUM_THEMISTOR];
	uint16_t cell_volt_lowest;
	uint16_t cell_volt_highest;
	uint16_t cell_volt_avg;
	uint16_t cell_volt_diff;
	uint16_t cell_volt_sum;
	uint16_t temp_avg;
}batt_info;

void bms_ic_host_control_EN();
void bms_ic_read_voltage(batt_info *b);


#endif /* __BMS_IC_H */
