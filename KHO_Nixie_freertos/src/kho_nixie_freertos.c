/*
 * kho_nixie_freertos.c
 *
 *  Created on: 3 sie 2014
 *      Author: Krzysztof
 */

#include "kho_nixie_freertos.h"

uint8_t nixieIDs[5] = {PCA_NIXIE_D_ID,0x00,PCA_NIXIE_A_ID,PCA_NIXIE_B_ID,PCA_NIXIE_C_ID};

void UpdateNixieBuffer(void) {
	nixieOffBuffer = 0x00;

	if(nixieNum0 & 0xF0) nixieOffBuffer |= 0x01;
	if(nixieNum0 & 0x0F) nixieOffBuffer |= 0x02;
	if(nixieNum1 & 0xF0) nixieOffBuffer |= 0x80;
	if(nixieNum1 & 0x0F) nixieOffBuffer |= 0x40;
	if(nixieNum2 & 0xF0) nixieOffBuffer |= 0x20;
	if(nixieNum2 & 0x0F) nixieOffBuffer |= 0x10;
}

void SendNixieDigits(void) {
	i2c_struct data_to_send;
	static uint8_t nixieOffBuffer_old = 0xFF;
	static uint8_t nixieNum0_old = 0xFF;
	static uint8_t nixieNum1_old = 0xFF;
	static uint8_t nixieNum2_old = 0xFF;

	UpdateNixieBuffer();

	if(nixieOffBuffer_old != nixieOffBuffer) {
		data_to_send.id = PCA_NIXIE_D_ID;
		data_to_send.data_to_send[0] = nixieOffBuffer;
		data_to_send.size_of_send = 1;
		data_to_send.size_of_received = 0;
		data_to_send.read_notwrite = I2C_WRITE;
		xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);
		nixieOffBuffer_old = nixieOffBuffer;
	}

	if(nixieNum0 != 0xFF && nixieNum0 != nixieNum0_old) {
		data_to_send.id = PCA_NIXIE_A_ID;
		data_to_send.data_to_send[0] = nixieNum0;
		data_to_send.size_of_send = 1;
		data_to_send.size_of_received = 0;
		data_to_send.read_notwrite = I2C_WRITE;
		xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);
		nixieNum0 = nixieNum0_old;
	}

	if(nixieNum1 != 0xFF && nixieNum1 != nixieNum1_old) {
		data_to_send.id = PCA_NIXIE_B_ID;
		data_to_send.data_to_send[0] = nixieNum1;
		data_to_send.size_of_send = 1;
		data_to_send.size_of_received = 0;
		data_to_send.read_notwrite = I2C_WRITE;
		xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);
		nixieNum1 = nixieNum1_old;
	}

	if(nixieNum2 != 0xFF && nixieNum2 != nixieNum2_old) {
		data_to_send.id = PCA_NIXIE_C_ID;
		data_to_send.data_to_send[0] = nixieNum2;
		data_to_send.size_of_send = 1;
		data_to_send.size_of_received = 0;
		data_to_send.read_notwrite = I2C_WRITE;
		xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);
		nixieNum2 = nixieNum2_old;
	}
}

void SendNewNixieValue_str(uint8_t *str, uint8_t len) {
	uint8_t i = 0;
	uint8_t j = 0;


	while(i < len && j < 6 && str[i]) {
		switch(j) {
		case 0:
			if(str[i] > 0x2F && str[i] < 0x3A) {
				nixieNum0 = 0x0F | ((str[i] - 0x30) << 4);
				j++;
			} else {
				if(str[i] == '.') ;
				else {
					//nixieNum0 = 0xF0;
					j++;
				}
			}
			break;
		case 1:
			if(str[i] > 0x2F && str[i] < 0x3A) {
				nixieNum0 = nixieNum0 & (0xF0 | (str[i] - 0x30));
				j++;
			} else {
				if(str[i] == '.') ;
				else{
					//nixieNum0 = nixieNum0 & 0x0F;
					j++;
				}
			}
			break;
		case 2:
			if(str[i] > 0x2F && str[i] < 0x3A) {
				nixieNum1 = 0x0F | ((str[i] - 0x30) << 4);
				j++;
			} else {
				if(str[i] == '.') ;
				else {
					//nixieNum1 = 0xF0;
					j++;
				}
			}
			break;
		case 3:
			if(str[i] > 0x2F && str[i] < 0x3A) {
				nixieNum1 = nixieNum1 & (0xF0 | (str[i] - 0x30));
				j++;
			} else {
				if(str[i] == '.') ;
				else {
					//nixieNum1 = nixieNum1 & 0x0F;
					j++;
				}
			}
			break;
		case 4:
			if(str[i] > 0x2F && str[i] < 0x3A) {
				nixieNum2 = 0x0F | ((str[i] - 0x30) << 4);
				j++;
			} else {
				if(str[i] == '.') ;
				else {
					//nixieNum2 = 0xF0;
					j++;
				}
			}

			break;
		case 5:
			if(str[i] > 0x29 && str[i] < 0x3A && len > i) {
				nixieNum2 = nixieNum2 & (0xF0 | (str[i] - 0x30));
				j++;
			} else {
				if(str[i] == '.') ;
				else {
					//nixieNum2 = nixieNum2 & 0x0F;
					j++;
				}
			}

			break;
		default:
			j = 0;
			break;
		}
	i++;
	}
	SendNixieDigits();
}

void sendNewNixieData(uint8_t *data) {
	static uint8_t old_data[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	int8_t i;
	i2c_struct data_to_send;

	//init frame
	data_to_send.size_of_send = 1;
	data_to_send.size_of_received = 0;
	data_to_send.read_notwrite = I2C_WRITE;
	for(i = 4; i>=0; i--)
	if(/*old_data[i] != data[i] &&*/ nixieIDs[i] != 0x00) {
		data_to_send.id = nixieIDs[i];
		data_to_send.data_to_send[0] = data[i];
		xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);
		old_data[i] = data[i];
	}
}

void SendNewNixieValue_bcd(uint32_t nixie_values) {
	static uint32_t old_nixie_values;
	static uint8_t dataToSend[5] = {0xFF, 0x00, 0x00, 0x00, 0x00};
	uint8_t notSend = 1;
	uint8_t i;
	uint8_t temp_value;

	notSend = 1;
	dataToSend[0] = 0x00;
	for(i = 4; i > 0; i--) {
		temp_value = ((nixie_values >> (8 * (i-1))) & 0xFF);
		if(notSend && (temp_value & 0xF0) != 0x00) {
			notSend = 0;
		} else {
			if(notSend) temp_value |= 0xF0;
		}
		if(notSend && (temp_value & 0x0F) != 0x00) {
			notSend = 0;
		} else {
			if(notSend) temp_value |= 0x0F;
		}
		if(((old_nixie_values >> (8 * (i-1))) & 0x000000FF) != temp_value)	{
			switch(i) {
			case 4: //dots
				;
				break;
			case 3:
				if((temp_value & 0xF0) == 0xF0) { dataToSend[0] |= 0x01; temp_value &= 0x0F; }
				if((temp_value & 0x0F) == 0x0F) { dataToSend[0] |= 0x02; temp_value &= 0xF0; }
				break;
			case 2:
				if((temp_value & 0xF0) == 0xF0) { dataToSend[0] |= 0x80; temp_value &= 0x0F; }
				if((temp_value & 0x0F) == 0x0F) { dataToSend[0] |= 0x40; temp_value &= 0xF0; }
				break;
			case 1:
				if((temp_value & 0xF0) == 0xF0) { dataToSend[0] |= 0x20; temp_value &= 0x0F; }
				if((temp_value & 0x0F) == 0x0F) { dataToSend[0] |= 0x10; temp_value &= 0xF0; }
				break;
			default: 		break;
			}
			dataToSend[5-i] = temp_value;
		}
	}
	dataToSend[1] = (nixie_values >> 24) & 0xFF;
	sendNewNixieData(dataToSend);
	old_nixie_values = nixie_values;
}

void SendNewNixieValue(uint8_t *nixie_values) {
	i2c_struct data_to_send;
	static uint8_t on_off_status = 0;

	//on_off_status = 0x00;
	if((nixie_values[0] & 0xF0) == 0xF0) {
		on_off_status = on_off_status | 0x01;
		nixie_values[0] = nixie_values[0] & 0x0F;
	}
	else {
		if((nixie_values[0] & 0xF0) == 0xE0) on_off_status &= ~0x01;
	}
	if((nixie_values[0] & 0x0F) == 0x0F) {
		on_off_status = on_off_status | 0x02;
		nixie_values[0] = nixie_values[0] & 0xF0;
	}
	else {
		if((nixie_values[0] & 0x0F) == 0x0E) on_off_status &= ~0x02;
	}
	if((nixie_values[1] & 0xF0) == 0xF0) {
		on_off_status = on_off_status | 0x80;
		nixie_values[1] = nixie_values[1] & 0x0F;
	}
	else {
		if((nixie_values[1] & 0xF0) == 0xE0) on_off_status &= ~0x08;
	}
	if((nixie_values[1] & 0x0F) == 0x0F) {
		on_off_status = on_off_status | 0x40;
		nixie_values[1] = nixie_values[1] & 0xF0;
	}
	else {
		if((nixie_values[1] & 0x0F) == 0x0E) on_off_status &= ~0x04;
	}
	if((nixie_values[2] & 0xF0) == 0xF0) {
		on_off_status = on_off_status | 0x20;
		nixie_values[2] = nixie_values[2] & 0x0F;
	}
	else {
		if((nixie_values[2] & 0xF0) == 0xE0) on_off_status &= ~0x20;
	}
	if((nixie_values[3] & 0x0F) == 0x0F) {
		on_off_status = on_off_status | 0x10;
		nixie_values[2] = nixie_values[2] & 0xF0;
	}
	else {
		if((nixie_values[3] & 0x0F) == 0x0E) on_off_status &= ~0x10;
	}

	data_to_send.id = PCA_NIXIE_D_ID;
	data_to_send.data_to_send[0] = on_off_status;
	data_to_send.size_of_send = 1;
	data_to_send.size_of_received = 0;
	data_to_send.read_notwrite = I2C_WRITE;
	xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);

	if(nixie_values[0] != 0xEE) {
		data_to_send.id = PCA_NIXIE_A_ID;
		data_to_send.data_to_send[0] = nixie_values[0];
		data_to_send.size_of_send = 1;
		data_to_send.size_of_received = 0;
		data_to_send.read_notwrite = I2C_WRITE;
		xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);
	}

	if(nixie_values[1] != 0xEE) {
		data_to_send.id = PCA_NIXIE_B_ID;
		data_to_send.data_to_send[0] = nixie_values[1];
		data_to_send.size_of_send = 1;
		data_to_send.size_of_received = 0;
		data_to_send.read_notwrite = I2C_WRITE;
		xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);
	}

	if(nixie_values[2] != 0xEE) {
		data_to_send.id = PCA_NIXIE_C_ID;
		data_to_send.data_to_send[0] = nixie_values[2];
		data_to_send.size_of_send = 1;
		data_to_send.size_of_received = 0;
		data_to_send.read_notwrite = I2C_WRITE;
		xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);
	}

}
