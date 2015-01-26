/*
 * kho_scheduler.h
 *
 *  Created on: 15 gru 2014
 *      Author: Krzysztof
 */

#ifndef INC_KHO_SCHEDULER_H_
#define INC_KHO_SCHEDULER_H_

#include "board.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "kho_rtc_freertos.h"

typedef enum scheduleItemType_enum {
	itemCurrentTime, itemStartLED, itemReadTemperatureInternal, itemReadTemperatureExternal, itemReadTemperatureTank, itemReadPressure, itemReadHumidity
} scheduleItemType;

typedef struct scheduleItem_struct {
	scheduleItemType actionType;
	struct scheduleItem_struct *nextItem;
	RTC_TIME_T AlarmFullTime;
	uint16_t lifeTime;
	uint8_t alarmMask;
} scheduleItem;

scheduleItem *aquaSchedule, *aquaScheduleTemp;

void UpdateScheduler_DateTime(RTC_TIME_T *curr);
uint8_t compareFullTime(RTC_TIME_T AlarmFullTime_1, RTC_TIME_T AlarmFullTime_2);
void updateSchedulerItems(void);
void addSchedulerItem(scheduleItemType typeOfItem, uint16_t year, uint8_t month, uint8_t dayofmonth, uint8_t dayofweek, uint16_t dayofyear, uint8_t hour, uint8_t minute, uint8_t second, uint8_t lifetime);
void initScheduler(uint16_t year, uint8_t month, uint8_t dayofmonth, uint8_t dayofweek, uint16_t dayofyear, uint8_t hour, uint8_t minute, uint8_t second);
void removeSchedulerItem(scheduleItem *itemToReMove);
void rescheduleItem(scheduleItem *itemToMove);
uint8_t compareTwoDate(RTC_TIME_T *AlarmFullTime_1, RTC_TIME_T *AlarmFullTime_2, uint8_t level);
void sortScheduler(void);

#endif /* INC_KHO_SCHEDULER_H_ */
