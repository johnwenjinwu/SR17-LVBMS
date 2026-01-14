
//CAN send function

#include "main.h"
#include "CAN.h"

void can_send(can_message *m){
	m->can_data[0] = 12;
	m->can_data[1] = 34;
	m->can_data[2] = 56;
	m->can_data[3] = 78;

	HAL_CAN_AddTxMessage(&hcan1, &m->tx_header, &m->can_data[0], &m->tx_mailbox);
}
