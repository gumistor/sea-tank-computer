/*
 * kho_gpio_inittask.h
 *
 *  Created on: 13 gru 2014
 *      Author: Krzysztof
 */

#ifndef INC_KHO_GPIO_INITTASK_H_
#define INC_KHO_GPIO_INITTASK_H_

//TASK DEFINITION
#define PRIORITY_GPIOIRQTask 				GLOBAL_PRIORITY_GPIOIRQTask
#define STACK_GPIOIRQTask					configMINIMAL_STACK_SIZE
#define NAME_GPIOIRQTask					"GPIOIRQTask"
static xTaskHandle *handleGPIOIRQTask	 	= NULL;
void GPIOIRQTask(void *);
uint8_t taskGPIOready						= 0;
//IRQ DEFINITION
#define IRQ_PRIORITY_GPIO					GLOBAL_IRQ_PRIORITY_GPIO

#endif /* INC_KHO_GPIO_INITTASK_H_ */
