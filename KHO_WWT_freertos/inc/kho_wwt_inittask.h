/*
 * kho_wwt_inittask.h
 *
 *  Created on: 6 gru 2014
 *      Author: Krzysztof
 */

#ifndef KHO_WWT_INITTASK_H_
#define KHO_WWT_INITTASK_H_

#include "freertos.h"
#include "taskConfigData.h"

//TASK DEFINITION
#define PRIORITY_WWTTask 							GLOBAL_PRIORITY_WWTTask
#define STACK_WWTTask 								configMINIMAL_STACK_SIZE
#define NAME_WWTTask								"WWTTask"
static xTaskHandle handleWWTTask	 				= NULL;
void WWTTask(void *);
uint8_t taskWWTready								= 0;
//IRQ DEFINITION
#define IRQ_PRIORITY_WWT							GLOBAL_IRQ_PRIORITY_WWT

#endif /* KHO_WWT_INITTASK_H_ */
