/*
 * kho_aquaLCD_Data.h
 *
 *  Created on: 15 gru 2014
 *      Author: Krzysztof
 */

#ifndef INC_KHO_AQUALCD_DATA_H_
#define INC_KHO_AQUALCD_DATA_H_

#include "board.h"
#include "aqua_main.h"
#include "kho_lcd_ea_freertos.h"

#define XY_DATE			1+8*0
#define XY_DATE_DAY		1+8*1
#define XY_TIME			1+8*2 //two lines
#define XY_HUMID		1+8*4
#define XY_TROOM		1+8*5
#define XY_TTANK		1+8*6
#define XY_TEXTR		1+8*7
#define XY_PROOM		1+8*8
#define XY_LED0			1+8*10
#define XY_LED1			1+8*11
#define XY_LED2			1+8*12
#define XY_LED3			1+8*13

void putTime(uint16_t, aquaNixieShowType, aquaNixieShowType, uint32_t);
void putLED(uint8_t, uint16_t, aquaNixieShowType, aquaNixieShowType, uint16_t, uint8_t *);
void putPressure(uint16_t, aquaNixieShowType, aquaNixieShowType, uint16_t, uint8_t *);
void putTemperExtr(uint16_t, aquaNixieShowType, aquaNixieShowType, uint16_t, uint8_t *);
void putTemperRoom(uint16_t, aquaNixieShowType, aquaNixieShowType, uint16_t, uint8_t *);
void putTemperTank(uint16_t, aquaNixieShowType, aquaNixieShowType, uint16_t, uint8_t *);
void putHumidity(uint16_t, aquaNixieShowType, aquaNixieShowType, uint16_t, uint8_t *);
void putDate(uint16_t, aquaNixieShowType, aquaNixieShowType, uint32_t, uint8_t *, uint8_t *);

#endif /* INC_KHO_AQUALCD_DATA_H_ */
