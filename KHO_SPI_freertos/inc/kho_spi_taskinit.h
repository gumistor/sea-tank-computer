/*
 * kho_spi_taskinit.h
 *
 *  Created on: 6 gru 2014
 *      Author: Krzysztof
 */

#ifndef KHO_SPI_TASKINIT_H_
#define KHO_SPI_TASKINIT_H_

#include "taskConfigData.h"

//TASK DEFINITION
#define PRIORITY_SPISendTask 					GLOBAL_PRIORITY_SPISendTask
#define STACK_SPISendTask 						configMINIMAL_STACK_SIZE
#define NAME_SPISendTask 	 					"SPITaskSend"
static xTaskHandle handleSPISendTask			= NULL;
void SPISendTask(void *);
uint8_t taskSPIready							= 0;
#endif /* KHO_SPI_TASKINIT_H_ */
