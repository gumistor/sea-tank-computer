/*
 * kho_rtc_inittask.h
 *
 *  Created on: 7 gru 2014
 *      Author: Krzysztof
 */

#ifndef INC_KHO_RTC_INITTASK_H_
#define INC_KHO_RTC_INITTASK_H_

#include "taskConfigData.h"

//TASK DEFINITION
#define PRIORITY_RTCTask 						GLOBAL_PRIORITY_RTCTask
#define STACK_RTCTask							configMINIMAL_STACK_SIZE
#define NAME_RTCTask							"MBR3ButtonTask"
static xTaskHandle handleRTCTask			 	= NULL;
void MBR3ButtonTask(void *);
uint8_t taskRTCready							= 0;

//IRQ DEFINITION
#define IRQ_PRIORITY_RTC						GLOBAL_IRQ_PRIORITY_RTC

#endif /* INC_KHO_RTC_INITTASK_H_ */
