/*
 * kho_mbr3_inittask.h
 *
 *  Created on: 7 gru 2014
 *      Author: Krzysztof
 */

#ifndef INC_KHO_MBR3_INITTASK_H_
#define INC_KHO_MBR3_INITTASK_H_

#include "taskConfigData.h"

//TASK DEFINITION
#define PRIORITY_MBR3ButtonTask 				GLOBAL_PRIORITY_MBR3ButtonTask
#define STACK_MBR3ButtonTask					configMINIMAL_STACK_SIZE
#define NAME_MBR3ButtonTask						"MBR3ButtonTask"
static xTaskHandle handleMBR3ButtonTask		 	= NULL;
void MBR3ButtonTask(void *);
uint8_t taskMBR3ready							= 0;
//IRQ DEFINITION

#endif /* INC_KHO_MBR3_INITTASK_H_ */
