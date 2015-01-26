/*
 * kho_pwm_freertos.h
 *
 *  Created on: 1 gru 2014
 *      Author: Krzysztof
 */

#ifndef KHO_PWM_FREERTOS_H_
#define KHO_PWM_FREERTOS_H_

#include "board.h"

void initLPC_PWM(void);

void setPWM(uint8_t, int16_t);
void setPWM1(int16_t value);
void setPWM2(int16_t value);
void setPWM3(int16_t value);
void setPWM4(int16_t value);

#endif /* KHO_PWM_FREERTOS_H_ */
