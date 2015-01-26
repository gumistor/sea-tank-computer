/*
 * kho_spi_freertos.h
 *
 *  Created on: 31 sie 2014
 *      Author: Krzysztof
 */

#ifndef KHO_SPI_FREERTOS_H_
#define KHO_SPI_FREERTOS_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define SPI_INIT_READY	1
#define SPI_INIT_ERROR	0

xQueueHandle xWrite_SPIQueue;
xQueueHandle xRead_SPIQueue;

#define MAX_SPI_DATA_TO_SEND	0x40
#define MAX_SPI_DATA_RECEIVED	0x40

#define SPI_PIN_CLK		7//15
#define SPI_PIN_SSEL	6//16
#define	SPI_PIN_MOSI	9//18
#define SPI_PIN_MISO	8//17

typedef struct {
	uint16_t data_to_send[MAX_SPI_DATA_TO_SEND];
	uint16_t data_received[MAX_SPI_DATA_RECEIVED];
	uint8_t size_of_send;
	uint8_t size_of_received;
} spi_struct;

void SPI_AssertSSEL(void);
void SPI_DeassertSSEL(void);
uint8_t init_SPI(void);
uint8_t onLineSPI(void);

#endif /* KHO_SPI_FREERTOS_H_ */
