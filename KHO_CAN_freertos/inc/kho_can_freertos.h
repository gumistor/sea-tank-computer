/*
 * kho_can_freertos.h
 *
 *  Created on: 13 sie 2014
 *      Author: Krzysztof
 */

#ifndef KHO_CAN_FREERTOS_H_
#define KHO_CAN_FREERTOS_H_

#include "board.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "kho_nixie_freertos.h"

xQueueHandle xQueueCAN_RxBuffor;
xQueueHandle xQueueCANError;

void CAN_Setup(void);
void CAN_IRQHandler(void);
void PrintCANErrorInfo(uint32_t);
void vCANReceivedMsgTask(void *);
void vSendCANMessage(void *);

#endif /* KHO_CAN_FREERTOS_H_ */
