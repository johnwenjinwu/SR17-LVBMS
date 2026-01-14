#include "main.h"
#include "BMS_IC.h"



//


const uint8_t status_reg = 0x00; // 
const uint8_t output_control_reg = 0x01;
const uint8_t state_control_reg = 0x02;
const uint8_t function_control_reg = 0x03;
const uint8_t cell_balance_reg = 0x04;
const uint8_t cell_sel_reg = 0x05;
uint8_t HOST_EN = 0x42;
uint8_t VAEN = 0x01;
uint8_t PACK = 0x09;
uint8_t DSG = 0x02;
uint8_t SHDN = 0x01;
uint8_t Voltage_Read_buffer;
uint8_t buffer;
uint8_t CELL_Voltage[10] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
uint32_t adc_raw;
float vadc, actual_voltage, actual;
uint32_t adc_val;
char msg[30];


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
}
void bms_ic_read_voltage(){

	  HAL_I2C_Mem_Read(&hi2c1, BMS_ADDR, 0x00, I2C_MEMADD_SIZE_8BIT, &Voltage_Read_buffer, 1, HAL_MAX_DELAY);
	  // Convert to string and send
	  sprintf(buffer, "status: %02X\r\n", Voltage_Read_buffer);  // or %02X for hex
	  //Transmit data thru UART serial monitor
	  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);

	  for(int i=0; i<10; i++){
		  HAL_I2C_Mem_Write(&hi2c1,BMS_ADDR, 0x05, I2C_MEMADD_SIZE_8BIT, &CELL_Voltage[i],1,100);
		  HAL_I2C_Mem_Read(&hi2c1, BMS_ADDR, 0x05, I2C_MEMADD_SIZE_8BIT, Voltage_Read_buffer, 1, HAL_MAX_DELAY);
		  // Convert to string and send
		  sprintf(buffer, "Cell: %02X\r\n", Voltage_Read_buffer);  // or %02X for hex
		  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
		  HAL_Delay(10);
		  //ADC reading
		  HAL_ADC_Start(&hadc1);
		  HAL_ADC_PollForConversion(&hadc1, 100);
		  adc_val = HAL_ADC_GetValue(&hadc1);
		  vadc = (adc_val * 3.3) / 4095.0;
		  actual = (1.2 - (vadc + 0.047)) / 0.2;
		  msg[30];
		  sprintf(msg, "%0.3fV\r\n", actual);
		  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

		  HAL_Delay(10);
	  }

	  HAL_I2C_Mem_Write(&hi2c1,BMS_ADDR, 0x03, I2C_MEMADD_SIZE_8BIT, &PACK,1,100);
	  //ADC reading
	  HAL_ADC_Start(&hadc1);
	  HAL_ADC_PollForConversion(&hadc1, 100);
	  adc_val = HAL_ADC_GetValue(&hadc1);
	  vadc = (adc_val * 3.3) / 4095.0;
	  actual = (vadc+0.047) * 50;
	  msg[30];
	  sprintf(msg, "Pack: %0.3fV\r\n", actual);
	  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
	  HAL_I2C_Mem_Write(&hi2c1,BMS_ADDR,0x03,I2C_MEMADD_SIZE_8BIT, &VAEN,1,100);
}
