/*
 * kho_gpio_freertos.c
 *
 *  Created on: 2 sie 2014
 *      Author: Krzysztof
 */

#include "kho_gpio_freertos.h"
#include "taskConfigData.h"
#include "kho_gpio_inittask.h"

void GPIO_Init_port(uint8_t int_port, uint8_t pin)
{
	/* Configure GPIO interrupt pin as input */
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, int_port, pin);
	/* Configure the GPIO interrupt */
	Chip_GPIOINT_SetIntFalling(LPC_GPIOINT, int_port, 1 << pin);
}

uint8_t init_GPIO(void)
{
	uint8_t return_value = GPIO_INIT_READY;
	// PIN30 PORT 0
	GPIO_Init_port(GPIO_INTERRUPT_PORT, GPIO_INTERRUPT_PIN);
	/* Enable interrupt in the NVIC */
	NVIC_ClearPendingIRQ(GPIO_INTERRUPT_NVIC_NAME);
	NVIC_SetPriority(GPIO_INTERRUPT_NVIC_NAME, IRQ_PRIORITY_GPIO);

	xIRQ_GPIOQueue = xQueueCreate( 5, sizeof(uint32_t));
	if(NULL != xIRQ_GPIOQueue)
	{
		if(xTaskCreate(GPIOIRQTask, NAME_GPIOIRQTask, STACK_GPIOIRQTask, NULL, PRIORITY_GPIOIRQTask,	&handleGPIOIRQTask)) {
			NVIC_EnableIRQ(GPIO_INTERRUPT_NVIC_NAME);
		} else {
			//ERROR Task not created
			return_value = GPIO_INIT_ERROR;
		}
	} else {
		//ERROR Queue not created
		return_value = GPIO_INIT_ERROR;
	}
	return return_value;
}

void GPIO_IRQ_HANDLER(void)
{
	uint32_t return_value = 0b1000000000000000000000000000000;

	return_value = Chip_GPIOINT_GetStatusFalling(LPC_GPIOINT, GPIO_INTERRUPT_PORT);
	Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT, GPIO_INTERRUPT_PORT, 1 << GPIO_INTERRUPT_PIN);

	xQueueSendToBackFromISR(xIRQ_GPIOQueue, &return_value , 0);
}

uint8_t onLineGPIO(void) {
	return (handleGPIOIRQTask != NULL) && taskGPIOready;
}

void GPIOIRQTask(void *pvParameters) {
	uint32_t irq_status;

	if(uxQueueMessagesWaiting(xIRQ_GPIOQueue)) {
		xQueueReset(xIRQ_GPIOQueue);
	}

	taskGPIOready = 1;

	while (1) {
		if(pdPASS == xQueueReceive(xIRQ_GPIOQueue, &irq_status, portMAX_DELAY))
		{
			if((irq_status >> GPIO_INTERRUPT_PIN) & 0x0001) getMBR3input();
		}
		else
		{
			configASSERT(0);
		}
	}
}

