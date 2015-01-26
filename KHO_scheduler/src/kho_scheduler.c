/*
 * kho_scheduler.c
 *
 *  Created on: 15 gru 2014
 *      Author: Krzysztof
 */
#include "kho_scheduler.h"

uint8_t compareFullTime(RTC_TIME_T AlarmFullTime_1, RTC_TIME_T AlarmFullTime_2) {
	uint8_t return_value;

	return_value = 0;

	if(AlarmFullTime_1.time[RTC_TIMETYPE_YEAR] > AlarmFullTime_2.time[RTC_TIMETYPE_YEAR]) return_value = 1;
	else if(AlarmFullTime_1.time[RTC_TIMETYPE_YEAR] == AlarmFullTime_2.time[RTC_TIMETYPE_YEAR]) {
		if(AlarmFullTime_1.time[RTC_TIMETYPE_MONTH] > AlarmFullTime_2.time[RTC_TIMETYPE_MONTH]) return_value = 1;
		else if(AlarmFullTime_1.time[RTC_TIMETYPE_MONTH] == AlarmFullTime_2.time[RTC_TIMETYPE_MONTH]) {
			if(AlarmFullTime_1.time[RTC_TIMETYPE_DAYOFMONTH] > AlarmFullTime_2.time[RTC_TIMETYPE_DAYOFMONTH]) return_value = 1;
			else if(AlarmFullTime_1.time[RTC_TIMETYPE_DAYOFMONTH] == AlarmFullTime_2.time[RTC_TIMETYPE_DAYOFMONTH]) {
				if(AlarmFullTime_1.time[RTC_TIMETYPE_HOUR] > AlarmFullTime_2.time[RTC_TIMETYPE_HOUR]) return_value = 1;
				else if(AlarmFullTime_1.time[RTC_TIMETYPE_HOUR] == AlarmFullTime_2.time[RTC_TIMETYPE_HOUR]) {
					if(AlarmFullTime_1.time[RTC_TIMETYPE_MINUTE] > AlarmFullTime_2.time[RTC_TIMETYPE_MINUTE]) return_value = 1;
					else if(AlarmFullTime_1.time[RTC_TIMETYPE_MINUTE] == AlarmFullTime_2.time[RTC_TIMETYPE_MINUTE]) {
						if(AlarmFullTime_1.time[RTC_TIMETYPE_SECOND] > AlarmFullTime_2.time[RTC_TIMETYPE_SECOND]) return_value = 1;
					}
				}
			}
		}
	}
	return return_value;
}

void UpdateScheduler_DateTime(RTC_TIME_T *curr) {
	if(aquaSchedule && curr) {
		aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_SECOND] = curr->time[RTC_TIMETYPE_SECOND];
		aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_MINUTE] = curr->time[RTC_TIMETYPE_MINUTE];
		aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_HOUR] = curr->time[RTC_TIMETYPE_HOUR];
		aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_DAYOFMONTH] = curr->time[RTC_TIMETYPE_DAYOFMONTH];
		aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_DAYOFWEEK] = curr->time[RTC_TIMETYPE_DAYOFWEEK];
		aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_DAYOFYEAR] = curr->time[RTC_TIMETYPE_DAYOFYEAR];
		aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_MONTH] = curr->time[RTC_TIMETYPE_MONTH];
		aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_YEAR] = curr->time[RTC_TIMETYPE_YEAR];

		sortScheduler();
	}

}

void updateSchedulerItems(void) {
	scheduleItem *tempOfSchedule;

	aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_SECOND] = returnDateTime()->time[RTC_TIMETYPE_SECOND];
	aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_MINUTE] = returnDateTime()->time[RTC_TIMETYPE_MINUTE];
	aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_HOUR] = returnDateTime()->time[RTC_TIMETYPE_HOUR];
	aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_DAYOFMONTH] = returnDateTime()->time[RTC_TIMETYPE_DAYOFMONTH];
	aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_DAYOFWEEK] = returnDateTime()->time[RTC_TIMETYPE_DAYOFWEEK];
	aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_DAYOFYEAR] = returnDateTime()->time[RTC_TIMETYPE_DAYOFYEAR];
	aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_MONTH] = returnDateTime()->time[RTC_TIMETYPE_MONTH];
	aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_YEAR] = returnDateTime()->time[RTC_TIMETYPE_YEAR];
	tempOfSchedule = aquaSchedule->nextItem;
	while(tempOfSchedule != NULL) {
		if((tempOfSchedule->alarmMask) & 0x1) tempOfSchedule->AlarmFullTime.time[RTC_TIMETYPE_SECOND] = aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_SECOND];
		if((tempOfSchedule->alarmMask >> 1) & 0x1) tempOfSchedule->AlarmFullTime.time[RTC_TIMETYPE_MINUTE] = aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_MINUTE];
		if((tempOfSchedule->alarmMask >> 2) & 0x1) tempOfSchedule->AlarmFullTime.time[RTC_TIMETYPE_HOUR] = aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_HOUR];
		if((tempOfSchedule->alarmMask >> 4) & 0x1) tempOfSchedule->AlarmFullTime.time[RTC_TIMETYPE_DAYOFYEAR] = aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_DAYOFYEAR];
		if((tempOfSchedule->alarmMask >> 3) & 0x1) tempOfSchedule->AlarmFullTime.time[RTC_TIMETYPE_DAYOFMONTH] = aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_DAYOFMONTH];
		if((tempOfSchedule->alarmMask >> 5) & 0x1) tempOfSchedule->AlarmFullTime.time[RTC_TIMETYPE_DAYOFWEEK] = aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_DAYOFWEEK];
		if((tempOfSchedule->alarmMask >> 6) & 0x1) tempOfSchedule->AlarmFullTime.time[RTC_TIMETYPE_MONTH] = aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_MONTH];
		if((tempOfSchedule->alarmMask >> 7) & 0x1) tempOfSchedule->AlarmFullTime.time[RTC_TIMETYPE_YEAR] = aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_YEAR];
		tempOfSchedule = tempOfSchedule->nextItem;
	}
}

void addSchedulerItem(scheduleItemType typeOfItem, uint16_t year, uint8_t month, uint8_t dayofmonth, uint8_t dayofweek, uint16_t dayofyear, uint8_t hour, uint8_t minute, uint8_t second, uint8_t lifetime) {
	scheduleItem *tempOfSchedule, *newItemOfSchedule;

	newItemOfSchedule = malloc(sizeof(scheduleItem));
	if(newItemOfSchedule != NULL) {
		newItemOfSchedule->alarmMask = 0x00;

		if(second != 0xFF) {
			newItemOfSchedule->AlarmFullTime.time[RTC_TIMETYPE_SECOND] = second;
		}
		else {
			newItemOfSchedule->alarmMask |= 0x01;
			//newItemOfSchedule->AlarmFullTime.time[RTC_TIMETYPE_SECOND] = aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_SECOND];
		}

		if(minute != 0xFF) {
			newItemOfSchedule->AlarmFullTime.time[RTC_TIMETYPE_MINUTE] = minute;
		}
		else {
					newItemOfSchedule->alarmMask |= 0x01 << 1;
					//newItemOfSchedule->AlarmFullTime.time[RTC_TIMETYPE_MINUTE] = aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_MINUTE];
		}

		if(hour != 0xFF) {
			newItemOfSchedule->AlarmFullTime.time[RTC_TIMETYPE_HOUR] = hour;
		}
		else {
					newItemOfSchedule->alarmMask |= 0x01 << 2;
					//newItemOfSchedule->AlarmFullTime.time[RTC_TIMETYPE_HOUR] = aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_HOUR];

		}

		if(dayofmonth != 0xFF) {
			newItemOfSchedule->AlarmFullTime.time[RTC_TIMETYPE_DAYOFMONTH] = dayofmonth;
		}
		else {
					newItemOfSchedule->alarmMask |= 0x01 << 3;
					//newItemOfSchedule->AlarmFullTime.time[RTC_TIMETYPE_DAYOFMONTH] = aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_DAYOFMONTH];
		}

		if(dayofweek != 0xFF) {
			newItemOfSchedule->AlarmFullTime.time[RTC_TIMETYPE_DAYOFWEEK] = dayofweek;
		}
		else {
					newItemOfSchedule->alarmMask |= 0x01 << 5;
					//newItemOfSchedule->AlarmFullTime.time[RTC_TIMETYPE_DAYOFWEEK] = aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_DAYOFWEEK];
		}

		if(dayofyear != 0xFF)  newItemOfSchedule->AlarmFullTime.time[RTC_TIMETYPE_DAYOFYEAR] = dayofyear;
		else newItemOfSchedule->alarmMask |= 0x01 << 4;

		if(month != 0xFF) {
			newItemOfSchedule->AlarmFullTime.time[RTC_TIMETYPE_MONTH] = month;
		}
		else {
					newItemOfSchedule->alarmMask |= 0x01 << 6;
					//newItemOfSchedule->AlarmFullTime.time[RTC_TIMETYPE_MONTH] = aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_MONTH];
		}

		if(year != 0xFFFF) {
			newItemOfSchedule->AlarmFullTime.time[RTC_TIMETYPE_YEAR] = year;
		}
		else {
			newItemOfSchedule->alarmMask |= 0x01 << 7;
			//newItemOfSchedule->AlarmFullTime.time[RTC_TIMETYPE_YEAR] = aquaSchedule->AlarmFullTime.time[RTC_TIMETYPE_YEAR];
		}

		newItemOfSchedule->actionType = typeOfItem;
		newItemOfSchedule->lifeTime = lifetime;
		newItemOfSchedule->nextItem = NULL;
		if(aquaSchedule == NULL) {
			aquaSchedule = newItemOfSchedule;
		}
		else {
			tempOfSchedule = aquaSchedule;
			while(tempOfSchedule->nextItem != NULL) tempOfSchedule = tempOfSchedule->nextItem;
			tempOfSchedule->nextItem = newItemOfSchedule;
		}
	}

}

void initScheduler(uint16_t year, uint8_t month, uint8_t dayofmonth, uint8_t dayofweek, uint16_t dayofyear, uint8_t hour, uint8_t minute, uint8_t second) {
	addSchedulerItem(itemCurrentTime, 2014, 11, 24, 5, 19, 100, 56, 00, 0);
}

void removeSchedulerItem(scheduleItem *itemToReMove) {
	scheduleItem *tempOfSchedule;
	tempOfSchedule = aquaSchedule;
	while(tempOfSchedule->nextItem != itemToReMove && tempOfSchedule != NULL) tempOfSchedule = tempOfSchedule->nextItem;
	if(tempOfSchedule != NULL) {
		tempOfSchedule->nextItem = itemToReMove->nextItem;
		free(itemToReMove);
	}
}

void rescheduleItem(scheduleItem *itemToMove) {
	int8_t addNext;
	scheduleItem *tempItem, *prevTempItem;
	addNext = 0;
	if((itemToMove->alarmMask & 0b11001111) == 0x00) removeSchedulerItem(itemToMove);
	else {
		if(itemToMove->alarmMask & 0b00000010) {
			itemToMove->AlarmFullTime.time[RTC_TIMETYPE_MINUTE] ++;
			if(itemToMove->AlarmFullTime.time[RTC_TIMETYPE_MINUTE] > 59) {
				itemToMove->AlarmFullTime.time[RTC_TIMETYPE_MINUTE] = 0;
				addNext = 0;
			}
			else {
				addNext = -1;
			}
		}
		if(itemToMove->alarmMask & 0b00000100) {
			itemToMove->AlarmFullTime.time[RTC_TIMETYPE_HOUR] = addNext + itemToMove->AlarmFullTime.time[RTC_TIMETYPE_HOUR] + 1;
			if(itemToMove->AlarmFullTime.time[RTC_TIMETYPE_HOUR] > 23) {
				itemToMove->AlarmFullTime.time[RTC_TIMETYPE_HOUR] = 0;
				addNext = 0;
			}
			else {
				addNext = -1;
			}
		}
		if(itemToMove->alarmMask & 0b00001000) {
			itemToMove->AlarmFullTime.time[RTC_TIMETYPE_DAYOFMONTH] = addNext + itemToMove->AlarmFullTime.time[RTC_TIMETYPE_DAYOFMONTH] + 1;
			if((itemToMove->AlarmFullTime.time[RTC_TIMETYPE_DAYOFMONTH] > 31 && (itemToMove->AlarmFullTime.time[RTC_TIMETYPE_MONTH] == 1 || itemToMove->AlarmFullTime.time[RTC_TIMETYPE_MONTH] == 3 || itemToMove->AlarmFullTime.time[RTC_TIMETYPE_MONTH] == 5 || itemToMove->AlarmFullTime.time[RTC_TIMETYPE_MONTH] == 7 || itemToMove->AlarmFullTime.time[RTC_TIMETYPE_MONTH] == 8 || itemToMove->AlarmFullTime.time[RTC_TIMETYPE_MONTH] == 10 || itemToMove->AlarmFullTime.time[RTC_TIMETYPE_MONTH] == 12)) ||
					(itemToMove->AlarmFullTime.time[RTC_TIMETYPE_DAYOFMONTH] > 28 && (itemToMove->AlarmFullTime.time[RTC_TIMETYPE_MONTH] == 2)) ||
					itemToMove->AlarmFullTime.time[RTC_TIMETYPE_DAYOFMONTH] > 30) {
				itemToMove->AlarmFullTime.time[RTC_TIMETYPE_DAYOFMONTH] = 0;
				addNext = 0;
			}
			else {
				addNext = -1;
			}
		}
		if(itemToMove->alarmMask & 0b01000000) {
			itemToMove->AlarmFullTime.time[RTC_TIMETYPE_MONTH] = addNext + itemToMove->AlarmFullTime.time[RTC_TIMETYPE_MONTH] + 1;
			if(itemToMove->AlarmFullTime.time[RTC_TIMETYPE_MONTH] > 12) {
				itemToMove->AlarmFullTime.time[RTC_TIMETYPE_MONTH] = 0;
				addNext = 0;
			}
			else {
				addNext = -1;
			}
		}
		if(itemToMove->alarmMask & 0b10000000) {
			itemToMove->AlarmFullTime.time[RTC_TIMETYPE_YEAR] = addNext + itemToMove->AlarmFullTime.time[RTC_TIMETYPE_YEAR] + 1;
		}
	}
	tempItem = aquaSchedule;
	prevTempItem = NULL;
	while(compareFullTime(itemToMove->AlarmFullTime, tempItem->AlarmFullTime) && tempItem != NULL) {
		prevTempItem = tempItem;
		tempItem = tempItem->nextItem;
	}
	if(prevTempItem) {
		itemToMove->nextItem = tempItem;
		prevTempItem->nextItem = itemToMove;
	}

}

uint8_t compareTwoDate(RTC_TIME_T *AlarmFullTime_1, RTC_TIME_T *AlarmFullTime_2, uint8_t level) {
	uint16_t x,y;
	x = AlarmFullTime_1->time[level];
	y = AlarmFullTime_2->time[level];
	if((0xFF & x)==255) x = 0;
	if((0xFF & y)==255) y = 0;
	return x>y;
}

void sortScheduler(void) {
	scheduleItem *A, *B, *C;
	uint8_t change;//, x, y;
	uint8_t timePart;

	updateSchedulerItems();

	timePart = RTC_TIMETYPE_SECOND;
	do {
		change = 1;
		while(change) {
			change = 0;
			A = aquaSchedule;
			B = A->nextItem;
			while(B->nextItem != NULL && B != NULL) {
				C = B->nextItem;
				if(compareTwoDate(&B->AlarmFullTime, &C->AlarmFullTime, timePart)) {
						A->nextItem = C;
						B->nextItem = C->nextItem;
						C->nextItem = B;
						A = C;
				//	}
					change = 1;
				} else {
				A = B;
				B = C;
				}
			}
		}
		switch(timePart) {
		case RTC_TIMETYPE_SECOND:
			timePart = RTC_TIMETYPE_MINUTE;
			break;
		case RTC_TIMETYPE_MINUTE:
			timePart = RTC_TIMETYPE_HOUR;
			break;
		case RTC_TIMETYPE_HOUR:
			timePart = RTC_TIMETYPE_DAYOFMONTH;
			break;
		case RTC_TIMETYPE_DAYOFMONTH:
			timePart = RTC_TIMETYPE_MONTH;
			break;
		case RTC_TIMETYPE_MONTH:
			timePart = RTC_TIMETYPE_YEAR;
			break;
		case RTC_TIMETYPE_YEAR:
			timePart = RTC_TIMETYPE_SECOND;
			break;
		default: ;
		}
	}
	while(timePart != RTC_TIMETYPE_SECOND);

	B = aquaSchedule->nextItem;
	while(compareFullTime(aquaSchedule->AlarmFullTime, B->AlarmFullTime)) {
		aquaSchedule->nextItem = B->nextItem;
		rescheduleItem(B);
		B = aquaSchedule->nextItem;
	}
}
