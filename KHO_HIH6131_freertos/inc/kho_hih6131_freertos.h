/*
 * kho_ds1340_freertos.h
 *
 *  Created on: 4 sie 2014
 *      Author: Krzysztof
 */

#ifndef KHO_HIH131_FREERTOS_H_
#define KHO_HIH131_FREERTOS_H_

#include "board.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "kho_mbr3_freertos.h"

#define HIH_INIT_READY	1
#define HIH_INIT_ERROR	0

#define HIH61_I2C_ID							0x27

#define NO_VALID_DATA							0b00000000
#define VALID_DATA								0b01000000
#define ERROR_DATA								0b10000000

volatile static int16_t Temperature;
volatile static uint16_t Humidity;
volatile static uint16_t Temperature_bcd;
volatile static uint16_t Humidity_bcd;
static uint8_t Temperature_str[7] = " 000.0";
static uint8_t Humidity_str[7]="000.00";

uint8_t init_hih61(void);
uint8_t setConfig_hih61(void);

void HIHTaskRequestMeasurement(void);
void HIHTaskCalculate(void *);
uint16_t HIHReturnHumidity(void);
int16_t HIHReturnTemperature(void);
uint16_t HIHReturnHumidity_bcd(void);
uint16_t HIHReturnTemperature_bcd(void);
uint8_t *  HIHReturnHumidity_str(void);
uint8_t * HIHReturnTemperature_str(void);

#endif /* KHO_HIH131_FREERTOS_H_ */
