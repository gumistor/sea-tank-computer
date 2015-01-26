/*
 * kho_spi_freertos.c
 *
 *  Created on: 31 sie 2014
 *      Author: Krzysztof
 */

#include "kho_spi_freertos.h"
#include "kho_spi_taskinit.h"

void SPI_AssertSSEL(void)
{
	Chip_GPIO_WritePortBit(LPC_GPIO, 0, SPI_PIN_SSEL, false);
}

void SPI_DeassertSSEL(void)
{
	Chip_GPIO_WritePortBit(LPC_GPIO, 0, SPI_PIN_SSEL, true);
}

uint8_t init_SPI(void) {
	static SSP_ConfigFormat ssp_format;
	uint8_t return_value = SPI_INIT_READY;

	Chip_IOCON_PinMux(LPC_IOCON, 0, SPI_PIN_CLK, IOCON_MODE_INACT, IOCON_FUNC2);
	Chip_IOCON_PinMux(LPC_IOCON, 0, SPI_PIN_SSEL, IOCON_MODE_INACT, IOCON_FUNC2);
	Chip_IOCON_PinMux(LPC_IOCON, 0, SPI_PIN_MISO, IOCON_MODE_INACT, IOCON_FUNC2);
	Chip_IOCON_PinMux(LPC_IOCON, 0, SPI_PIN_MOSI, IOCON_MODE_INACT, IOCON_FUNC2);

	Chip_SSP_Init(LPC_SSP1);
	ssp_format.frameFormat = SSP_FRAMEFORMAT_SPI;
	ssp_format.bits = SSP_BITS_9;
	ssp_format.clockMode = SSP_CLOCK_MODE0;
	Chip_SSP_SetClockRate(LPC_SSP1, 0, 10);
	Chip_SSP_SetFormat(LPC_SSP1, ssp_format.bits, ssp_format.frameFormat, ssp_format.clockMode);
	Chip_SSP_Enable(LPC_SSP1);
	Chip_SSP_SetMaster(LPC_SSP1, 1);

	xWrite_SPIQueue = xQueueCreate( 5, sizeof(spi_struct));
	xRead_SPIQueue = xQueueCreate( 5, sizeof(spi_struct));
	taskSPIready = 0;

	if(NULL == xWrite_SPIQueue || NULL == xRead_SPIQueue) {
		return_value = SPI_INIT_ERROR;
		configASSERT(0);
	}
	else {
		if(!xTaskCreate(SPISendTask,NAME_SPISendTask,STACK_SPISendTask,NULL,PRIORITY_SPISendTask,&handleSPISendTask)) {
			return_value = SPI_INIT_ERROR;
			configASSERT(0);
		}
	}
	return return_value;
}

uint8_t onLineSPI(void) {
	return (NULL != handleSPISendTask) && taskSPIready;
}

void SPISendTask(void *pvParameters) {
	spi_struct data_to_send;
	portBASE_TYPE xStatus;

	if(uxQueueMessagesWaiting(xWrite_SPIQueue)) {
		xQueueReset(xWrite_SPIQueue);
	}

	taskSPIready = 1;

	while (1) {
		xStatus = xQueueReceive(xWrite_SPIQueue, &data_to_send, portMAX_DELAY);
		if( xStatus == pdPASS) {
			Chip_SSP_WriteFrames_Blocking(LPC_SSP1, data_to_send.data_to_send, data_to_send.size_of_send);
		}
		else {
			configASSERT(0);
		}
	}
}
