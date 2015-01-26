/*
 * kho_wwt_freertos.c
 *
 *  Created on: 6 gru 2014
 *      Author: Krzysztof
 */

#include "board.h"
#include "kho_wwt_freertos.h"
#include "kho_wwt_inittask.h"

void WDT_IRQHandler(void)
{
	NVIC_DisableIRQ(WDT_IRQn);
	NVIC_ClearPendingIRQ(WDT_IRQn);
	configASSERT(0);
}

void startWWD(void) {
	//Start WatchDog
	Chip_WWDT_Start(LPC_WWDT);
}

void initWWT(void) {

	Board_LED_Set(1, false);
	if(!xTaskCreate(WWTTask, NAME_WWTTask, STACK_WWTTask, NULL, PRIORITY_WWTTask, &handleWWTTask)) {
		configASSERT(0);
	}
	else {
		Chip_WWDT_Init(LPC_WWDT);

		Chip_WWDT_SelClockSource(LPC_WWDT, WWDT_CLKSRC_RTC_CLK);
		Chip_WWDT_SetTimeOut(LPC_WWDT, Chip_Clock_GetRTCOscRate()*2); //4 second
		//Chip_WWDT_SetOption(LPC_WWDT, WWDT_WDMOD_WDRESET); //enalbe reseting
		Chip_WWDT_ClearStatusFlag(LPC_WWDT, WWDT_WDMOD_WDTOF);

		NVIC_ClearPendingIRQ(WDT_IRQn);
		NVIC_EnableIRQ(WDT_IRQn);

		//startWWD();
	}
}

void WWTTask(void *pvParameters) {
	bool LedState = true;

	while (1) {
		Board_LED_Set(0, LedState);
		LedState = (bool) !LedState;
		Chip_WWDT_Feed(LPC_WWDT);
		vTaskDelay(configTICK_RATE_HZ);
	}
}
