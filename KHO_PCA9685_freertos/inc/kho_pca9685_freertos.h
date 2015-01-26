/*
 * kho_pca9685_freertos.h
 *
 *  Created on: 2 sie 2014
 *      Author: Krzysztof
 */

#ifndef KHO_PCA9685_FREERTOS_H_
#define KHO_PCA9685_FREERTOS_H_

#include "board.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "kho_mbr3_freertos.h"

#define PCA_I2C_ID							0x40

//MODE1 bit 7
#define PCA_RESTART_DISABLED				0b00000000	//default
#define PCA_RESTART_ENABLED					0b10000000
//MODE1 bit 6
#define PCA_INTERNAL_CLK					0b00000000	//default
#define PCA_EXTERNAL_CLK					0b01000000
//MODE1 bit 5
#define PCA_AUTO_INCREMENT_OFF				0b00000000	//default
#define PCA_AUTO_INCREMENT_ON				0b00100000
//MODE1 bit 4
#define PCA_NORMAL_MODE						0b00000000
#define PCA_SLEEP_MODE						0b00010000	//default
//MODE1 bit 3
#define PCA_SUBADDRES_1_OFF					0b00000000	//default
#define PCA_SUBADDRES_1_ON					0b00001000
//MODE1 bit 2
#define PCA_SUBADDRES_2_OFF					0b00000000	//default
#define PCA_SUBADDRES_2_ON					0b00000100
//MODE1 bit 1
#define PCA_SUBADDRES_3_OFF					0b00000000	//default
#define PCA_SUBADDRES_3_ON					0b00000010
//MODE1 bit 0
#define PCA_ALL_CALL_OFF					0b00000000
#define PCA_ALL_CALL_ON						0b00000001	//default
//MODE2 bit 4
#define PCA_INVRT_OUTPUT_NOT_INVERTED		0b00000000	//default
#define PCA_INVRT_OUTPUT_INVERTED			0b00010000
//MODE2 bit 3
#define PCA_CHANGE_ON_STOP					0b00000000	//default
#define PCA_CHANGE_ON_ACK					0b00001000
//MODE2 bit 3
#define PCA_OPEN_DRAIN						0b00000000
#define PCA_TOTEM_POLE						0b00000100	//default
//MODE2 bit 1 and 0
#define PCA_OE1_OFF_ALL						0b00000000	//default
#define PCA_OE1_VS_OPEN_DRAIN				0b00000001
#define PCA_OE1_HIGH_IMP					0b00000010

#define PCA_MODE1_ADDR						0x00
#define PCA_MODE1			PCA_RESTART_DISABLED | PCA_INTERNAL_CLK | PCA_AUTO_INCREMENT_OFF | PCA_NORMAL_MODE | PCA_SUBADDRES_1_OFF | PCA_SUBADDRES_2_OFF | PCA_SUBADDRES_3_OFF | PCA_ALL_CALL_ON
#define PCA_MODE2_ADDR						0x01
#define PCA_MODE2			PCA_INVRT_OUTPUT_NOT_INVERTED | PCA_CHANGE_ON_STOP | PCA_TOTEM_POLE | PCA_OE1_OFF_ALL
#define PCA_PRE_SCALE_ADDR					0xFE
#define PCA_PRE_SCALE						0x0F

static int16_t ledOutput[4] = {0x0000, 0x0000, 0x0000, 0x0000};
static uint8_t ledOutput_percent[4] = {0,0,0,0};

static uint8_t ledSelected = 0;

void PCA_Init(void);

void SendNewPWM(uint8_t);
void SendOneNewPWM(uint8_t, uint16_t);
void ChangePWMBy(uint8_t, int8_t);
void ChangePWM(uint8_t, uint8_t);
void ChangeLED(uint8_t);

uint8_t getCurrentLedSelected(void);
uint8_t getCurrentLedPercent(uint8_t);

#endif /* KHO_PCA9685_FREERTOS_H_ */
