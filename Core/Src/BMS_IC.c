/*This library contains functions to interface with the BMS IC over I2C
Function:
Enable host control
Read faults
Read cell and bat voltages
Balance cells
Sort top 3 highest cells
Read temperature
Read current
*/

#include "main.h"
#include "BMS_IC.h"

const uint8_t status_reg = 0x00;  
const uint8_t output_control_reg = 0x01;
const uint8_t state_control_reg = 0x02;
const uint8_t function_control_reg = 0x03;
const uint8_t cell_balance_reg = 0x04;
const uint8_t cell_sel_reg = 0x05;
const uint8_t HOST_EN = 0xC2;
const uint8_t ADC_EN = 0x23;
const uint8_t BAT = 0x33;
const uint8_t DSG = 0x02;
const uint8_t SHDN = 0x01;
const uint8_t CELL_Voltage[10] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
float vadc, adc_val;

void bms_ic_host_control_EN(){
	HAL_I2C_Mem_Write(&hi2c1,BMS_ADDR,state_control_reg,I2C_MEMADD_SIZE_8BIT,&HOST_EN,1,100);
	HAL_I2C_Mem_Write(&hi2c1,BMS_ADDR,function_control_reg,I2C_MEMADD_SIZE_8BIT,&ADC_EN,1,100);
}

//This function reads cell voltages and pack voltage and represents in mV
void bms_ic_read_voltage(batt_info_t *b){
	  for(int i=0; i<6; i++){
		  // Send I2C command to switch cell reading to ADC pin
		  HAL_I2C_Mem_Write(&hi2c1,BMS_ADDR, cell_sel_reg, I2C_MEMADD_SIZE_8BIT, &CELL_Voltage[i],1,100);
		  HAL_Delay(1);
		  //ADC reading
		  HAL_ADC_Start(&hadc1);
		  HAL_ADC_PollForConversion(&hadc1, 100);
		  adc_val = HAL_ADC_GetValue(&hadc1);
		  vadc = (adc_val * 3.3) / 4095.0;
		  b->voltage_buffer[i] = (1000 * ((1.2 - (vadc + 0)) / 0.2));
	  }
	  // Turns on BAT to ADC
	  HAL_I2C_Mem_Write(&hi2c1,BMS_ADDR, function_control_reg, I2C_MEMADD_SIZE_8BIT, &BAT,1,100);
	  HAL_Delay(1);
	  //ADC reading
	  HAL_ADC_Start(&hadc1);
	  HAL_ADC_PollForConversion(&hadc1, 100);
	  adc_val = HAL_ADC_GetValue(&hadc1);
	  vadc = (adc_val * 3.3) / 4095.0;
	  b->cell_volt_sum = (vadc * 1000 / 50);
	  HAL_ADC_Stop(&hadc1);
	  // Turns off Pack to ADC
	  HAL_I2C_Mem_Write(&hi2c1, BMS_ADDR, function_control_reg, I2C_MEMADD_SIZE_8BIT, &ADC_EN,1,100);
}

//This function reads current represents in mV
void bms_ic_read_current(batt_info_t *b){
	//ADC reading
	sConfig.Channel = ADC_CHANNEL_1;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 100);
	adc_val = HAL_ADC_GetValue(&hadc1);
	vadc = (adc_val * 3.3) / 4095.0;
	b->current = (vadc * 1000 / 5);
	HAL_ADC_Stop(&hadc1);
}

void bms_ic_read_temp(batt_info_t *b){
	//ADC reading
	sConfig.Channel = ADC_CHANNEL_6;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 100);
	adc_val = HAL_ADC_GetValue(&hadc1);
	vadc = (adc_val * 3.3) / 4095.0;
	b->temp_buffer = (vadc * 1000 / 5);
	HAL_ADC_Stop(&hadc1);
}

void bms_ic_read_faults(batt_info_t *b){
	uint8_t data = 0x01;
	HAL_I2C_Mem_Read(&hi2c1, BMS_ADDR, status_reg, I2C_MEMADD_SIZE_8BIT, &b->fault_info, sizeof(uint8_t), 100);
	//Clear fault info after reading by toggleling LTCLR to 1 then 0
	HAL_I2C_Mem_Write(&hi2c1, BMS_ADDR, output_control_reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
	data = 0x00;
	HAL_I2C_Mem_Write(&hi2c1, BMS_ADDR, output_control_reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
}
         
/*This function calls the top3 sort function and balances the top 3 highest cells
If top three are not neighbor, balance all 3
If top three are neighbor, check if top two are neighbor
If top two are neighbor, balance only the highest cell
Else balance top two cells
The check uses absolute difference between cell indexes
If difference is 1, they are neighbor*/

void bms_ic_balance_cells(batt_info_t *b){
	//make array to hold top 3 highest cells
	float top3_cells[3] = {0.0, 0.0, 0.0};
	//make array to hold top 3 highest cell indexes
	uint8_t top3_indexes[3] = {0, 0, 0};
	uint8_t balance_cmd = 0x00;
	//Call sort function
	bms_ic_top3_sort(b, top3_cells, top3_indexes);

	//Check if top 3 are neighbor
	// 1. Always balance the highest cell (Rank 0)
    balance_cmd |= (1 << top3_indexes[0]);

    // 2. Add Rank 1 only if it's NOT a neighbor of Rank 0
    if (abs((int)top3_indexes[0] - (int)top3_indexes[1]) > 1) {
        balance_cmd |= (1 << top3_indexes[1]);
        
        // 3. Add Rank 2 only if it's NOT a neighbor of Rank 0 AND Rank 1
        if (abs((int)top3_indexes[2] - (int)top3_indexes[0]) > 1 && 
            abs((int)top3_indexes[2] - (int)top3_indexes[1]) > 1) {
            balance_cmd |= (1 << top3_indexes[2]);
        }
    } 
    else {
        // Rank 1 was a neighbor of Rank 0 (skipped).
        // Check if Rank 2 is far enough from Rank 0 to be included.
        if (abs((int)top3_indexes[2] - (int)top3_indexes[0]) > 1) {
            balance_cmd |= (1 << top3_indexes[2]);
        }
    }
    HAL_I2C_Mem_Write(&hi2c1, BMS_ADDR, cell_balance_reg, I2C_MEMADD_SIZE_8BIT, &balance_cmd, 1, 100);
}

//This function sorts the top 3 highest cells in descending order
void bms_ic_top3_sort(batt_info_t *b, float *top3, uint8_t *indexes){
	float h1 = -1.0, h2 = -1.0, h3 = -1.0;
	uint8_t i1 = 0, i2 = 0, i3 = 0;

	//Find top 3 highest cells cell 1 to cell 6
	for(int i=0; i<NUM_CELLS; i++){
		if(b->voltage_buffer[i] > h1){
			h3 = h2; i3 = i2;
			h2 = h1; i2 = i1;
			h1 = b->voltage_buffer[i]; i1 = i;
		}
		else if(b->voltage_buffer[i] > h2){
			h3 = h2; i3 = i2;
			h2 = b->voltage_buffer[i]; i2 = i;
		}
		else if(b->voltage_buffer[i] > h3){
			h3 = b->voltage_buffer[i]; i3 = i;
		}
	}
	top3[0] = h1; indexes[0] = i1;
	top3[1] = h2; indexes[1] = i2;
	top3[2] = h3; indexes[2] = i3;
}
