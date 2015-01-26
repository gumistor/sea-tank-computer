/*
 * aqua_main.h
 *
 *  Created on: 6 lis 2014
 *      Author: Krzysztof
 */

#ifndef AQUA_MAIN_H_
#define AQUA_MAIN_H_

#define VARIABLE_INIT_READY		1
#define VARIABLE_INIT_ERROR		0


//
// LCD
//

#define BACK_GROUND_DEFAULT		LCD_BLACK
#define BACK_GROUND_SELECTED	LCD_GREEN

#define AQUA_LCD_ON		1
#define AQUA_LCD_OFF	0

static volatile uint8_t aquaLcdStatus;

#define AQUA_LCD_BACKLIGHT_ON		1
#define AQUA_LCD_BACKLIGHT_OFF		0

static volatile uint8_t aquaLcdBacklightStatus;

static volatile uint8_t aquaLCDContrast;

static volatile enum aquaLCDShowPage_enum
{
	LCDInit_page, LCDMain_page
} aquaLCDShowPage;

//
// NIXIE
//

#define AQUA_NIXIE_ON	1
#define AQUA_NIXIE_OFF	0

static volatile uint8_t aquaNixieStatus;

//
// LED Status
//
#define AQUALED_NO	4

static volatile int16_t aquaLED[AQUALED_NO] = {0x0000, 0x0000, 0x0000, 0x0000};
static uint8_t aquaLED_str[AQUALED_NO][6] = {"  0.0","  0.0","  0.0","  0.0"};

static volatile int16_t	aquaLED01 = 0x0000;
static uint8_t aquaLED01_str[6] = "  0.0";
static volatile int16_t	aquaLED02 = 0x0000;
static uint8_t aquaLED02_str[6] = "  0.0";
static volatile int16_t	aquaLED03 = 0x0000;
static uint8_t aquaLED03_str[6] = "  0.0";
static volatile int16_t	aquaLED04 = 0x0000;
static uint8_t aquaLED04_str[6] = "  0.0";

static volatile uint16_t aquaLEDxxPedestal = 0x0000;

//
// Nixie status
//

typedef enum aquaNixieShow_enum
{
	mSM_None, mSM_Time,  mSM_Date,  mSM_LED0,  mSM_LED1,  mSM_LED2,  mSM_LED3,  mSM_Humid,  mSM_Temper, mSM_TemperExt, mSM_TemperTank, mSM_Press,  mSM_Test
} aquaNixieShowType;

static aquaNixieShowType aquaNixieShow = mSM_Time;

void doEverySecond(RTC_TIME_T CurrTime);
void doEveryAlarm(RTC_TIME_T CurrTime);

#endif /* AQUA_MAIN_H_ */
