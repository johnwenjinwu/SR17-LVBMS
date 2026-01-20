
#ifndef __BMS_IC_H
#define __BMS_IC_H

#include "main.h"

#define BMS_ADDR 0x10 << 1				//Slave ADDR of BMS IC, must be left shifted by 1 for i2c transmit
#define NUM_CELLS 6

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;
extern ADC_HandleTypeDef hadc1;

//Cell info buffers
typedef struct batt_info_t{
	uint16_t voltage_buffer[NUM_CELLS];
	uint16_t temp_buffer;
	uint16_t cell_volt_lowest;
	uint16_t cell_volt_highest;
	uint16_t cell_volt_avg;
	uint16_t cell_volt_diff;
	uint16_t cell_volt_sum;
	uint16_t current;
	uint8_t fault_info;
}batt_info_t;

typedef enum finite_state_machine_t{
	batt_state_standby = 1,
	batt_state_charing = 2,
	batt_state_discharging = 4,
	batt_state_fault = 8,
	batt_state_charging_and_balancing = 16,
	batt_state_balancing = 32,
	batt_state_xx = 64,
	batt_state_xxx = 128
}finite_state_machine_t;

void bms_ic_host_control_EN();
void bms_ic_read_voltage(batt_info_t *b);
void bms_ic_read_current(batt_info_t *b);
void bms_ic_read_temp(batt_info_t *b);
void bms_ic_read_faults(batt_info_t *b);
void bms_ic_balance_cells(batt_info_t *b);
void bms_ic_top3_sort(batt_info_t *b, float *top3, uint8_t *indexes);
void bms_software_protection(batt_info_t *b);

#endif /* __BMS_IC_H */
