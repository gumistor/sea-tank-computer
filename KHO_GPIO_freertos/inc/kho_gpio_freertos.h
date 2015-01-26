/*
0 * kho_gpio_freertos.h
 *
 *  Created on: 2 sie 2014
 *      Author: Krzysztof
 */

#ifndef KHO_GPIO_FREERTOS_H_
#define KHO_GPIO_FREERTOS_H_

#include "board.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "kho_mbr3_freertos.h"

#define GPIO_INIT_READY	1
#define GPIO_INIT_ERROR	0

#define GPIO_IRQ_HANDLER  			EINT3_IRQHandler/* GPIO interrupt IRQ function name */
#define GPIO_INTERRUPT_NVIC_NAME    EINT3_IRQn	/* GPIO interrupt NVIC interrupt name */


#define GPIO_INTERRUPT_PIN     		25	/* GPIO pin number mapped to interrupt */
#define GPIO_INTERRUPT_PORT    		GPIOINT_PORT0	/* GPIO port number mapped to interrupt */

void GPIO_Init_port(uint8_t, uint8_t);
uint8_t init_GPIO(void);
uint8_t onLineGPIO(void);

xQueueHandle xIRQ_GPIOQueue;

#endif /* KHO_GPIO_FREERTOS_H_ */
