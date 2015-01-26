/*
 * @brief FreeRTOS Blinky example
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2014
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "kho_i2c_freertos.h"
#include "kho_spi_freertos.h"
#include "kho_mbr3_freertos.h"
#include "kho_gpio_freertos.h"
//#include "kho_pca9685_freertos.h"
//#include "kho_ds1340_freertos.h"
#include "kho_rtc_freertos.h"
#include "kho_hih6131_freertos.h"
#include "kho_hp02s_freertos.h"
#include "kho_LED_schedule.h"
#include "kho_CAN_freertos.h"
#include "kho_lcd_ea_freertos.h"
#include "kho_pwm_freertos.h"
#include "kho_wwt_freertos.h"
#include "aqua_main.h"
#include "taskConfigData.h"
#include "kho_scheduler.h"
#include "kho_aquaLCD_Data.h"
#include "kho_1wire_freertos.h"
#include "kho_ds18b20_freertos.h"
#include <stdlib.h>

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

#define AQUA_LCD_BACKLIGHT_KEEP_ON	-1

int8_t aquaLCDBacklightTimer = 20;

#define MBR3_IRQ_PIN	30
#define MBR3_IRQ_PORT	0

#define LCD_PIN			29
#define LCD_PORT		0

#define PRIORITY_vSystemInitTask			GLOBAL_PRIORITY_vSystemInitTask
#define STACK_vSystemInitTask 				0x500
#define NAME_vSystemInitTask 				"vSystemInitTask"
static xTaskHandle *handlevSystemInitTask	 = NULL;

xTaskHandle vLEDSchedulerTaskHandle;

SemaphoreHandle_t xRunDisplay = NULL;



/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

void vAssertCalled(int status, char* asFILE, int asLINE)
{
	char outLine[50];
	int x,y,i;
	if(!status) {
		taskDISABLE_INTERRUPTS();
		x = 1; y = 1; i = 0;
		sprintf(outLine,"%s:%d",asFILE,asLINE);
		while(outLine[i] != 0 && i < 50) {
			lcdPutCh(outLine[i++],x,y,LCD_MEDIUM, LCD_WHITE, LCD_BLACK);
			if(x < 120) x = x + 8;
			else {
				x = 1;
				y = y + 8;
			}
		}
		for( ;; );
	}
}

void HardFault_Handler(void)
{
	configASSERT(0);
	while(1) {}
}

/* Sets up system hardware */
static void prvSetupHardware(void)
{
	//uint32_t priority_grouping;

	//System Core Clock Update
	SystemCoreClockUpdate();
	//Board init - developement board
	Chip_GPIO_Init(LPC_GPIO2);
	Chip_IOCON_Init(LPC_IOCON);
	//Chip_GPIO_SetPinDIROutput(LPC_GPIO, 2, 13);

	Board_Init();
	//DAC init
	Chip_DAC_Init(LPC_DAC);
	Chip_Clock_SetPCLKDiv(SYSCTL_PCLK_DAC, SYSCTL_CLKDIV_1);

}

void doEverySecond(RTC_TIME_T CurrTime) {
	RTC_TIME_T FullTime;

	Chip_RTC_GetFullAlarmTime(LPC_RTC,&FullTime);
	CurrTime = *returnDateTime();
	if(compareFullTime(CurrTime, FullTime)) {
		UpdateScheduler_DateTime(returnDateTime());
		LPC_RTCsetDateTimeAlarm(&aquaSchedule->nextItem->AlarmFullTime);
	}
	if(NULL != xRunDisplay)	xSemaphoreGive( xRunDisplay );
	//HIHTaskRequestMeasurement();
}

void doEveryAlarm(RTC_TIME_T CurrTime) {
	volatile scheduleItemType eventDescriptor;
	eventDescriptor = aquaSchedule->nextItem->actionType;
		if(aquaSchedule->nextItem != NULL && aquaSchedule != NULL) {
			UpdateScheduler_DateTime(returnDateTime());
			LPC_RTCsetDateTimeAlarm(&aquaSchedule->nextItem->AlarmFullTime);
		}

		switch(eventDescriptor) {
		case itemStartLED:
			break;
		case itemReadTemperatureInternal:
			HIHTaskRequestMeasurement();
			break;
		case itemReadTemperatureExternal:
			DS18B20RequestMeasurement(EXTR_SENSOR);
			break;
		case itemReadTemperatureTank:
			DS18B20RequestMeasurement(TANK_SENSOR);
			break;
		case itemReadPressure:
			HPRequestMeasurement();
			break;
		case itemReadHumidity:
			HIHTaskRequestMeasurement();
			break;
		default:
			break;
		}
}

static void vMainStateEngine(void *pvParameters) {
	portBASE_TYPE xStatus;
	ButtonPressedStruct buttonStatus;
	uint8_t onTimer = 60;

	while (1) {
		if(onTimer) xStatus = xQueueReceive(xButtonPressedQueue, &buttonStatus, configTICK_RATE_HZ);
		else xStatus = xQueueReceive(xButtonPressedQueue, &buttonStatus, portMAX_DELAY);//configTICK_RATE_HZ * 10);
		if( xStatus == pdPASS ) {
			aquaLcdBacklightStatus = AQUA_LCD_BACKLIGHT_ON;
			onTimer = aquaLCDBacklightTimer;
			switch(aquaNixieShow) {
			case mSM_None:
				if(buttonStatus.buttonStatus == BUTTON_LEFT) aquaNixieShow = mSM_Test;
				if(buttonStatus.buttonStatus == BUTTON_RIGHT) aquaNixieShow = mSM_Date;
				break;
			case mSM_Date:
				if(buttonStatus.buttonStatus == BUTTON_LEFT) aquaNixieShow = mSM_None;
				if(buttonStatus.buttonStatus == BUTTON_RIGHT) aquaNixieShow = mSM_Time;
				break;
			case mSM_Time:
				if(buttonStatus.buttonStatus == BUTTON_LEFT) aquaNixieShow = mSM_Date;
				if(buttonStatus.buttonStatus == BUTTON_RIGHT) aquaNixieShow = mSM_Humid;
				break;
			case mSM_Humid:
				if(buttonStatus.buttonStatus == BUTTON_LEFT) aquaNixieShow = mSM_Time;
				if(buttonStatus.buttonStatus == BUTTON_RIGHT) aquaNixieShow = mSM_Temper;
				break;
			case mSM_Temper:
				if(buttonStatus.buttonStatus == BUTTON_LEFT) aquaNixieShow = mSM_Humid;
				if(buttonStatus.buttonStatus == BUTTON_RIGHT) aquaNixieShow = mSM_TemperTank;
				break;
			case mSM_TemperTank:
				if(buttonStatus.buttonStatus == BUTTON_LEFT) aquaNixieShow = mSM_Temper;
				if(buttonStatus.buttonStatus == BUTTON_RIGHT) aquaNixieShow = mSM_TemperExt;
				break;
			case mSM_TemperExt:
				if(buttonStatus.buttonStatus == BUTTON_LEFT) aquaNixieShow = mSM_TemperTank;
				if(buttonStatus.buttonStatus == BUTTON_RIGHT) aquaNixieShow = mSM_Press;
				break;
			case mSM_Press:
				if(buttonStatus.buttonStatus == BUTTON_LEFT) aquaNixieShow = mSM_TemperExt;
				if(buttonStatus.buttonStatus == BUTTON_RIGHT) aquaNixieShow = mSM_LED0;
				break;
			case mSM_LED0:
				if(buttonStatus.buttonStatus == BUTTON_LEFT) aquaNixieShow = mSM_Press;
				if(buttonStatus.buttonStatus == BUTTON_RIGHT) aquaNixieShow = mSM_LED1;
				//if(buttonStatus.buttonStatus == BUTTON_UP) ChangePWMBy(0, 1);
				//if(buttonStatus.buttonStatus == BUTTON_DOWN) ChangePWMBy(0, -1);
				break;
			case mSM_LED1:
				if(buttonStatus.buttonStatus == BUTTON_LEFT) aquaNixieShow = mSM_LED0;
				if(buttonStatus.buttonStatus == BUTTON_RIGHT) aquaNixieShow = mSM_LED2;
				//if(buttonStatus.buttonStatus == BUTTON_UP) ChangePWMBy(1, 1);
				//if(buttonStatus.buttonStatus == BUTTON_DOWN) ChangePWMBy(1, -1);
				break;
			case mSM_LED2:
				if(buttonStatus.buttonStatus == BUTTON_LEFT) aquaNixieShow = mSM_LED1;
				if(buttonStatus.buttonStatus == BUTTON_RIGHT) aquaNixieShow = mSM_LED3;
				//if(buttonStatus.buttonStatus == BUTTON_UP) ChangePWMBy(2, 1);
				//if(buttonStatus.buttonStatus == BUTTON_DOWN) ChangePWMBy(2, -1);
				break;
			case mSM_LED3:
				if(buttonStatus.buttonStatus == BUTTON_LEFT) aquaNixieShow = mSM_LED2;
				if(buttonStatus.buttonStatus == BUTTON_RIGHT) aquaNixieShow = mSM_Test;
				//if(buttonStatus.buttonStatus == BUTTON_UP) ChangePWMBy(3, 1);
				//if(buttonStatus.buttonStatus == BUTTON_DOWN) ChangePWMBy(3, -1);
				break;
			case mSM_Test:
				if(buttonStatus.buttonStatus == BUTTON_LEFT) aquaNixieShow = mSM_LED3;
				if(buttonStatus.buttonStatus == BUTTON_RIGHT) aquaNixieShow = mSM_None;
				break;
			default:
				break;
			}
		}
		else {
			if(aquaLCDBacklightTimer != AQUA_LCD_BACKLIGHT_KEEP_ON) {
				onTimer--;
				if(!onTimer) {
					//aquaNixieShow = mSM_Time;
					aquaLcdBacklightStatus = AQUA_LCD_BACKLIGHT_OFF;
				}
			} else {
				aquaLcdBacklightStatus = AQUA_LCD_BACKLIGHT_ON;
			}
		}
		if(NULL != xRunDisplay)	xSemaphoreGive( xRunDisplay );
	}
}

static void vLEDSchedulerTask(void *pvParameters) {
	static uint16_t step;
	static uint16_t old_step;
	uint8_t hour;
	uint8_t minute;
	uint8_t aquaLEDindex;

	old_step = 0;
	while (1) {
		step = 0;
		minute = ((returnTime_bcd() >> 8) & 0xFF);
		hour = ((returnTime_bcd() >> 16) & 0xFF);

		step = 60*((10*((hour>>4)&0x0F)+(hour&0x0F))-5)-40;
		step = step+10*((minute>>4)&0x0F) + (minute&0x0F);

		if(step >= 0 && step < 961) {
			while(old_step < step) {
				for(aquaLEDindex = 0; aquaLEDindex < AQUALED_NO; aquaLEDindex++) {
					if(LED_schedule[aquaLEDindex][old_step] != 0) {
						aquaLED[aquaLEDindex] = aquaLED[aquaLEDindex] + LED_schedule[aquaLEDindex][old_step];
						if(aquaLED[aquaLEDindex] < 0) aquaLED[aquaLEDindex] = 0;
						if(aquaLED[aquaLEDindex] > 1000 - aquaLEDxxPedestal) aquaLED[aquaLEDindex] = 1000 - aquaLEDxxPedestal;
						sprintf((char*)aquaLED_str[aquaLEDindex],"%3d.%1d",aquaLED[aquaLEDindex]/10,aquaLED[aquaLEDindex]%10);
						setPWM(aquaLEDindex+1, aquaLED[aquaLEDindex]);
					}
				}
				vTaskDelay(configTICK_RATE_HZ / 100);
				old_step ++;
			}
		}
		else {
			step = 0;
			aquaLED01 = 0;
			aquaLED02 = 0;
			aquaLED03 = 0;
			aquaLED04 = 0;

			if(old_step != 0) {
				setPWM1(aquaLED01);
				setPWM2(aquaLED02);
				setPWM3(aquaLED03);
				setPWM4(aquaLED04);
				old_step = 0;
			}
			//SetAlarmTime(0x05,0x40,0x00,0xFF,0xFF,0xFF);
			//vTaskSuspend(NULL);
		}

		old_step = step;

		vTaskDelay(configTICK_RATE_HZ * 60);
	}
}

uint8_t init_Status(void) {
	//power status of the LCD
	aquaLcdStatus = AQUA_LCD_OFF;
	//power status of the Nixie display
	aquaNixieStatus = AQUA_NIXIE_OFF;
	//backlight of LCD power
	aquaLcdBacklightStatus = AQUA_LCD_BACKLIGHT_ON;
	//
	aquaLCDContrast = 0x20;
	//
	aquaLCDShowPage = LCDMain_page;
	//aquaCurrentTime = 0xFFFFFFFF;
	//aquaCurrentDate = 0xFFFFFFFF;
	aquaNewTime = 0x00000000;
	aquaNewDate = 0x00000000;

	aquaLED01 = 0x0000;
	aquaLED02 = 0x0000;
	aquaLED03 = 0x0000;
	aquaLED04 = 0x0000;
	aquaLEDxxPedestal = 300;

	//aquaPressureRoom = 0x0000;

	aquaNixieShow = mSM_Time;
	return VARIABLE_INIT_READY;
}

static void vSystemUpdateOutputsTask(void *pvParameters) {
	//old values to check changes
	static uint8_t aquaLcdStatus_old = AQUA_LCD_ON;
	static uint8_t aquaNixieStatus_old = AQUA_NIXIE_ON;
	static uint8_t aquaLcdBacklightStatus_old = AQUA_LCD_BACKLIGHT_OFF;
	static uint8_t aquaLCDContrast_old = 0x10;
	static uint8_t aquaLCDShowPage_old = LCDInit_page;

	//local variables
	static uint8_t aquaLCDTimerFlasher = 0;

	uint8_t aquaNixieShow_old = 0x00;

	while (1) {
		//LCD Section
		if(aquaLcdStatus != aquaLcdStatus_old) {
			aquaLcdStatus_old = aquaLcdStatus;
		}
		if(aquaNixieStatus != aquaNixieStatus_old) {
			aquaNixieStatus_old = aquaNixieStatus;
			//change nixie status
		}
		if(aquaLcdBacklightStatus != aquaLcdBacklightStatus_old) {
			aquaLcdBacklightStatus_old = aquaLcdBacklightStatus;
			if(aquaLcdBacklightStatus) lcdBacklightOn();
			else lcdBacklightOff();
		}
		if(aquaLCDContrast != aquaLCDContrast_old) {
			aquaLCDContrast_old = aquaLCDContrast;
			//change contrast
			lcdContrast(aquaLCDContrast);
		}
		if(aquaLCDShowPage != aquaLCDShowPage_old) {
			aquaLCDShowPage_old = aquaLCDShowPage;
			switch(aquaLCDShowPage) {
				case LCDInit_page:

					aquaLCDShowPage = LCDMain_page;
					break;
				case LCDMain_page:
					lcdClrscr();
					putDate(LCD_WHITE,aquaNixieShow,mSM_Date,returnDate_bcd(),returnDate_str(),returnDayName_str());
					putTime(LCD_CYJAN,aquaNixieShow,mSM_Time,returnTime_bcd());
					putHumidity(LCD_WHITE,aquaNixieShow,mSM_Humid,HIHReturnHumidity_bcd(),HIHReturnHumidity_str());
					putTemperRoom(LCD_YELLOW,aquaNixieShow,mSM_Temper,HIHReturnTemperature_bcd(),HIHReturnTemperature_str());
					putTemperTank(LCD_WHITE,aquaNixieShow,mSM_TemperTank,DS18B20Temperature_bcd(TANK_SENSOR),DS18B20Temperature_str(TANK_SENSOR));
					putTemperExtr(LCD_YELLOW,aquaNixieShow,mSM_TemperExt,DS18B20Temperature_bcd(EXTR_SENSOR),DS18B20Temperature_str(EXTR_SENSOR));
					putPressure(LCD_WHITE,aquaNixieShow,mSM_Press,HPReturnPressure_bcd(),HPReturnPressure_str());
					putLED(0,LCD_YELLOW,aquaNixieShow,mSM_LED0,aquaLED[0],aquaLED_str[0]);
					putLED(1,LCD_WHITE,aquaNixieShow,mSM_LED1,aquaLED[1],aquaLED_str[1]);
					putLED(2,LCD_YELLOW,aquaNixieShow,mSM_LED2,aquaLED[2],aquaLED_str[2]);
					putLED(3,LCD_WHITE,aquaNixieShow,mSM_LED3,aquaLED[3],aquaLED_str[3]);
					break;
				default:
					break;
			}
		}
		else {
			switch(aquaLCDShowPage) {
				case LCDInit_page:
					break;
				case LCDMain_page:
					putDate(LCD_WHITE,aquaNixieShow,mSM_Date,returnDate_bcd(),returnDate_str(),returnDayName_str());
					putTime(LCD_CYJAN,aquaNixieShow,mSM_Time,returnTime_bcd());
					putHumidity(LCD_WHITE,aquaNixieShow,mSM_Humid,HIHReturnHumidity_bcd(),HIHReturnHumidity_str());
					putTemperRoom(LCD_YELLOW,aquaNixieShow,mSM_Temper,HIHReturnTemperature_bcd(),HIHReturnTemperature_str());
					putTemperTank(LCD_WHITE,aquaNixieShow,mSM_TemperTank,DS18B20Temperature_bcd(TANK_SENSOR),DS18B20Temperature_str(TANK_SENSOR));
					putTemperExtr(LCD_YELLOW,aquaNixieShow,mSM_TemperExt,DS18B20Temperature_bcd(EXTR_SENSOR),DS18B20Temperature_str(EXTR_SENSOR));
					putPressure(LCD_WHITE,aquaNixieShow,mSM_Press,HPReturnPressure_bcd(),HPReturnPressure_str());
					putLED(0,LCD_YELLOW,aquaNixieShow,mSM_LED0,aquaLED[0],aquaLED_str[0]);
					putLED(1,LCD_WHITE,aquaNixieShow,mSM_LED1,aquaLED[1],aquaLED_str[1]);
					putLED(2,LCD_YELLOW,aquaNixieShow,mSM_LED2,aquaLED[2],aquaLED_str[2]);
					putLED(3,LCD_WHITE,aquaNixieShow,mSM_LED3,aquaLED[3],aquaLED_str[3]);

					switch(aquaNixieShow) {
						case mSM_Date:			SendNewNixieValue_bcd(returnDate_bcd() 						| 0x14000000); 	break;
						case mSM_Time:			SendNewNixieValue_bcd(returnTime_bcd() 						| 0x14000000); 	break;
						case mSM_Humid:			SendNewNixieValue_bcd(HIHReturnHumidity_bcd()				| 0x02000000); 	break;
						case mSM_Temper:		SendNewNixieValue_bcd(HIHReturnTemperature_bcd() 			| 0x04000000); 	break;
						case mSM_TemperTank:	SendNewNixieValue_bcd(DS18B20Temperature_bcd(TANK_SENSOR) 	| 0x04000000); 	break;
						case mSM_TemperExt:		SendNewNixieValue_bcd(DS18B20Temperature_bcd(EXTR_SENSOR) 	| 0x04000000); 	break;
						case mSM_Press:			SendNewNixieValue_bcd(HPReturnPressure_bcd() 				| 0x00000000); 	break;
						case mSM_LED0:			SendNewNixieValue_bcd(aquaLED[0] 							| 0x02000000); 	break;
						case mSM_LED1:			SendNewNixieValue_bcd(aquaLED[1] 							| 0x02000000); 	break;
						case mSM_LED2:			SendNewNixieValue_bcd(aquaLED[2] 							| 0x02000000); 	break;
						case mSM_LED3:			SendNewNixieValue_bcd(aquaLED[3] 							| 0x02000000); 	break;
						default:				SendNewNixieValue_bcd(0xFFFFFFFF 							| 0x00000000);	break;
					}

					break;
				default:
					break;
			}
			aquaNixieShow_old = aquaNixieShow;
		}
		xSemaphoreTake( xRunDisplay, configTICK_RATE_HZ );
	}
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

static void vSystemInitTask(void *pvParameters) {
	aquaSchedule = NULL;

	if(VARIABLE_INIT_READY == init_Status()) {
		//init SPI interface
		if(SPI_INIT_READY == init_SPI()) {
			initLCD_EA();
			vTaskDelay(configTICK_RATE_HZ/5);
			lcdClrscr();
			lcdPutStrln("Variables OK");
			lcdPutStrln("SPI OK");
			lcdPutStrln("LCD OK");
			// GPIO section init
			if(GPIO_INIT_ERROR == init_GPIO()) {
				lcdPutStrln("GPIO NOK");
				configASSERT(0);
			} else {
				lcdPutStrln("GPIO OK");
				// MBR3 init
				if(MBR_INIT_READY == init_mbr3()) {
					lcdPutStrln("MBR OK");
					if(!xTaskCreate(vMainStateEngine, (char *) "vMainStateEngine", configMINIMAL_STACK_SIZE, NULL, PRIORITY_vMainStateEngine, (xTaskHandle *) NULL)) {
						lcdPutStrln("Buttons NOK");
						configASSERT(0);
					}
				} else {
					lcdPutStrln("MBR NOK");
					configASSERT(0);
				}
			}
			// I2C init
			if(I2C_INIT_READY == init_I2C()) {
				lcdPutStrln("I2C OK");
				// init huminity sensor
				if(HIH_INIT_READY == init_hih61()) {
					lcdPutStrln("HIH OK");
				} else {
					lcdPutStrln("HIH NOK");
					configASSERT(0);
				}
				// init pressure sensor
				if(HP_INIT_READY == init_hp02s()) {
					lcdPutStrln("HP OK");
					vTaskDelay(configTICK_RATE_HZ/10);
					HPRequestMeasurement();
					vTaskDelay(configTICK_RATE_HZ/10);
				} else {
					lcdPutStrln("HP NOK");
					configASSERT(0);
				}
			} else {
				lcdPutStrln("I2C NOK");
				configASSERT(0);
			}
			//init RTC
			if(RTC_INIT_READY == initLPC_RTC()) {
				lcdPutStrln("RTC OK");
				//init LPC PWM
				initLPC_PWM();
				lcdPutStrln("PWM OK");
				//init LED scheduler
				if(xTaskCreate(vLEDSchedulerTask, (char *) "vLEDSchedulerTask", configMINIMAL_STACK_SIZE, NULL, PRIORITY_vLEDSchedulerTask, &vLEDSchedulerTaskHandle)) {
					lcdPutStrln("LED OK");
				} else {
					lcdPutStrln("LED NOK");
					configASSERT(0);
				}
			} else {
				lcdPutStrln("RTC NOK");
				configASSERT(0);
			}
			//init 1WIRE
			if(ONEWIRE_INIT_READY == init_oneWire()) {
				lcdPutStrln("1WIRE OK");
				//init ds18b20s
				if(DS18B20_INIT_READY == init_ds18b20()) {
					lcdPutStrln("DS18B20 OK");

				} else {
					lcdPutStrln("DS18B20 NOK");
					configASSERT(0);
				}
			} else {
				lcdPutStrln("1WIRE NOK");
				configASSERT(0);
			}
			//init CAN
			//CAN_Setup();
			vTaskDelay(configTICK_RATE_HZ / 5);

			vSemaphoreCreateBinary( xRunDisplay );
			if(xTaskCreate(vSystemUpdateOutputsTask, (char *) "vSystemUpdateOutputsTask", 255, NULL, PRIORITY_vSystemUpdateOutputsTask,	(xTaskHandle *) NULL) && (NULL != xRunDisplay)) {
				lcdPutStrln("Display OK");
			} else {
				lcdPutStrln("Display NOK");
				configASSERT(0);
			}
		} else {
			configASSERT(0);
		}

			initScheduler(									0xFFFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
			addSchedulerItem(itemStartLED, 					0xFFFF, 0xFF, 0xFF, 0xFF, 0xFF,	5,		55,  0, 5);
			addSchedulerItem(itemReadTemperatureInternal, 	0xFFFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  5, 5);
			addSchedulerItem(itemReadTemperatureInternal, 	0xFFFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 35, 5);
			addSchedulerItem(itemReadTemperatureExternal, 	0xFFFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 10, 5);
			addSchedulerItem(itemReadTemperatureExternal, 	0xFFFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 40, 5);
			addSchedulerItem(itemReadHumidity, 				0xFFFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 15, 5);
			addSchedulerItem(itemReadTemperatureTank, 		0xFFFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 20, 5);
			addSchedulerItem(itemReadTemperatureTank, 		0xFFFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 50, 5);
			addSchedulerItem(itemReadPressure, 				0xFFFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 55, 5);

			sortScheduler();

			UpdateScheduler_DateTime(returnDateTime());
			LPC_RTCsetDateTimeAlarm(&aquaSchedule->nextItem->AlarmFullTime);


	} else {
		configASSERT(0);
	}


	vTaskDelay(configTICK_RATE_HZ / 5);
	//init watchdog
	initWWT();
	vTaskDelete(handlevSystemInitTask);
}

int main(void)
{
	prvSetupHardware();

	//LPC_RTCsetDateTime(2014,12,15,18,50,00);
	if(xTaskCreate(vSystemInitTask, NAME_vSystemInitTask, STACK_vSystemInitTask, NULL, PRIORITY_vSystemInitTask, &handlevSystemInitTask))
		vTaskStartScheduler();
	else
		configASSERT(0);
	/* Should never arrive here */
	return 1;
}
