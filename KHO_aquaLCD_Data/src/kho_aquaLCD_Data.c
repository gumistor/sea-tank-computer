/*
 * kho_aquaLCD_Data.c
 *
 *  Created on: 15 gru 2014
 *      Author: Krzysztof
 */

#include "kho_aquaLCD_Data.h"

void putDate(uint16_t currColor, aquaNixieShowType aquaNixieShow_local, aquaNixieShowType tempStatus, uint32_t new_value, uint8_t *str1, uint8_t *str2) {
	static uint32_t value_old = 0x00000000;
	static uint8_t status = 2;
	static uint8_t selected = 0;
	static uint8_t updated = 0;
	static uint16_t backColor = LCD_BLACK;

	if(updated == 1) {
		updated = 0;
		if(!status) status = 1;
	}

	if(value_old != new_value) {
		value_old = new_value;
		updated = 1;
		if(!status) status = 1;
	}

	if(aquaNixieShow_local == tempStatus && selected == 0) {
		backColor = LCD_DARKCYJAN;
		//currColor = LCD_BLACK;
		status = 2;
		selected = 1;
	} if(aquaNixieShow_local != tempStatus && selected == 1) {
		backColor = LCD_BLACK;
		status = 2;
		selected = 0;
	} else {
		//
	}

	if(status > 0) lcdPutStrXY(str1, 1, 	XY_DATE, 		LCD_MEDIUM, currColor, backColor);

	if(updated) {
		currColor = LCD_RED;
	} else {
		//if(selected > 0) currColor = LCD_BLACK;
	}

	if(status > 0) lcdPutStrXY(str2, 1, 	XY_DATE_DAY, 	LCD_MEDIUM, currColor, backColor);

	status = 0;
}

void putHumidity(uint16_t currColor, aquaNixieShowType aquaNixieShow_local, aquaNixieShowType tempStatus, uint16_t new_value, uint8_t *str1) {
	static uint16_t value_old = 0x0000;
	static uint8_t status = 2;
	static uint8_t selected = 0;
	static uint8_t updated = 0;
	static uint16_t backColor = LCD_BLACK;
	static uint8_t str[17] = "Humidity:      %";

	if(updated == 1) {
		updated = 0;
		if(!status) status = 1;
	}

	if(value_old != new_value) {
		value_old = new_value;
		updated = 1;
		if(!status) status = 1;
	}

	if(aquaNixieShow_local == tempStatus && selected == 0) {
		backColor = LCD_DARKCYJAN;
		//currColor = LCD_BLACK;
		status = 2;
		selected = 1;
	} if(aquaNixieShow_local != tempStatus && selected == 1) {
		backColor = LCD_BLACK;
		status = 2;
		selected = 0;
	} else {
		//
	}

	if(status > 1) lcdPutStrXY(str, 					 1,			XY_HUMID, LCD_MEDIUM, currColor, backColor);

	if(updated) {
		currColor = LCD_RED;
	} else {
		//if(selected > 0) currColor = LCD_BLACK;
	}

	if(status > 0) lcdPutStrXY(str1, 1+10*8,	XY_HUMID, LCD_MEDIUM, currColor, backColor);

	status = 0;
}

void putTemperTank(uint16_t currColor, aquaNixieShowType aquaNixieShow_local, aquaNixieShowType tempStatus, uint16_t new_value, uint8_t *str1) {
	static uint16_t value_old = 0x0000;
	static uint8_t status = 2;
	static uint8_t selected = 0;
	static uint8_t updated = 0;
	static uint16_t backColor = LCD_BLACK;
	static uint8_t str[17] = "T.tank:       oC";

	if(updated == 1) {
		updated = 0;
		if(!status) status = 1;
	}

	if(value_old != new_value) {
		value_old = new_value;
		updated = 1;
		if(!status) status = 1;
	}

	if(aquaNixieShow_local == tempStatus && selected == 0) {
		backColor = LCD_DARKCYJAN;
		//currColor = LCD_BLACK;
		status = 2;
		selected = 1;
	} if(aquaNixieShow_local != tempStatus && selected == 1) {
		backColor = LCD_BLACK;
		status = 2;
		selected = 0;
	} else {
		//
	}

	if(status > 1) lcdPutStrXY(str, 	1,		XY_TTANK, LCD_MEDIUM, currColor, backColor);

	if(updated) {
		currColor = LCD_RED;
	} else {
		//if(selected > 0) currColor = LCD_BLACK;
	}

	if(status > 0) lcdPutStrXY(str1, 1+7*8,	XY_TTANK, LCD_MEDIUM, currColor, backColor);

	status = 0;
}

void putTemperRoom(uint16_t currColor, aquaNixieShowType aquaNixieShow_local, aquaNixieShowType tempStatus, uint16_t new_value, uint8_t *str1) {
	static uint16_t value_old = 0x0000;
	static uint8_t status = 2;
	static uint8_t selected = 0;
	static uint8_t updated = 0;
	static uint16_t backColor = LCD_BLACK;
	static uint8_t str[17] = "T.room:       oC";

	if(updated == 1) {
		updated = 0;
		if(!status) status = 1;
	}

	if(value_old != new_value) {
		value_old = new_value;
		updated = 1;
		if(!status) status = 1;
	}

	if(aquaNixieShow_local == tempStatus && selected == 0) {
		backColor = LCD_DARKCYJAN;
		//currColor = LCD_BLACK;
		status = 2;
		selected = 1;
	} if(aquaNixieShow_local != tempStatus && selected == 1) {
		backColor = LCD_BLACK;
		status = 2;
		selected = 0;
	} else {
		//
	}

	if(status > 1) lcdPutStrXY(str, 					 	1,		XY_TROOM, LCD_MEDIUM, currColor, backColor);

	if(updated) {
		currColor = LCD_RED;
	} else {
		//if(selected > 0) currColor = LCD_BLACK;
	}

	if(status > 0) lcdPutStrXY(str1, 1+8*8,	XY_TROOM, LCD_MEDIUM, currColor, backColor);

	status = 0;

}

void putTemperExtr(uint16_t currColor, aquaNixieShowType aquaNixieShow_local, aquaNixieShowType tempStatus, uint16_t new_value, uint8_t *str1) {
	static uint16_t value_old = 0x0000;
	static uint8_t status = 2;
	static uint8_t selected = 0;
	static uint8_t updated = 0;
	static uint16_t backColor = LCD_BLACK;
	static uint8_t str[17] = "T.extr:       oC";

	if(updated == 1) {
		updated = 0;
		if(!status) status = 1;
	}

	if(value_old != new_value) {
		value_old = new_value;
		updated = 1;
		if(!status) status = 1;
	}

	if(aquaNixieShow_local == tempStatus && selected == 0) {
		backColor = LCD_DARKCYJAN;
		//currColor = LCD_BLACK;
		status = 2;
		selected = 1;
	} if(aquaNixieShow_local != tempStatus && selected == 1) {
		backColor = LCD_BLACK;
		status = 2;
		selected = 0;
	} else {
		//
	}

	if(status > 1) lcdPutStrXY(str, 		1,			XY_TEXTR, LCD_MEDIUM, currColor, backColor);

	if(updated) {
		currColor = LCD_RED;
	} else {
		//if(selected > 0) currColor = LCD_BLACK;
	}

	if(status > 0) lcdPutStrXY(str1, 	1+7*8,		XY_TEXTR, LCD_MEDIUM, currColor, backColor);

	status = 0;
}

void putPressure(uint16_t currColor, aquaNixieShowType aquaNixieShow_local, aquaNixieShowType tempStatus, uint16_t new_value, uint8_t *str1) {
	static uint16_t value_old = 0x0000;
	static uint8_t status = 2;
	static uint8_t selected = 0;
	static uint8_t updated = 0;
	static uint16_t backColor = LCD_BLACK;
	static uint8_t str[17] = "P.room:      hPa";

	if(updated == 1) {
		updated = 0;
		if(!status) status = 1;
	}

	if(value_old != new_value) {
		value_old = new_value;
		updated = 1;
		if(!status) status = 1;
	}

	if(aquaNixieShow_local == tempStatus && selected == 0) {
		backColor = LCD_DARKCYJAN;
		//currColor = LCD_BLACK;
		status = 2;
		selected = 1;
	} if(aquaNixieShow_local != tempStatus && selected == 1) {
		backColor = LCD_BLACK;
		status = 2;
		selected = 0;
	} else {
		//
	}

	if(status > 1) lcdPutStrXY(str, 				 1,			XY_PROOM, LCD_MEDIUM, currColor, backColor);

	if(updated) {
		currColor = LCD_RED;
	} else {
		//if(selected > 0) currColor = LCD_BLACK;
	}

	if(status > 0) lcdPutStrXY(str1, 1+7*8,  XY_PROOM, LCD_MEDIUM, currColor, backColor);

	status = 0;
}

void putLED(uint8_t index, uint16_t currColor, aquaNixieShowType aquaNixieShow_local, aquaNixieShowType tempStatus, uint16_t new_value, uint8_t *str1) {
	static uint16_t value_old[4] = {0x0000,0x0000,0x0000,0x0000};
	static uint8_t status[4] = {2,2,2,2};
	static uint8_t selected[4] = {0,0,0,0};
	static uint8_t updated[4] = {0,0,0,0};
	static uint16_t backColor[4] = {LCD_BLACK,LCD_BLACK,LCD_BLACK,LCD_BLACK};
	static uint8_t str[4][17] = {"LED1:      %","LED2:      %","LED3:      %","LED4:      %"};
	uint8_t tmp;

	if(updated[index] == 1) {
		updated[index] = 0;
		if(!status[index]) status[index] = 1;
	}

	if(value_old[index] != new_value) {
		value_old[index] = new_value;
		updated[index] = 1;
		if(!status[index]) status[index] = 1;
	}

	if(aquaNixieShow_local == tempStatus && selected[index] == 0) {
		backColor[index] = LCD_DARKCYJAN;
		//currColor = LCD_BLACK;
		status[index] = 2;
		selected[index] = 1;
	} if(aquaNixieShow_local != tempStatus && selected[index] == 1) {
		backColor[index] = LCD_BLACK;
		status[index] = 2;
		selected[index] = 0;
	} else {
		//
	}

	switch(index) {
		case 0: 	tmp = XY_LED0; break;
		case 1: 	tmp = XY_LED1; break;
		case 2: 	tmp = XY_LED2; break;
		case 3: 	tmp = XY_LED3; break;
		default: 	tmp = XY_LED0; break;
	}

	if(status[index] > 1) lcdPutStrXY(str[index], 				 1,			tmp, LCD_MEDIUM, currColor, backColor[index]);

	if(updated[index]) {
		currColor = LCD_RED;
	} else {
		//if(selected[index] > 0) currColor = LCD_BLACK;
	}

	if(status[index] > 0) lcdPutStrXY(str1, 1+6*8,  	tmp, LCD_MEDIUM, currColor, backColor[index]);

	status[index] = 0;
}

void putTime(uint16_t currColor, aquaNixieShowType aquaNixieShow_local, aquaNixieShowType tempStatus, uint32_t new_value) {
	static uint32_t value_old = 0x00000000;
	static uint8_t aquaLCDTimerFlasher = 0;
	static uint8_t status_h = 2;
	static uint8_t status_m = 2;
	static uint8_t status_s = 2;
	static uint8_t selected = 0;
	static uint16_t backColor = LCD_BLACK;
	static uint8_t str[17] = "        ";

	if(value_old != new_value) {
		value_old = new_value;
		if(!status_s) status_s = 1;
		if(!status_m) status_m = 1;
		if(!status_h) status_h = 1;
		aquaLCDTimerFlasher = 1 - aquaLCDTimerFlasher;
	}

	if(aquaNixieShow_local == tempStatus && selected == 0) {
		status_h = 2;
		status_m = 1;
		status_s = 1;
		selected = 1;
	} if(aquaNixieShow_local != tempStatus && selected == 1) {
		status_h = 2;
		status_m = 1;
		status_s = 1;
		selected = 0;
	} else {
		//
	}

	if(aquaNixieShow_local == tempStatus) {
		backColor = LCD_DARKCYJAN;
		//currColor = LCD_BLACK;
	} else {
		backColor = LCD_BLACK;
	}

	if(status_h > 1) lcdPutStrXY(str, 1+8*8,  	XY_TIME, LCD_LARGE, currColor, backColor);
	if(status_s > 0) {
		lcdPutCh((uint8_t)((value_old >> 4) & 0x0000000F)+0x30 ,1+6*8, XY_TIME, LCD_LARGE, currColor, backColor);
		lcdPutCh((uint8_t)((value_old) & 0x0000000F)+0x30 ,1+7*8, XY_TIME, LCD_LARGE, currColor, backColor);
	}

	if(status_m > 0) {
		lcdPutCh((uint8_t)((value_old >> 12) & 0x0000000F)+0x30 ,1+3*8, XY_TIME, LCD_LARGE, currColor, backColor);
		lcdPutCh((uint8_t)((value_old >> 8) & 0x0000000F)+0x30 ,1+4*8, XY_TIME, LCD_LARGE, currColor, backColor);
	}

	if(status_h > 0) {
		lcdPutCh((uint8_t)((value_old >> 20) & 0x0000000F)+0x30 ,1, XY_TIME, LCD_LARGE, currColor, backColor);
		lcdPutCh((uint8_t)((value_old >> 16) & 0x0000000F)+0x30 ,1+1*8, XY_TIME, LCD_LARGE, currColor, backColor);
	}

	if(aquaLCDTimerFlasher) {
		lcdPutCh(':',1+2*8, XY_TIME, LCD_LARGE, currColor, backColor);
		lcdPutCh(':',1+5*8, XY_TIME, LCD_LARGE, currColor, backColor);
	}
	else {
		lcdPutCh(' ',1+2*8, XY_TIME, LCD_LARGE, currColor, backColor);
		lcdPutCh(' ',1+5*8, XY_TIME, LCD_LARGE, currColor, backColor);
	}

	status_s = 0;
	status_m = 0;
	status_h = 0;
}
