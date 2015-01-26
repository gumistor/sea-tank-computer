/*
 * kho_i2c_freertos.c
 *
 *  Created on: 1 sie 2014
 *      Author: Krzysztof
 */

#include "kho_i2c_freertos.h"
#include "kho_i2c_taskinit.h"

uint8_t init_I2C(void) {
	uint8_t return_value = I2C_INIT_READY;
	//Initialize i2c on board
	Board_I2C_Init(I2C_ID);
	/* Initialize I2C */
	Chip_I2C_Init(I2C_ID);
	Chip_I2C_SetClockRate(I2C_ID, I2C_SPEED);


	Chip_I2C_SetMasterEventHandler(I2C_ID, Chip_I2C_EventHandler);

	//Enable IRQ
	NVIC_SetPriority(I2C2_IRQn, IRQ_PRIORITY_I2C);
	NVIC_EnableIRQ(I2C2_IRQn);

	xWrite_I2CQueue = xQueueCreate( 5, sizeof(i2c_struct));
	xRead_I2CQueue = xQueueCreate( 5, sizeof(i2c_struct));
	xError_I2CQueue = xQueueCreate( 5, sizeof(i2c_struct));

	taskI2Cready = 0;

	if(NULL == xWrite_I2CQueue || NULL == xRead_I2CQueue || NULL == xError_I2CQueue)
	{
		return_value = I2C_INIT_ERROR;
		configASSERT(0);
	}
	else {
		if(!xTaskCreate(I2CSendTask, NAME_I2CSendTask, STACK_I2CSendTask, NULL, PRIORITY_I2CSendTask, &handleI2CSendTask)) {
			return_value = I2C_INIT_ERROR;
			configASSERT(0);
		}
	}
	return return_value;
}

uint8_t onLineI2C(void) {
	return (handleI2CSendTask != NULL) && taskI2Cready;
}

void I2C2_IRQHandler(void) {
	if (Chip_I2C_IsMasterActive(I2C_ID)) {
		Chip_I2C_MasterStateHandler(I2C_ID);
	} else {
		Chip_I2C_SlaveStateHandler(I2C_ID);
	}
}

void I2CSendTask(void *pvParameters) {
	i2c_struct data_to_send, trash_i2c_msg;
	portBASE_TYPE xStatus;
	int tmp;

	if(uxQueueMessagesWaiting(xWrite_I2CQueue)) {
		xQueueReset(xWrite_I2CQueue);
	}

	if(uxQueueMessagesWaiting(xRead_I2CQueue)) {
			xQueueReset(xRead_I2CQueue);
		}

	if(uxQueueMessagesWaiting(xError_I2CQueue)) {
			xQueueReset(xError_I2CQueue);
		}

	taskI2Cready = 1;

	while (1) {
		xStatus = xQueueReceive(xWrite_I2CQueue, &data_to_send, portMAX_DELAY);

		if( xStatus == pdPASS)
		{
			if(I2C_WRITE == data_to_send.read_notwrite)	{
				tmp = Chip_I2C_MasterSend(I2C_ID, data_to_send.id, data_to_send.data_to_send, data_to_send.size_of_send);
				if(tmp > 0) {
					//
				}
				else {
					if(!xQueueSendToBack(xError_I2CQueue, &data_to_send, 0)) {
						if(xQueueReceive(xError_I2CQueue, &trash_i2c_msg, portMAX_DELAY)) {
							if(!xQueueSendToBack(xError_I2CQueue, &data_to_send, 0)) {
								configASSERT(0);
							}
						}
						else {
							configASSERT(0);
						}
					}
				}
			}
			else {
				if(data_to_send.size_of_send != 0)
				tmp = Chip_I2C_MasterCmdRead(I2C_ID, data_to_send.id, data_to_send.data_to_send[0], &data_to_send.data_received, (int)data_to_send.size_of_received);
				else
				tmp = Chip_I2C_MasterRead(I2C_ID, data_to_send.id, &data_to_send.data_received, (int)data_to_send.size_of_received);
				if(tmp > 0) {
					if(!xQueueSendToBack(xRead_I2CQueue, &data_to_send, 0)) {
						configASSERT(0);
						if(xQueueReceive(xRead_I2CQueue, &trash_i2c_msg, portMAX_DELAY)) {
							if(!xQueueSendToBack(xRead_I2CQueue, &data_to_send, 0)) {
								configASSERT(0);
							}
						}
						else {
							configASSERT(0);
						}
					}
				}
				else {
					if(!xQueueSendToBack(xError_I2CQueue, &data_to_send, 0)) {
						if(xQueueReceive(xError_I2CQueue, &trash_i2c_msg, portMAX_DELAY)) {
							if(!xQueueSendToBack(xError_I2CQueue, &data_to_send, 0)) {
								configASSERT(0);
							}
						}
						else {
							configASSERT(0);
						}
					}
				}
			}
		}
		else
		{
			tmp = 0;
			configASSERT(0);
		}
	}
}
