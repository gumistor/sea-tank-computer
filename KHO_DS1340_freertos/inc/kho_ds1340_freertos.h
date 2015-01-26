/*
 * kho_ds1340_freertos.h
 *
 *  Created on: 4 sie 2014
 *      Author: Krzysztof
 */

#ifndef KHO_DS1340_FREERTOS_H_
#define KHO_DS1340_FREERTOS_H_

#include "board.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "kho_mbr3_freertos.h"

#define RTC_I2C_ID							0x68

#define MON			1
#define TUE			2
#define WED			3
#define	THU			4
#define FRI			5
#define SAT			6
#define SUN			7

#define JAN			0x01
#define FEB			0x02
#define MAR			0x03
#define APR			0x04
#define MAY			0x05
#define JUN			0x06
#define JUL			0x07
#define AUG			0x08
#define SEP			0x09
#define OCT			0x10
#define NOV			0x11
#define DEC			0x12

xSemaphoreHandle xRTCAlarmSemaphore;

static uint8_t currentTime[3] = {0x00, 0x00, 0x00};
static uint8_t currentDate[4] = {0x00, 0x00, 0x00, 0x00};
static uint8_t alarmTime[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

void RTC_Init(void);

void RTCTask(void *);
void ReturnCurrentTime(uint8_t*, uint8_t*, uint8_t*);
void ReturnCurrentDate(uint8_t*, uint8_t*, uint8_t*, uint8_t*);

void SetAlarmTime(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);

void RTCsetDateTime(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);

#endif /* KHO_DS1340_FREERTOS_H_ */
