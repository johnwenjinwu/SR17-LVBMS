
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
	float voltage_buffer[NUM_CELLS];
	float temp_buffer[NUM_THEMISTOR];
	float cell_volt_lowest;
	float cell_volt_highest;
	float cell_volt_avg;
	float cell_volt_diff;
	float cell_volt_sum;
	float temp_avg;
}batt_info;

typedef struct fault_info{
	uint8_t short_circuit;
	uint8_t overload;
	uint8_t over_voltage;
	uint8_t under_voltage;
	uint8_t over_temperature;
	uint8_t vgood;
	uint8_t discharge_fet;
	uint8_t charge_fet;
}fault_info;

void bms_ic_host_control_EN();
void bms_ic_read_voltage(batt_info *b);
void bms_ic_read_faults(fault_info *f);
void bms_ic_balance_cells(batt_info *b);
void bms_ic_top3_sort(batt_info *b, float *top3, uint8_t *indexes);


#endif /* __BMS_IC_H */
