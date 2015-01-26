/*
 * kho_hih6131_inittask.h
 *
 *  Created on: 6 gru 2014
 *      Author: Krzysztof
 */

#ifndef KHO_HIH6131_INITTASK_H_
#define KHO_HIH6131_INITTASK_H_

#include "taskConfigData.h"

//TASK DEFINITION
#define PRIORITY_HIHTaskCalculate 				GLOBAL_PRIORITY_HIHTaskCalculate
#define STACK_HIHTaskCalculate					configMINIMAL_STACK_SIZE
#define NAME_HIHTaskCalculate					"HIHTaskCalc"
static xTaskHandle handleHIHTaskCalculate	 	= NULL;
void HIHTaskCalculate(void *);
uint8_t taskHIHready							= 0;
//IRQ DEFINITION


#endif /* KHO_HIH6131_INITTASK_H_ */
