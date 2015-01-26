/*
 * kho_i2c_freertos.h
 *
 *  Created on: 1 sie 2014
 *      Author: Krzysztof
 */

#ifndef KHO_I2C_FREERTOS_H_
#define KHO_I2C_FREERTOS_H_

#include "board.h"
#include "FreeRTOS.h"
#include "queue.h"

#define I2C_INIT_READY	1
#define I2C_INIT_ERROR	0

#define SPEED_50KHZ        50000
#define SPEED_100KHZ        100000
#define SPEED_400KHZ        400000

#define MAX_I2C_DATA_TO_SEND	15
#define MAX_I2C_DATA_RECEIVED	15

#define I2C_SPEED			SPEED_50KHZ

#define I2C_ID				I2C2

#define I2C_READ			1
#define I2C_WRITE			0

xQueueHandle xWrite_I2CQueue;
xQueueHandle xRead_I2CQueue;
xQueueHandle xError_I2CQueue;

typedef struct {
	uint8_t id;
	uint8_t data_to_send[MAX_I2C_DATA_TO_SEND];
	uint8_t data_received[MAX_I2C_DATA_RECEIVED];
	uint8_t read_notwrite;
	uint8_t size_of_send;
	uint8_t size_of_received;
} i2c_struct;

uint8_t init_I2C(void);
uint8_t onLineI2C(void);
void I2C2_IRQHandler(void);

#endif /* KHO_I2C_FREERTOS_H_ */
