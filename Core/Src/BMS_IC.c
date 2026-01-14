#include "main.h"
#include "BMS_IC.h"

//

#define OFFSET_CELL_ADC 0.047
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;
extern ADC_HandleTypeDef hadc1;
const uint8_t status_reg = 0x00; // 
const uint8_t output_control_reg = 0x01;
const uint8_t state_control_reg = 0x02;
const uint8_t function_control_reg = 0x03;
const uint8_t cell_balance_reg = 0x04;
const uint8_t cell_sel_reg = 0x05;
uint8_t HOST_EN = 0x42;
uint8_t VAEN, VGAIN = 0x01;
uint8_t PACK = 0x09;
uint8_t DSG = 0x02;
uint8_t SHDN = 0x01;
uint8_t CELL_Voltage[10] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
float vadc, adc_val;


////This function reads and returns the status of the BMS IC
//void BMS_IC_STATUS (){
//	uint8_t status_buffer[1];
//	HAL_I2C_Mem_Read(&hi2c1, BMS_ADDR, status_reg, I2C_MEMADD_SIZE_8BIT, status_buffer, 1, HAL_MAX_DELAY);
//    return status_buffer[0];
//}
//
//void BMS_IC_

void bms_ic_host_control_EN(){
	HAL_I2C_Mem_Write(&hi2c1,BMS_ADDR,0x02,I2C_MEMADD_SIZE_8BIT,&HOST_EN,1,100);
	HAL_I2C_Mem_Write(&hi2c1,BMS_ADDR,0x03,I2C_MEMADD_SIZE_8BIT,&VAEN,1,100);
	HAL_I2C_Mem_Write(&hi2c1,BMS_ADDR,0x03,I2C_MEMADD_SIZE_8BIT,&VGAIN,1,100);
}
void bms_ic_read_voltage(batt_info *b){

	  for(int i=0; i<6; i++){
		  // Send I2C command to switch cell reading to ADC pin
		  HAL_I2C_Mem_Write(&hi2c1,BMS_ADDR, 0x05, I2C_MEMADD_SIZE_8BIT, &CELL_Voltage[i],1,100);
		  HAL_Delay(5);
		  //ADC reading
		  HAL_ADC_Start(&hadc1);
		  HAL_ADC_PollForConversion(&hadc1, 100);
		  adc_val = HAL_ADC_GetValue(&hadc1);
		  vadc = (adc_val * 3.3) / 4095.0;
		  b->voltage_buffer[i] = ((1.2 - (vadc + 0.047)) / 0.2);
	  }
//	  // Turns on Pack to ADC
//	  HAL_I2C_Mem_Write(&hi2c1,BMS_ADDR, 0x03, I2C_MEMADD_SIZE_8BIT, &PACK,1,100);
//	  HAL_Delay(5);
//	  //ADC reading
//	  HAL_ADC_Start(&hadc1);
//	  HAL_ADC_PollForConversion(&hadc1, 100);
//	  adc_val = HAL_ADC_GetValue(&hadc1);
//	  vadc = (adc_val * 3.3) / 4095.0;
//	  b->cell_volt_sum = (vadc / 50);
//	  // Turns off Pack to ADC
//	  HAL_I2C_Mem_Write(&hi2c1,BMS_ADDR, 0x03, I2C_MEMADD_SIZE_8BIT, &VAEN,1,100);

}
