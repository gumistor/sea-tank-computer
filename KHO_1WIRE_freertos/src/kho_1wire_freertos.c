/*
 * kho_1wire_freertos.c
 *
 *  Created on: 15 gru 2014
 *      Author: Krzysztof
 */

#include "kho_1wire_freertos.h"
#include "kho_1wire_taskinit.h"
#include "semphr.h"

uint8_t oneWireBuffor[20] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x33,0x10};
uint8_t read_notwrite;
uint8_t countBit = 0;
uint8_t countByte = 9;
uint8_t bitValue;

#define TICK_1us 24

void TIMER0_IRQHandler(void)
{
	int i;
	//NVIC_ClearPendingIRQ(TIMER0_IRQn);
	for(i = 0; i < 4; i++) {
		if(Chip_TIMER_MatchPending(LPC_TIMER0, i)) {
			Chip_TIMER_ClearMatch(LPC_TIMER0, i);
			if(i == 0)	if(oneWireBuffor[countByte] & 1) one1WiresetHigh();
			if(i == 1)	{
				oneWireBuffor[countByte] = (oneWireBuffor[countByte] >> 1) & 0b01111111;
				oneWireBuffor[countByte] |= (Chip_GPIO_GetPinState(LPC_GPIO, GPIO_1WIRE_INPUT_PORT, GPIO_1WIRE_INPUT_PIN) << 7);
			}
			if(i == 2)	one1WiresetHigh();
			if(i == 3)	{
				Chip_TIMER_Disable(LPC_TIMER0);
				Chip_TIMER_Reset(LPC_TIMER0);
				Chip_TIMER_SetMatch(LPC_TIMER0, 0, TICK_1us*1);
				Chip_TIMER_MatchEnableInt(LPC_TIMER0, 0);
				Chip_TIMER_SetMatch(LPC_TIMER0, 1, (4+1)*TICK_1us);
				Chip_TIMER_MatchEnableInt(LPC_TIMER0, 1);
				Chip_TIMER_SetMatch(LPC_TIMER0, 2, (1+10+49)*TICK_1us);
				Chip_TIMER_MatchEnableInt(LPC_TIMER0, 2);
				Chip_TIMER_SetMatch(LPC_TIMER0, 3, (1+10+49+1)*TICK_1us);
				Chip_TIMER_MatchEnableInt(LPC_TIMER0, 3);
				if(countBit--) {
					one1WiresetLow();
					Chip_TIMER_Enable(LPC_TIMER0);
				} else {
					if(countByte--) {
						countBit = 7;
						one1WiresetLow();
						Chip_TIMER_Enable(LPC_TIMER0);
					} else {
						if(NULL != waitForSendOneWire) xSemaphoreGiveFromISR( waitForSendOneWire, pdFALSE );
					}
				}
			}
		}
	}
}

uint8_t init_oneWire(void) {
	uint8_t return_value = ONEWIRE_INIT_READY;
	LPC_RITIMER_T *pRITimer;

	xWrite_oneWireQueue = xQueueCreate( 5, sizeof(onewire_struct));
	xRead_oneWireQueue = xQueueCreate( 5, sizeof(onewire_struct));
	vSemaphoreCreateBinary( waitForSendOneWire );

	Chip_TIMER_Init(LPC_TIMER0);

	NVIC_ClearPendingIRQ(TIMER0_IRQn);
	NVIC_SetPriority(TIMER0_IRQn, 8);
	NVIC_EnableIRQ(TIMER0_IRQn);

	Chip_IOCON_PinMux(LPC_IOCON, GPIO_1WIRE_OUTPUT_PORT, GPIO_1WIRE_OUTPUT_PIN,0x10,IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, GPIO_1WIRE_OUTPUT_PORT, GPIO_1WIRE_OUTPUT_PIN);
	Chip_IOCON_EnableOD(LPC_IOCON, GPIO_1WIRE_OUTPUT_PORT, GPIO_1WIRE_OUTPUT_PIN);

	Chip_IOCON_PinMux(LPC_IOCON, GPIO_1WIRE_INPUT_PORT, GPIO_1WIRE_INPUT_PIN,0x10,IOCON_FUNC0);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_1WIRE_INPUT_PORT, GPIO_1WIRE_INPUT_PIN);

	one1WiresetHigh();

	//one1SendBuffor();

	taskOneWireready = 0;

	if(NULL == xWrite_oneWireQueue || NULL == xRead_oneWireQueue || NULL == waitForSendOneWire)
	{
		return_value = ONEWIRE_INIT_QUEUE_ERROR;
	}
	else {
		if(!xTaskCreate(oneWireSendTask, NAME_oneWireSendTask, STACK_oneWireSendTask, NULL, PRIORITY_oneWireSendTask, &handleOneWireSendTask)) {
			return_value = ONEWIRE_INIT_TASK_ERROR;
		} else {

		}
	}
	return return_value;
}

void one1SendBuffor(void) {
	countBit = 0;
	Chip_TIMER_SetMatch(LPC_TIMER0, 0, TICK_1us*480);
	Chip_TIMER_MatchEnableInt(LPC_TIMER0, 0);
	Chip_TIMER_SetMatch(LPC_TIMER0, 1, TICK_1us*(480+60));
	Chip_TIMER_MatchDisableInt(LPC_TIMER0, 1);
	Chip_TIMER_SetMatch(LPC_TIMER0, 2, TICK_1us*(480+60));
	Chip_TIMER_MatchEnableInt(LPC_TIMER0, 2);
	Chip_TIMER_SetMatch(LPC_TIMER0, 3, TICK_1us*(480+480));
	Chip_TIMER_MatchEnableInt(LPC_TIMER0, 3);
	one1WiresetLow();
	Chip_TIMER_Enable(LPC_TIMER0);
}

void one1WiresetHigh(void) {
	Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_1WIRE_OUTPUT_PORT, GPIO_1WIRE_OUTPUT_PIN, true);
}

void one1WiresetLow(void) {
	Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_1WIRE_OUTPUT_PORT, GPIO_1WIRE_OUTPUT_PIN, false);
}

uint8_t onLineOneWire(void) {
	return (handleOneWireSendTask != NULL) && taskOneWireready;
}

void oneWireSendTask(void *pvParameters) {
	onewire_struct data_to_send, trash_onewire_msg;
	portBASE_TYPE xStatus;
	int tmp;

	if(uxQueueMessagesWaiting(xWrite_oneWireQueue)) {
		xQueueReset(xWrite_oneWireQueue);
	}

	if(uxQueueMessagesWaiting(xRead_oneWireQueue)) {
			xQueueReset(xRead_oneWireQueue);
		}


	taskOneWireready = 1;

	while (1) {
		xStatus = xQueueReceive(xWrite_oneWireQueue, &data_to_send, portMAX_DELAY);
		if( xStatus == pdPASS) {
			for(tmp = 1; tmp <= data_to_send.size_of_send; tmp++)
				oneWireBuffor[data_to_send.size_of_send-tmp] = data_to_send.data_to_send[tmp-1];
			countByte = data_to_send.size_of_send;
			oneWireBuffor[countByte] = 0x10;
			xSemaphoreTake( waitForSendOneWire, configTICK_RATE_HZ * 2 );
			one1SendBuffor();
			if(xSemaphoreTake( waitForSendOneWire, configTICK_RATE_HZ * 2 )) {
				for(tmp = 0; tmp <= data_to_send.size_of_send; tmp++)
					data_to_send.data_to_send[tmp] = oneWireBuffor[data_to_send.size_of_send-tmp-1];
			} else {
				configASSERT(0);
			}
			if(!xQueueSendToBack(xRead_oneWireQueue, &data_to_send, 0)) {
				if(xQueueReceive(xRead_oneWireQueue, &trash_onewire_msg, portMAX_DELAY)) {
					if(!xQueueSendToBack(xRead_oneWireQueue, &data_to_send, 0)) {
						configASSERT(0);
					}
				}
				else {
					configASSERT(0);
				}
			}
			xSemaphoreGive( waitForSendOneWire );
		} else {
			configASSERT(0);
		}
	}
}
