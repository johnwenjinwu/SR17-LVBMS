
//CAN send function

#include "main.h"
#include "CAN.h"

/*This functions load the batt_info to the CAN message and load to send*/
void can_send(batt_info_t *b, can_id_lookup_t *id, can_message_t *m){
	//Load cell voltages 0-3 to CAN data
	m->can_data[1] = (uint8_t) (b->voltage_buffer[0] >> 8);
	m->can_data[0] = (uint8_t) (b->voltage_buffer[0]);
	m->can_data[2] = (uint8_t) (b->voltage_buffer[1]);
	m->can_data[3] = (uint8_t) (b->voltage_buffer[1] >> 8);
	m->can_data[4] = (uint8_t) (b->voltage_buffer[2]);
	m->can_data[5] = (uint8_t) (b->voltage_buffer[2] >> 8);
	m->can_data[6] = (uint8_t) (b->voltage_buffer[3]);
	m->can_data[7] = (uint8_t) (b->voltage_buffer[3] >> 8);
	//Set CAN ID for message 1
	m->tx_header.StdId = id->bms_message_1_id;
	can_real_send(m);
	
	//Load cell ID for cells 4-5 + highest + lowest to CAN data
	m->can_data[0] = (uint8_t) (b->voltage_buffer[4]);
	m->can_data[1] = (uint8_t) (b->voltage_buffer[4] >> 8);
	m->can_data[2] = (uint8_t) (b->voltage_buffer[5]);
	m->can_data[3] = (uint8_t) (b->voltage_buffer[5] >> 8);
	m->can_data[4] = (uint8_t) (b->cell_volt_highest);
	m->can_data[5] = (uint8_t) (b->cell_volt_highest >> 8);
	m->can_data[6] = (uint8_t) (b->cell_volt_lowest);
	m->can_data[7] = (uint8_t) (b->cell_volt_lowest >> 8);
	//Set CAN ID for message 2
	m->tx_header.StdId = id->bms_message_2_id;
	can_real_send(m);
	
	//load cell_avg, cell_diff, pack voltage, cell current
	m->can_data[0] = (uint8_t) (b->cell_volt_avg);
	m->can_data[1] = (uint8_t) (b->cell_volt_avg >> 8);
	m->can_data[2] = (uint8_t) (b->cell_volt_diff);
	m->can_data[3] = (uint8_t) (b->cell_volt_diff >> 8);
	m->can_data[4] = (uint8_t) (b->cell_volt_sum);
	m->can_data[5] = (uint8_t) (b->cell_volt_sum >> 8);
	m->can_data[6] = (uint8_t) (b->current);
	m->can_data[7] = (uint8_t) (b->current >> 8);
	//Set CAN ID for message 3
	m->tx_header.StdId = id->bms_message_3_id;
	can_real_send(m);

	
}

void can_real_send(can_message_t *m){
	//Check if mailbox is free
	if(HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0){
		uint32_t start_tick = HAL_GetTick();
		//No free mailbox, wait
		while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0){
		//Timeout after 10ms
			if((HAL_GetTick() - start_tick) > 10){
				return; //Exit function if timeout
			}
		}
	}
	HAL_CAN_AddTxMessage(&hcan1, &m->tx_header, &m->can_data[0], &m->tx_mailbox);
}

