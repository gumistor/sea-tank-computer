/*
 * taskConfigData.h
 *
 *  Created on: 6 gru 2014
 *      Author: Krzysztof
 */
#include "task.h"

#ifndef TASKCONFIGDATA_H_
#define TASKCONFIGDATA_H_

#define GLOBAL_PRIORITY_vSystemInitTask 			tskIDLE_PRIORITY + 6UL
#define GLOBAL_PRIORITY_RTCTask						tskIDLE_PRIORITY + 7UL
#define GLOBAL_PRIORITY_SPISendTask 				tskIDLE_PRIORITY + 7UL
#define GLOBAL_PRIORITY_I2CSendTask 				tskIDLE_PRIORITY + 7UL
#define GLOBAL_PRIORITY_oneWireSendTask 			tskIDLE_PRIORITY + 7UL
#define PRIORITY_vSendCANMessage			tskIDLE_PRIORITY + 6UL
#define PRIORITY_vCANReceivedMsgTask		tskIDLE_PRIORITY + 6UL
#define PRIORITY_vCANErrorDetectionTask		tskIDLE_PRIORITY + 6UL
#define GLOBAL_PRIORITY_GPIOIRQTask 				tskIDLE_PRIORITY + 5UL
#define GLOBAL_PRIORITY_MBR3ButtonTask 				tskIDLE_PRIORITY + 4UL
#define PRIORITY_vMainStateEngine			tskIDLE_PRIORITY + 4UL
#define GLOBAL_PRIORITY_HIHTaskCalculate			tskIDLE_PRIORITY + 4UL
#define GLOBAL_PRIORITY_HPTaskCalculate			tskIDLE_PRIORITY + 4UL
#define GLOBAL_PRIORITY_DS18B20TaskCalculate		tskIDLE_PRIORITY + 4UL
#define PRIORITY_vSystemUpdateOutputsTask 	tskIDLE_PRIORITY + 3UL
#define PRIORITY_vLEDSchedulerTask 			tskIDLE_PRIORITY + 2UL
#define GLOBAL_PRIORITY_WWTTask 					tskIDLE_PRIORITY + 1UL

#define STACK_vSendCANMessage				configMINIMAL_STACK_SIZE
#define STACK_vCANReceivedMsgTask			configMINIMAL_STACK_SIZE
#define STACK_vCANErrorDetectionTask		configMINIMAL_STACK_SIZE
#define STACK_vMainStateEngine				configMINIMAL_STACK_SIZE
#define STACK_vSystemUpdateOutputsTask 		configMINIMAL_STACK_SIZE
#define STACK_vLEDSchedulerTask 			configMINIMAL_STACK_SIZE

static volatile char namevSendCANMessage[]				= "";
static volatile char namevCANReceivedMsgTask[]			= "";
static volatile char namevCANErrorDetectionTask[]		= "";
static volatile char namevMainStateEngine[]				= "";
static volatile char namevSystemUpdateOutputsTask[]		= "";
static volatile char namevLEDSchedulerTask[]			= "";

static volatile xTaskHandle *handlevSendCANMessage					= NULL;
static volatile xTaskHandle *handlevCANReceivedMsgTask				= NULL;
static volatile xTaskHandle *handlevCANErrorDetectionTask			= NULL;
static volatile xTaskHandle *handlevMainStateEngine					= NULL;
static volatile xTaskHandle *handlevSystemUpdateOutputsTask			= NULL;
static volatile xTaskHandle *handlevLEDSchedulerTask				= NULL;

#define GLOBAL_IRQ_PRIORITY_GPIO			9
#define GLOBAL_IRQ_PRIORITY_CAN				8
#define GLOBAL_IRQ_PRIORITY_I2C				8
#define GLOBAL_IRQ_PRIORITY_RTC				7
#define GLOBAL_IRQ_PRIORITY_WWT				0


#endif /* TASKCONFIGDATA_H_ */
