/*
 * kho_ds18b20_freertos.h
 *
 *  Created on: 15 gru 2014
 *      Author: Krzysztof
 */

#ifndef INC_KHO_DS18B20_FREERTOS_H_
#define INC_KHO_DS18B20_FREERTOS_H_

#include "board.h"
#include "kho_1wire_freertos.h"

#define DS18B20_INIT_READY	0
#define DS18B20_INIT_TASK_ERROR 1

#define TANK_SENSOR_ID	0x5
#define EXTR_SENSOR_ID	0x6

#define NO_OF_SENSORS	2

#define TANK_SENSOR		0
#define EXTR_SENSOR		1

volatile static uint16_t DS18B20_IDs[NO_OF_SENSORS][8] = {{0x28,0x89,0xD5,0xB3,0x04,0x00,0x00,0x69},{0x28,0x81,0xE6,0xB2,0x04,0x00,0x00,0x8D}};
//volatile static uint16_t DS18B20_IDs[NO_OF_SENSORS][8] = {{0x69,0x00,0x00,0x04,0xB3,0xD5,0x89,0x28},{0x8D,0x00,0x00,0x04,0xF2,0xE6,0x81,0x28}};
volatile static int16_t DS18B20_Temperature[NO_OF_SENSORS] = {0,0};
volatile static uint16_t DS18B20_Temperature_bcd[NO_OF_SENSORS] = {0,0};
static uint8_t  DS18B20_Temperature_str[NO_OF_SENSORS][8] = {"   0.00","   0.00"};

uint8_t init_ds18b20(void);

void DS18B20RequestMeasurement(int);
void DS18B20RequestData(int);

uint16_t DS18B20Temperature_dec(int);
uint16_t DS18B20Temperature_bcd(int);
uint8_t * DS18B20Temperature_str(int);

#endif /* INC_KHO_DS18B20_FREERTOS_H_ */
