/*
 * kho_ds18b20_taskinit.h
 *
 *  Created on: 15 gru 2014
 *      Author: Krzysztof
 */

#ifndef INC_KHO_DS18B20_TASKINIT_H_
#define INC_KHO_DS18B20_TASKINIT_H_

#include "taskConfigData.h"

//TASK DEFINITION
#define PRIORITY_DS18B20TaskCalculate 							GLOBAL_PRIORITY_DS18B20TaskCalculate
#define STACK_DS18B20TaskCalculate  							configMINIMAL_STACK_SIZE
#define NAME_DS18B20TaskCalculate 								"DS18B20TaskCalculat"
static xTaskHandle handleDS18B20TaskCalculate	 				= NULL;
void DS18B20TaskCalculate(void *);
uint8_t taskDS18B20ready									= 0;
//IRQ DEFINITION
//#define IRQ_PRIORITY_I2C				GLOBAL_IRQ_PRIORITY_I2C

#endif /* INC_KHO_DS18B20_TASKINIT_H_ */
