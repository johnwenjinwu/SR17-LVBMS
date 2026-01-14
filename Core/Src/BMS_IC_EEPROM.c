
//The BQ77PL900 uses I2C communication and this library is for programming the built-in EEPROM

#include "main.h"
#include "BMS_IC_EEPROM.h"

// This function is used to check//nihao
void program_eeprom() {

	HAL_I2C_Mem_Read(&hi2c1,BMS_ADDR,0x06,I2C_MEMADD_SIZE_8BIT,&Read_buffer[0],1,100);
	HAL_I2C_Mem_Read(&hi2c1,BMS_ADDR,0x07,I2C_MEMADD_SIZE_8BIT,&Read_buffer[1],1,100);
	HAL_I2C_Mem_Read(&hi2c1,BMS_ADDR,0x08,I2C_MEMADD_SIZE_8BIT,&Read_buffer[2],1,100);
	HAL_I2C_Mem_Read(&hi2c1,BMS_ADDR,0x09,I2C_MEMADD_SIZE_8BIT,&Read_buffer[3],1,100);
	HAL_I2C_Mem_Read(&hi2c1,BMS_ADDR,0x0A,I2C_MEMADD_SIZE_8BIT,&Read_buffer[4],1,100);

	//compare the data from the register with the desired setting before writing to register
	if((Read_buffer[0] == OV_CFG) &&
		(Read_buffer[1] == UV_CFG) &&
		(Read_buffer[2] == OC_UV_DELAY) &&
		(Read_buffer[3] == OCD_CFG) &&
		(Read_buffer[4] == SCD_CFG)) {
//		  HAL_UART_Transmit(&huart1, (uint8_t*) Good_message, 20, 100);
		return;
		}

	//write data to 0x06 to 0x0a
	HAL_I2C_Mem_Write(&hi2c1,BMS_ADDR,0x06,I2C_MEMADD_SIZE_8BIT,&OV_CFG,1,100);
	HAL_I2C_Mem_Write(&hi2c1,BMS_ADDR,0x07,I2C_MEMADD_SIZE_8BIT,&UV_CFG,1,100);
	HAL_I2C_Mem_Write(&hi2c1,BMS_ADDR,0x08,I2C_MEMADD_SIZE_8BIT,&OC_UV_DELAY,1,100);
	HAL_I2C_Mem_Write(&hi2c1,BMS_ADDR,0x09,I2C_MEMADD_SIZE_8BIT,&OCD_CFG,1,100);
	HAL_I2C_Mem_Write(&hi2c1,BMS_ADDR,0x0A,I2C_MEMADD_SIZE_8BIT,&SCD_CFG,1,100);

	//Send 0x62(0110 0010) for  prewriting value
	HAL_I2C_Mem_Write(&hi2c1,BMS_ADDR,0x0B,I2C_MEMADD_SIZE_8BIT,&Pre_Writing,1,100);

	//read data from 0x06 to 0x0A
	HAL_I2C_Mem_Read(&hi2c1,BMS_ADDR,0x06,I2C_MEMADD_SIZE_8BIT,&Read_buffer[0],1,100);
	HAL_I2C_Mem_Read(&hi2c1,BMS_ADDR,0x07,I2C_MEMADD_SIZE_8BIT,&Read_buffer[1],1,100);
	HAL_I2C_Mem_Read(&hi2c1,BMS_ADDR,0x08,I2C_MEMADD_SIZE_8BIT,&Read_buffer[2],1,100);
	HAL_I2C_Mem_Read(&hi2c1,BMS_ADDR,0x09,I2C_MEMADD_SIZE_8BIT,&Read_buffer[3],1,100);
	HAL_I2C_Mem_Read(&hi2c1,BMS_ADDR,0x0A,I2C_MEMADD_SIZE_8BIT,&Read_buffer[4],1,100);

	//compare the data from the register with the desired setting after writing to register
	if((Read_buffer[0] != OV_CFG) ||
		(Read_buffer[1] != UV_CFG) ||
		(Read_buffer[2] != OC_UV_DELAY) ||
		(Read_buffer[3] != OCD_CFG) ||
		(Read_buffer[4] != SCD_CFG)) {
		return;
		}

	//Send write command 0100 0001 to enable data write, wait 100ms, check if VGOOD(bit 5) == 1
	HAL_I2C_Mem_Write(&hi2c1,BMS_ADDR,0x0B,I2C_MEMADD_SIZE_8BIT,&Data_Write_EN,1,100);
	HAL_Delay(100);
	HAL_I2C_Mem_Read(&hi2c1, BMS_ADDR,0x00,I2C_MEMADD_SIZE_8BIT,&VGOOD,1,100);
	if((VGOOD & (0x20)) != 0x20){
//		HAL_UART_Transmit(&huart1, (uint8_t*) DK_message, 20, 100);
		return;
	}

	//Set EEPROM_PIN to HIGH, wait 100ms, set EEPROM_PIN to LOW
	HAL_GPIO_WritePin(Batt_EEPROM_Pin_GPIO_Port, Batt_EEPROM_Pin_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(Batt_EEPROM_Pin_GPIO_Port, Batt_EEPROM_Pin_Pin, GPIO_PIN_RESET);

	//read data from 0x06 to 0x0A
	HAL_I2C_Mem_Read(&hi2c1,BMS_ADDR,0x06,I2C_MEMADD_SIZE_8BIT,&Read_buffer[0],1,100);
	HAL_I2C_Mem_Read(&hi2c1,BMS_ADDR,0x07,I2C_MEMADD_SIZE_8BIT,&Read_buffer[1],1,100);
	HAL_I2C_Mem_Read(&hi2c1,BMS_ADDR,0x08,I2C_MEMADD_SIZE_8BIT,&Read_buffer[2],1,100);
	HAL_I2C_Mem_Read(&hi2c1,BMS_ADDR,0x09,I2C_MEMADD_SIZE_8BIT,&Read_buffer[3],1,100);
	HAL_I2C_Mem_Read(&hi2c1,BMS_ADDR,0x0A,I2C_MEMADD_SIZE_8BIT,&Read_buffer[4],1,100);
//
//	//compare the data from the register with the desired setting after writing to register
//	if((Read_buffer[0] == OV_CFG) &&
//		(Read_buffer[1] == UV_CFG) &&
//		(Read_buffer[2] == OC_UV_DELAY) &&
//		(Read_buffer[3] == OCD_CFG) &&
//		(Read_buffer[4] == SCD_CFG)) {
////		HAL_UART_Transmit(&huart1, (uint8_t*) Done_message, 20, 100);
//		}
//	else{
////		HAL_UART_Transmit(&huart1, (uint8_t*) Bad_message, 20, 100);
//	}
		return;

}

void bms_ic_eeprom_check(){

		//read from register thru I2C
		HAL_I2C_Mem_Read(&hi2c1, BMS_ADDR, 0x06, I2C_MEMADD_SIZE_8BIT, &Read_buffer[0], 1, HAL_MAX_DELAY);
		// Convert to string and send
		sprintf(message_buffer, "OV_CFG: %02X\r\n", Read_buffer[0]);  // or %02X for hex
		//Transmit data thru UART serial monitor
		HAL_UART_Transmit(&huart1, (uint8_t*)message_buffer, strlen(message_buffer), HAL_MAX_DELAY);
		HAL_I2C_Mem_Read(&hi2c1, BMS_ADDR, 0x07, I2C_MEMADD_SIZE_8BIT, &Read_buffer[1], 1, HAL_MAX_DELAY);
		// Convert to string and send
		sprintf(message_buffer, "UV_CFG: %02X\r\n", Read_buffer[1]);  // or %02X for hex
		//Transmit data thru UART serial monitor
		HAL_UART_Transmit(&huart1, (uint8_t*)message_buffer, strlen(message_buffer), HAL_MAX_DELAY);
		HAL_I2C_Mem_Read(&hi2c1, BMS_ADDR, 0x08, I2C_MEMADD_SIZE_8BIT, &Read_buffer[2], 1, HAL_MAX_DELAY);
		// Convert to string and send
		sprintf(message_buffer, "OC_UV_DELAY: %02X\r\n", Read_buffer[2]);  // or %02X for hex
		//Transmit data thru UART serial monitor
		HAL_UART_Transmit(&huart1, (uint8_t*)message_buffer, strlen(message_buffer), HAL_MAX_DELAY);
		HAL_I2C_Mem_Read(&hi2c1, BMS_ADDR, 0x09, I2C_MEMADD_SIZE_8BIT, &Read_buffer[3], 1, HAL_MAX_DELAY);
		// Convert to string and send
		sprintf(message_buffer, "OCD_CFG: %02X\r\n", Read_buffer[3]);  // or %02X for hex
		//Transmit data thru UART serial monitor
		HAL_UART_Transmit(&huart1, (uint8_t*)message_buffer, strlen(message_buffer), HAL_MAX_DELAY);
		HAL_I2C_Mem_Read(&hi2c1, BMS_ADDR, 0x0A, I2C_MEMADD_SIZE_8BIT, &Read_buffer[4], 1, HAL_MAX_DELAY);
		// Convert to string and send
		sprintf(message_buffer, "SCD_CFG: %02X\r\n\n", Read_buffer[4]);  // or %02X for hex
		//Transmit data thru UART serial monitor
		HAL_UART_Transmit(&huart1, (uint8_t*)message_buffer, strlen(message_buffer), HAL_MAX_DELAY);

}
