/*
 * kho_mbr3_freertos.h
 *
 *  Created on: 1 sie 2014
 *      Author: Krzysztof
 */

#ifndef KHO_MBR3_FREERTOS_H_
#define KHO_MBR3_FREERTOS_H_

#include "board.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "kho_i2c_freertos.h"

#define MBR_INIT_READY	1
#define MBR_INIT_ERROR	0

#define MBR3_INIT_ERROR_FREE		0x00;
#define MBR3_INIT_ERROR_TASK		0x01;
#define MBR3_INIT_ERROR_QUEUE		0x02;
#define MBR3_INIT_ERROR_RECEIVE		0x03;
#define MBR3_INIT_ERROR_SEND		0x04;

static uint8_t mbr3errorCode = MBR3_INIT_ERROR_FREE;

#define MBR3_I2C_ADDRESS		0x37

#define GPO0_REGISTER			0x80
#define GPO0_OUT_BIT			0x01
#define GPO2_REGISTER			0x80 //cs10
#define GPO2_OUT_BIT			0x04 //cs10
#define GPO3_REGISTER			0x80
#define GPO3_OUT_BIT			0x08
#define GPO4_REGISTER			0x80
#define GPO4_OUT_BIT			0x10
#define GPO5_REGISTER			0x80
#define GPO5_OUT_BIT			0x20
#define GPO6_REGISTER			0x80
#define GPO6_OUT_BIT			0x40

#define MBR3_I2C_BUTTON_STATUS	0xAA
#define CS0_OUTPUT_BIT			0b00000001
#define CS3_OUTPUT_BIT			0b00001000
#define CS4_OUTPUT_BIT			0b00010000
#define CS5_OUTPUT_BIT			0b00100000
#define CS6_OUTPUT_BIT			0b01000000

#define BUTTON_UP			0x08
#define BUTTON_DOWN			0x04
#define BUTTON_LEFT			0x02
#define BUTTON_RIGHT		0x01
#define PROXIMITY			0x10
#define BUTTON_NO			0x00

typedef struct {
	uint8_t buttonStatus;
} ButtonPressedStruct;

xQueueHandle xButtonPressedQueue;

uint8_t init_mbr3(void);
void setMBR3output(uint8_t, uint8_t, uint8_t);
void getMBR3input(void);
uint8_t mbr3OnLine(void);
uint8_t errorMBR3(void);

#endif /* KHO_MBR3_FREERTOS_H_ */
