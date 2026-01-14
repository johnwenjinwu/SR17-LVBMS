
//The BQ25756E uses I2C communication and this library is for programming the built-in EEPROM

#include "main.h"

#define Charge_ADDR 0x6A
/* These are analog front end programmed already
#define Charge_Voltage_Limit 0x0
#define Charge_Current_Limit 0x2
#define Input_Current_DPM_Limit 0x6
#define Input_Voltage_DPM_Limit 0x8
#define Precharge_Current_Limit 0x10
#define Termination_Current_Limit 0x12
*/

/*
 * We don't need these fancy features
#define MPPT_Control 0x1A
#define Reverse_Mode_Input_Current_Limit 0xA
#define Reverse_Mode_Input_Voltage_Limit 0xC
#define Three_Stage_Charge_Control 0x16
#define TS_Charging_Threshold_Control 0x1B
#define TS_Charging_Region_Behavior_Control 0x1C
#define TS_Reverse_Mode_Threshold_Control 0x1D
#define Reverse_Undervoltage_Control 0x1E
#define VAC_Max_Power_Point_Detected 0x1F
#define Reverse_Mode_Battery_Discharge_Current 0x62
#define Gate_Driver_Strength_Control 0x3B
#define Gate_Driver_Dead_Time_Control 0x3C
#define Part_Information 0x3D
*/

/* These are the control*/
#define Precharge_and_Termination_Control 0x14
#define Timer_Control 0x15
#define Charger_Control 0x17
#define Pin_Control 0x18
#define Power_Path_and_Reverse_Mode_Control 0x19

/* These are the status*/
#define Charger_Status_1 0x21
#define Charger_Status_2 0x22
#define Charger_Status_3 0x23
#define Fault_Status 0x24

/* These are real-time status*/
#define Charger_Flag_1 0x25
#define Charger_Flag_2 0x26
#define Fault_Flag 0x27

/* These enable events or changes to pulse the INT*/
#define Charger_Mask_1 0x28
#define Charger_Mask_2 0x29
#define Fault_Mask 0x2A

/* ADC Monitoring (Optional - 6 registers) */
#define ADC_Control 0x2B
#define ADC_Channel_Control 0x2C
#define IAC_ADC 0x2D
#define IBAT_ADC 0x2F
#define VAC_ADC 0x31
#define VBAT_ADC 0x33
#define TS_ADC 0x37
#define VFB_ADC 0x39
