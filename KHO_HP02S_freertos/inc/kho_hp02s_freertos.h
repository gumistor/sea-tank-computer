/*
 * kho_hp02s_freertos.h
 *
 *  Created on: 21 lis 2014
 *      Author: Krzysztof Holon
 */

#ifndef KHO_HP02S_FREERTOS_H_
#define KHO_HP02S_FREERTOS_H_

#include <stdio.h>
#include "board.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "kho_i2c_freertos.h"

#define HP_INIT_READY	1
#define HP_INIT_ERROR	0

//Address of AD converter - needed to get T1,D1 values
#define HP02S_I2C_ID							0x77
//Address of EEPROM with coefficients to calculate Pressure
//By default address is set to 0 and EEPROM is compatible with AT24C02
#define HP02S_I2C_EEPROM						0x50

//Setup of output port for XCLR - analog-digital converter in the sensor
#define HP02S_XCLR_PORT							2
#define HP02S_XCLR_PIN							13
#define HP02S_XCLR_ON							true
#define HP02S_XCLR_IDLE							false

//Coefficients needed to calculate pressure
//Should be read once and even can be hard coded if sensor would never change
volatile static uint16_t D0;	//add: 02:03, range: 0x0100 - 0xFFFF
volatile static uint16_t T0;	//add: 04:05, range: 0x0100 - 0xFFFF
volatile static uint16_t P0;	//add: 06:07, range: 0x0100 - 0xFFFF
volatile static uint16_t S;		//add: 08:09, range: 0x0001 - 0xFFFF
volatile static uint16_t C;		//add: 0A:0B, range: 0x0001 - 0xFFFF

//dUP = D1 - D0
//DUT = (29*(T1-T0)/50-(T1-T0)^2/76666)*S/C
//Pressure = P0 + 100*(dUP+dUT)/S
//calculated value is absolute pressure
volatile static uint16_t Pressure = 0;
volatile static uint16_t Pressure_bcd = 0;
static uint8_t  Pressure_str[7] = "0000.0";

//init sensor
uint8_t init_hp02s(void);

//calculate sensor FreeRTOS task
void HPTaskCalculate(void *);

//return current pressure
uint16_t HPReturnPressure_dec(void);
uint16_t HPReturnPressure_bcd(void);
uint8_t * HPReturnPressure_str(void);

void HPRequestMeasurement(void);
void HPRequestCoef(void);

#endif /* KHO_HP02S_FREERTOS_H_ */
