
#ifndef INC_CAN_H_
#define INC_CAN_H_

#include "main.h"

extern CAN_HandleTypeDef hcan1;

typedef struct can_message_t{
	CAN_TxHeaderTypeDef tx_header;
	uint8_t can_data[8];
	uint32_t tx_mailbox;
}can_message_t;

void can_send(can_message_t *m);

#endif /* INC_CAN_H_ */
