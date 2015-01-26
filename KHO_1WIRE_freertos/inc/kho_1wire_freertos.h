/*
 * kho_1wire_freertos.h
 *
 *  Created on: 15 gru 2014
 *      Author: Krzysztof
 */

#ifndef INC_KHO_1WIRE_FREERTOS_H_
#define INC_KHO_1WIRE_FREERTOS_H_

#include "board.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
//#include "kho_1wire_freertos.h"

#define 	ONEWIRE_RESET_TIME	0xBA4
#define		ONEWIRE_LOW_TIME	0x12
#define		ONEWIRE_HIGH_START_TIME	0x01
#define		ONEWIRE_HIGH_END_TIME	0x70
#define		ONEWIRE_SEPARATOR		0x10
#define		ONE_WIRE_SAMPLE_TIME	0x05
#define		ONE_WIRE_SAMPLERESET_TIME	0x20

#define ONEWIRE_INIT_READY	0
#define ONEWIRE_INIT_QUEUE_ERROR	1
#define ONEWIRE_INIT_TASK_ERROR		2

#define MAX_ONEWIRE_DATA_TO_SEND	20
#define MAX_ONEWIRE_DATA_RECEIVED	10

#define GPIO_1WIRE_OUTPUT_PORT		0
#define GPIO_1WIRE_OUTPUT_PIN		3
#define GPIO_1WIRE_INPUT_PIN		21
#define GPIO_1WIRE_INPUT_PORT		0

#define ONEWIRE_READ			1
#define ONEWIRE_WRITE			0

xQueueHandle xWrite_oneWireQueue;
xQueueHandle xRead_oneWireQueue;
SemaphoreHandle_t waitForSendOneWire;

typedef struct {
	uint8_t id;
	uint8_t data_to_send[MAX_ONEWIRE_DATA_TO_SEND];
	//uint8_t data_received[MAX_ONEWIRE_DATA_RECEIVED];
	//uint8_t read_notwrite;
	uint8_t size_of_send;
	//uint8_t size_of_received;
} onewire_struct;

void one1WiresetLow(void);
void one1WiresetHigh(void);
//uint8_t one1WireWait(uint16_t, uint16_t);
//uint8_t one1WireRecive(void);
//uint8_t one1WiresendLow(void);
//uint8_t one1WiresendHigh(void);
void one1SendBuffor(void);

uint8_t init_oneWire(void);
uint8_t onLineOneWire(void);

#endif /* INC_KHO_1WIRE_FREERTOS_H_ */
