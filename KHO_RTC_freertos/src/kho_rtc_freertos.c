/*
 * kho_rtc_freertos.c
 *
 *  Created on: 28 lis 2014
 *      Author: Krzysztof
 */

#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "kho_rtc_freertos.h"
#include "taskConfigData.h"
#include "kho_rtc_inittask.h"

uint16_t dayOfWeek(uint16_t y, uint8_t m, uint8_t d) {
	return (d+=m<3?y--:y-2,23*m/9+d+4+y/4-y/100+y/400)%7;
}

uint16_t dayOfYear(uint16_t y, uint8_t m, uint8_t d) {
	int result = 0;
	result = d+30*(m-1);
	if(m>4) result ++;
	if(m>6) result ++;
	if(m>7) result ++;
	if(m>9) result ++;
	if((y%4 == 0) && (y%100 != 0)) result ++;
	return result;
}

void RTC_IRQHandler(void)
{
	//uint32_t sec;
	uint8_t statusSend;
	//volatile portBASE_TYPE result;
	//static signed portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	/* Toggle heart beat LED for each second field change interrupt */
	//xHigherPriorityTaskWoken = pdFALSE;
	if (Chip_RTC_GetIntPending(LPC_RTC, RTC_INT_COUNTER_INCREASE)) {
		/* Clear pending interrupt */
		Chip_RTC_ClearIntPending(LPC_RTC, RTC_INT_COUNTER_INCREASE);
		statusSend = 0x01;
	}

	//if(xHigherPriorityTaskWoken != pdFALSE)
		//taskYIELD();

	/* Check for alarm match */
	if (Chip_RTC_GetIntPending(LPC_RTC, RTC_INT_ALARM)) {
		/* Clear pending interrupt */
		Chip_RTC_ClearIntPending(LPC_RTC, RTC_INT_ALARM);
		statusSend = 0x02;
		//fAlarmTimeMatched = true;	/* set alarm handler flag */
	}
	xQueueSendFromISR(xClockTickQueue, &statusSend, pdFALSE);
}

uint8_t onLineRTC(void) {
	return (handleRTCTask != NULL) && taskRTCready;
}

uint8_t errorRTC(void) {
	return RTCerrorCode;
}

void vClockEngineTask(void *pvParameters) {
	//RTC_TIME_T FullTime;
	portBASE_TYPE xStatus;
	uint8_t data = 0x00;
	static uint8_t init_status = 0x00;
	uint16_t temp;

	if(uxQueueMessagesWaiting(xClockTickQueue)) {
		xQueueReset(xClockTickQueue);
	}

	taskRTCready = 1;

	while (1) {
		xStatus = xQueueReceive(xClockTickQueue, &data, portMAX_DELAY);
		if( xStatus == pdPASS) {
			Chip_RTC_GetFullTime(LPC_RTC, &aquaDateTime);
			aquaCurrentTime = 0x00000000 |(aquaDateTime.time[RTC_TIMETYPE_HOUR]/10)<<20 | (aquaDateTime.time[RTC_TIMETYPE_HOUR]%10)<<16 | (aquaDateTime.time[RTC_TIMETYPE_MINUTE]/10)<<12 | (aquaDateTime.time[RTC_TIMETYPE_MINUTE]%10)<<8 | (aquaDateTime.time[RTC_TIMETYPE_SECOND]/10) << 4 | aquaDateTime.time[RTC_TIMETYPE_SECOND]%10;
			aquaCurrentDate = 0x00000000 |(aquaDateTime.time[RTC_TIMETYPE_DAYOFWEEK]%10)<<24 |  (aquaDateTime.time[RTC_TIMETYPE_DAYOFMONTH]/10)<<4 | (aquaDateTime.time[RTC_TIMETYPE_DAYOFMONTH]%10) | (aquaDateTime.time[RTC_TIMETYPE_MONTH]/10)<<12 | (aquaDateTime.time[RTC_TIMETYPE_MONTH]%10)<<8 | ((aquaDateTime.time[RTC_TIMETYPE_YEAR]-2000)/10) << 20 | ((aquaDateTime.time[RTC_TIMETYPE_YEAR]-2000)%10) << 16;
			sprintf((char *)aquaCurrentDate_str,"%2d. %s., %d",aquaDateTime.time[RTC_TIMETYPE_DAYOFMONTH],monthTable[aquaDateTime.time[RTC_TIMETYPE_MONTH]-1],aquaDateTime.time[RTC_TIMETYPE_YEAR]);
			if(data == 0x01 && init_status) {
				doEverySecond(aquaDateTime);
			}
			else {
				init_status = 1;
				doEveryAlarm(aquaDateTime);
			}
		}
		else {
			RTCerrorCode = RTC_INIT_ERROR_RECEIVE;
			configASSERT(0);
		}
	}
}

uint8_t initLPC_RTC(void) {
	uint8_t return_value = RTC_INIT_READY;
	Chip_RTC_Init(LPC_RTC);
	/* Set the RTC to generate an interrupt on each second */
	Chip_RTC_CntIncrIntConfig(LPC_RTC, RTC_AMR_CIIR_IMSEC, ENABLE);
	/* Enable RTC interrupt in NVIC */
	Chip_RTC_AlarmIntConfig(LPC_RTC, RTC_AMR_CIIR_BITMASK, ENABLE);
	/* Clear interrupt pending */
	Chip_RTC_ClearIntPending(LPC_RTC, RTC_INT_COUNTER_INCREASE | RTC_INT_ALARM);


	xClockTickQueue = xQueueCreate(2, sizeof(uint8_t));

	if(xClockTickQueue) {
		if(xTaskCreate(vClockEngineTask, NAME_RTCTask,STACK_RTCTask, NULL, PRIORITY_RTCTask, &handleRTCTask)) {
			NVIC_SetPriority(RTC_IRQn, IRQ_PRIORITY_RTC);
			NVIC_EnableIRQ((IRQn_Type) RTC_IRQn);
			Chip_RTC_Enable(LPC_RTC, ENABLE);
		} else {
			return_value = RTC_INIT_ERROR;
			RTCerrorCode = RTC_INIT_ERROR_TASK;
		}

	} else {
		return_value = RTC_INIT_ERROR;
		RTCerrorCode = RTC_INIT_ERROR_QUEUE;
	}
	return return_value;
}

void LPC_RTCsetDateTime(uint16_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t minute,uint8_t second) {
	RTC_TIME_T FullTime;

	FullTime.time[RTC_TIMETYPE_SECOND]  = second;
	FullTime.time[RTC_TIMETYPE_MINUTE]  = minute;
	FullTime.time[RTC_TIMETYPE_HOUR]    = hour;
	FullTime.time[RTC_TIMETYPE_DAYOFMONTH]  = day;
	FullTime.time[RTC_TIMETYPE_DAYOFWEEK]   = dayOfWeek(year,month,day);
	FullTime.time[RTC_TIMETYPE_DAYOFYEAR]   = dayOfYear(year,month,day);
	FullTime.time[RTC_TIMETYPE_MONTH]   = month;
	FullTime.time[RTC_TIMETYPE_YEAR]    = year;
	Chip_RTC_SetFullTime(LPC_RTC, &FullTime);
}

void LPC_RTCsetDateTimeAlarm(RTC_TIME_T *curr) {
	Chip_RTC_SetFullAlarmTime(LPC_RTC, curr);
}

RTC_TIME_T *returnDateTime(void) {
	return &aquaDateTime;
}

uint32_t returnDate_bcd(void) {
	return aquaCurrentDate;
}

uint8_t *returnDate_str(void) {
	return aquaCurrentDate_str;
}

uint8_t *returnDayName_str(void) {
	return (uint8_t *)weekTable[((aquaCurrentDate & 0xF000000) >> 24)];
}

uint32_t returnTime_bcd(void) {
	return aquaCurrentTime;
}

uint8_t *returnTime_str(void) {
	return "error";
}
