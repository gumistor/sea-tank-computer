/*
 * kho_i2c_taskinit.h
 *
 *  Created on: 6 gru 2014
 *      Author: Krzysztof
 */

#ifndef KHO_I2C_TASKINIT_H_
#define KHO_I2C_TASKINIT_H_

#include "taskConfigData.h"

//TASK DEFINITION
#define PRIORITY_I2CSendTask 							GLOBAL_PRIORITY_I2CSendTask
#define STACK_I2CSendTask 								configMINIMAL_STACK_SIZE
#define NAME_I2CSendTask								"I2CTaskSend"
static xTaskHandle handleI2CSendTask	 				= NULL;
void I2CSendTask(void *);
uint8_t taskI2Cready									= 0;
//IRQ DEFINITION
#define IRQ_PRIORITY_I2C				GLOBAL_IRQ_PRIORITY_I2C

#endif /* KHO_I2C_TASKINIT_H_ */
