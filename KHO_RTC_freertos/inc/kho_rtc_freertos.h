/*
 * kho_rtc_freertos.h
 *
 *  Created on: 28 lis 2014
 *      Author: Krzysztof
 */

#ifndef KHO_RTC_FREERTOS_H_
#define KHO_RTC_FREERTOS_H_

#define RTC_INIT_READY	1
#define RTC_INIT_ERROR	0

#define RTC_INIT_ERROR_FREE		0x00;
#define RTC_INIT_ERROR_TASK		0x01;
#define RTC_INIT_ERROR_QUEUE	0x02;
#define RTC_INIT_ERROR_RECEIVE	0x03;

//
// TIME and DATE
//

static char monthTable[12][4] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
static char weekTable[12][10] = {"Sunday   ","Monday   ","Tuesday  ","Wednesday","Thursday ","Friday   ","Saturday "};

static volatile uint32_t aquaCurrentTime = 0x00112233;
static volatile uint32_t aquaNewTime = 0x00223344;

static volatile uint32_t aquaCurrentDate = 0x00111111;
static volatile uint32_t aquaNewDate = 0x00222222;
static uint8_t aquaCurrentDate_str[15] = "DD. MMM., YYYY";

static uint8_t RTCerrorCode = RTC_INIT_ERROR_FREE;

RTC_TIME_T aquaDateTime;

xQueueHandle xClockTickQueue;

uint16_t dayOfWeek(uint16_t, uint8_t, uint8_t);
uint16_t dayOfYear(uint16_t, uint8_t, uint8_t);

void RTC_IRQHandler(void);
void vClockEngineTask(void *);
uint8_t initLPC_RTC(void);
uint8_t onLineRTC(void);
uint8_t errorRTC(void);

void LPC_RTCsetDateTime(uint16_t,uint8_t,uint8_t,uint8_t,uint8_t,uint8_t);
void LPC_RTCsetDateTimeAlarm(RTC_TIME_T *curr);

RTC_TIME_T *returnDateTime(void);
uint32_t returnDate_bcd(void);
uint8_t *returnDate_str(void);
uint8_t *returnDayName_str(void);
uint32_t returnTime_bcd(void);
uint8_t *returnTime_str(void);

#endif /* KHO_RTC_FREERTOS_H_ */
