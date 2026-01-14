
//The BQ77PL900 uses I2C communication and this library is for programming the built-in EEPROM


#include "main.h"
#include "BMS_IC_EEPROM.h"

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;

const uint8_t OV_CFG = 0xE9; 		/* 4.2V, 0.2V Hysteresis, 2.25s delay, 1110 1001 ->111 01 001*/
const uint8_t UV_CFG = 0x1B; 		/* 2.5V, 0.4V Hysteresis, 2.25s delay, 0001 1011 -> 0 0 01 1011*/
const uint8_t OC_UV_DELAY = 0x1A; 	/* 0.4mV/0.004mOhm=10A, 2s delay, 0001 1010 -> 0001 1010*/
const uint8_t OCD_CFG = 0xA4; 		/* 100ms delay, Recover by SO/OC Release, ZVC=0, CBEN=1, 1010 0100 -> 1 0 1 00100 */
const uint8_t SCD_CFG = 0x0F; 		/* 60mV/0.004mOhm=15A, 900us delay,  0000 1111 -> 0000 1111 */
const uint8_t Pre_Writing = 0x62;
const uint8_t Data_Write_EN = 0x41;
const uint8_t Read_buffer[5];

const uint8_t start_reg = 0x06;
const uint8_t VGOOD = 0;
HAL_StatusTypeDef status = HAL_OK;
char *Good_message = "EEPROM match already!\r\n";
char *Bad_message = "EEPROM is Cooked!\r\n";
char *DK_message = "VGOOD is Cooked!\r\n";
char *Done_message = "EEPROM program is Done!\r\n";

char buffer0[4];
char buffer1[4];
char buffer2[4];
char buffer3[4];
char buffer4[4];


// This function is used to check
void program_eeprom() {

	//read data from 0x06 to 0x0a
//	for(int i=0;i<5;i++){
//		status = HAL_I2C_Mem_Read(&hi2c1, BMS_ADDR,(uint16_t)(start_reg + i),I2C_MEMADD_SIZE_8BIT,&Read_buffer[i],1,100);
//		if(status != HAL_OK){
//			HAL_UART_Transmit(&huart1, (uint8_t*) ("I2C is Cooked"), 20, 100);
//		}
//	}
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
		  HAL_I2C_Mem_Read(&hi2c1, BMS_ADDR, 0x00, I2C_MEMADD_SIZE_8BIT, &Read_buffer[0], 1, HAL_MAX_DELAY);
		  // Convert to string and send
		  sprintf(buffer0, "OV_CFG: %02X\r\n", Read_buffer[0]);  // or %02X for hex
		  //Transmit data thru UART serial monitor
		  HAL_UART_Transmit(&huart1, (uint8_t*)buffer0, strlen(buffer0), HAL_MAX_DELAY);
		  HAL_I2C_Mem_Read(&hi2c1, BMS_ADDR, 0x07, I2C_MEMADD_SIZE_8BIT, &Read_buffer[1], 1, HAL_MAX_DELAY);
		  // Convert to string and send
		  sprintf(buffer1, "UV_CFG: %02X\r\n", Read_buffer[1]);  // or %02X for hex
		  //Transmit data thru UART serial monitor
		  HAL_UART_Transmit(&huart1, (uint8_t*)buffer1, strlen(buffer1), HAL_MAX_DELAY);
		  HAL_I2C_Mem_Read(&hi2c1, BMS_ADDR, 0x08, I2C_MEMADD_SIZE_8BIT, &Read_buffer[2], 1, HAL_MAX_DELAY);
		  // Convert to string and send
		  sprintf(buffer2, "OC_UV_DELAY: %02X\r\n", Read_buffer[2]);  // or %02X for hex
		  //Transmit data thru UART serial monitor
		  HAL_UART_Transmit(&huart1, (uint8_t*)buffer2, strlen(buffer2), HAL_MAX_DELAY);
		  HAL_I2C_Mem_Read(&hi2c1, BMS_ADDR, 0x09, I2C_MEMADD_SIZE_8BIT, &Read_buffer[3], 1, HAL_MAX_DELAY);
		  // Convert to string and send
		  sprintf(buffer3, "OCD_CFG: %02X\r\n", Read_buffer[3]);  // or %02X for hex
		  //Transmit data thru UART serial monitor
		  HAL_UART_Transmit(&huart1, (uint8_t*)buffer3, strlen(buffer3), HAL_MAX_DELAY);
		  HAL_I2C_Mem_Read(&hi2c1, BMS_ADDR, 0x0A, I2C_MEMADD_SIZE_8BIT, &Read_buffer[4], 1, HAL_MAX_DELAY);
		  // Convert to string and send
		  sprintf(buffer4, "SCD_CFG: %02X\r\n\n", Read_buffer[4]);  // or %02X for hex
		  //Transmit data thru UART serial monitor
		  HAL_UART_Transmit(&huart1, (uint8_t*)buffer4, strlen(buffer4), HAL_MAX_DELAY);

}
