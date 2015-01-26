#include "kho_mbr3_freertos.h"
#include "kho_mbr3_inittask.h"

uint8_t init_mbr3(void) {
	uint8_t return_value = MBR_INIT_READY;
	xButtonPressedQueue = xQueueCreate(5, sizeof(ButtonPressedStruct));
	if(NULL == xButtonPressedQueue)
	{
		return_value = MBR_INIT_ERROR;
		configASSERT(0);
	} else {
		if(!xTaskCreate(MBR3ButtonTask, NAME_MBR3ButtonTask, STACK_MBR3ButtonTask, NULL, PRIORITY_MBR3ButtonTask, &handleMBR3ButtonTask)) {
			return_value = MBR_INIT_ERROR;
			configASSERT(0);
		}
	}
	return return_value;
}

uint8_t errorMBR3(void) {
	return mbr3errorCode;
}

uint8_t onLineMBR3(void) {
	return (handleMBR3ButtonTask != NULL) && taskMBR3ready;
}

void setMBR3output(uint8_t mbr3_reg, uint8_t mbr3_bit, uint8_t output_status) {
	i2c_struct data_to_send;

	data_to_send.id = MBR3_I2C_ADDRESS;
	data_to_send.data_to_send[0] = mbr3_reg;
	//if(output_status)
	data_to_send.data_to_send[1] = ~(output_status);// else data_to_send.data_to_send[1] = 0x79;
	data_to_send.size_of_send = 2;
	data_to_send.size_of_received = 0;
	data_to_send.read_notwrite = I2C_WRITE;
	xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);

}

void getMBR3input(void) {
	i2c_struct data_to_send;

	data_to_send.id = MBR3_I2C_ADDRESS;
	data_to_send.data_to_send[0] = MBR3_I2C_BUTTON_STATUS;
	data_to_send.size_of_send = 1;
	data_to_send.size_of_received = 2;
	data_to_send.read_notwrite = I2C_READ;
	xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);
}

void MBR3ButtonTask(void *pvParameters) {
	i2c_struct data_to_send;
	uint8_t new_value;
	ButtonPressedStruct currentButtonStatus;
	portBASE_TYPE xStatus;

	if(uxQueueMessagesWaiting(xButtonPressedQueue)) {
		xQueueReset(xButtonPressedQueue);
	}

	taskMBR3ready = 1;

	while (1) {
		if(pdPASS == xQueuePeek(xRead_I2CQueue, &data_to_send, portMAX_DELAY)) {
			if(MBR3_I2C_ADDRESS == data_to_send.id) {
				if(pdPASS == xQueueReceive(xRead_I2CQueue, &data_to_send, portMAX_DELAY)) {
					//new_value = ((data_to_send.data_received[0] & CS0_OUTPUT_BIT) | 0x01 << 1 | 0x00 << 2 | (data_to_send.data_received[0] & CS3_OUTPUT_BIT) |
					//		(data_to_send.data_received[0] & CS4_OUTPUT_BIT) | (data_to_send.data_received[0] & CS5_OUTPUT_BIT) |
					//		(data_to_send.data_received[0] & CS6_OUTPUT_BIT) | 0x01 << 7);
					//setMBR3output(GPO0_REGISTER, GPO0_OUT_BIT, new_value);
					currentButtonStatus.buttonStatus = BUTTON_NO;
					if(data_to_send.data_received[0] & CS0_OUTPUT_BIT) currentButtonStatus.buttonStatus = PROXIMITY;
					if(data_to_send.data_received[0] & CS3_OUTPUT_BIT) currentButtonStatus.buttonStatus = BUTTON_RIGHT;
					if(data_to_send.data_received[0] & CS4_OUTPUT_BIT) currentButtonStatus.buttonStatus = BUTTON_LEFT;
					if(data_to_send.data_received[0] & CS5_OUTPUT_BIT) currentButtonStatus.buttonStatus = BUTTON_UP;
					if(data_to_send.data_received[0] & CS6_OUTPUT_BIT) currentButtonStatus.buttonStatus = BUTTON_DOWN;
					if(pdPASS != xQueueSendToBack(xButtonPressedQueue, &currentButtonStatus, 0)) {
						mbr3errorCode = MBR3_INIT_ERROR_SEND;
						//configASSERT(0);
					}
				} else {
					mbr3errorCode = MBR3_INIT_ERROR_RECEIVE;
					configASSERT(0);
				}

			}
		}
		else {
			mbr3errorCode = MBR3_INIT_ERROR_RECEIVE;
			configASSERT(0);
		}
	}
}

void setMBR3_CS10_output(uint8_t output_status) {
	setMBR3output(GPO2_REGISTER, GPO2_OUT_BIT, output_status);
}

void setMBR3_CS3_output(uint8_t output_status) {
	setMBR3output(GPO3_REGISTER, GPO3_OUT_BIT, output_status);
}
void setMBR3_CS4_output(uint8_t output_status) {
	setMBR3output(GPO4_REGISTER, GPO4_OUT_BIT, output_status);
}
void setMBR3_CS5_output(uint8_t output_status) {
	setMBR3output(GPO5_REGISTER, GPO5_OUT_BIT, output_status);
}
void setMBR3_CS6_output(uint8_t output_status) {
	setMBR3output(GPO6_REGISTER, GPO6_OUT_BIT, output_status);
}
