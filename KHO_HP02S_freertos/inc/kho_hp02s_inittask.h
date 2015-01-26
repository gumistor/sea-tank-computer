/*
 * kho_hp02s_inittask.h
 *
 *  Created on: 6 gru 2014
 *      Author: Krzysztof
 */

#ifndef KHO_HP02S_INITTASK_H_
#define KHO_HP02S_INITTASK_H_

#include "taskConfigData.h"

//TASK DEFINITION
#define PRIORITY_HPTaskCalculate 				GLOBAL_PRIORITY_HPTaskCalculate
#define STACK_HPTaskCalculate					configMINIMAL_STACK_SIZE
#define NAME_HPTaskCalculate					"HPTaskCalc"
static xTaskHandle handleHPTaskCalculate	 	= NULL;
void HPTaskCalculate(void *);
uint8_t taskHPready							= 0;
//IRQ DEFINITION


#endif /* KHO_HP02S_INITTASK_H_ */
