/*
 * kho_1wire_inittask.h
 *
 *  Created on: 15 gru 2014
 *      Author: Krzysztof
 */

#ifndef INC_KHO_1WIRE_TASKINIT_H_
#define INC_KHO_1WIRE_TASKINIT_H_

#include "taskConfigData.h"

//TASK DEFINITION
#define PRIORITY_oneWireSendTask 							GLOBAL_PRIORITY_oneWireSendTask
#define STACK_oneWireSendTask 								configMINIMAL_STACK_SIZE
#define NAME_oneWireSendTask								"oneWireTaskSend"
static xTaskHandle handleOneWireSendTask	 				= NULL;
void oneWireSendTask(void *);
uint8_t taskOneWireready									= 0;
//IRQ DEFINITION
//#define IRQ_PRIORITY_I2C				GLOBAL_IRQ_PRIORITY_I2C

#endif /* INC_KHO_1WIRE_TASKINIT_H_ */
