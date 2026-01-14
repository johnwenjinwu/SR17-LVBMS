/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CHG_XRST_Pin GPIO_PIN_1
#define CHG_XRST_GPIO_Port GPIOC
#define CHG_XALERT_Pin GPIO_PIN_2
#define CHG_XALERT_GPIO_Port GPIOC
#define Batt_EEPROM_Pin_Pin GPIO_PIN_3
#define Batt_EEPROM_Pin_GPIO_Port GPIOC
#define Batt_Voltage_Pin GPIO_PIN_0
#define Batt_Voltage_GPIO_Port GPIOA
#define Batt_Current_Pin GPIO_PIN_1
#define Batt_Current_GPIO_Port GPIOA
#define CGH_INT_Pin GPIO_PIN_2
#define CGH_INT_GPIO_Port GPIOA
#define CHG_STAT1_Pin GPIO_PIN_3
#define CHG_STAT1_GPIO_Port GPIOA
#define CHG_STAT2_Pin GPIO_PIN_4
#define CHG_STAT2_GPIO_Port GPIOA
#define Batt_Temp_Pin GPIO_PIN_6
#define Batt_Temp_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
