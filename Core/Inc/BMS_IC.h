
#ifndef __BMS_IC_H
#define __BMS_IC_H

#include "main.h"

#define BMS_ADDR 0x10 << 1				//Slave ADDR of BMS IC, must be left shifted by 1 for i2c transmit
#define NUM_CELLS 6
#define NUM_THEMISTOR 2

//Cell info buffers
typedef struct batt_info{
	float voltage_buffer[NUM_CELLS];
	float temp_buffer[NUM_THEMISTOR];
	float cell_volt_lowest;
	float cell_volt_highest;
	float cell_volt_avg;
	float cell_volt_diff;
	float cell_volt_sum;
	float temp_avg;
}batt_info;

void bms_ic_host_control_EN();
void bms_ic_read_voltage(batt_info *b);


#endif /* __BMS_IC_H */
