
#ifndef INC_CAN_H_
#define INC_CAN_H_

#include "main.h"
#include "BMS_IC.h"

extern CAN_HandleTypeDef hcan1;

//create a lookup table struct for CAN id
typedef struct can_id_lookup_t{
	uint8_t bms_message_1_id; //ID for cells 0-3
	uint8_t bms_message_2_id; //ID for cells 4-5 + highest + lowest
	uint8_t bms_message_3_id; //ID for cell_avg, cell_diff, pack voltage, cell current
	uint8_t bms_message_4_id; //ID for temperatures + fault info + balancing status
} can_id_lookup_t;

typedef struct can_message_t{
	CAN_TxHeaderTypeDef tx_header;
	uint8_t can_data[8];
	uint32_t tx_mailbox;
}can_message_t;

void can_send(batt_info_t *b, can_id_lookup_t *id, can_message_t *m);
void can_real_send(can_message_t *m);

#endif /* INC_CAN_H_ */
