/*
 * kho_pwm_freertos.c
 *
 *  Created on: 1 gru 2014
 *      Author: Krzysztof
 */

#include "kho_pwm_freertos.h"

void initLPC_PWM(void) {
	LPC_SYSCTL->PCLKSEL[0] &=~(3<<12);      // reset
	LPC_SYSCTL->PCLKSEL[0] |= (1<<12);      // set PCLK to full CPU speed (96MHz)
	LPC_SYSCTL->PCONP |= (1 << 6);        // PWM on
	LPC_IOCON->PINSEL[4] &=~(3<<0);    // reset
	LPC_IOCON->PINSEL[4] |= (1<<0);    // set PWM1.1 at P2.0
	LPC_IOCON->PINSEL[4] |= (1<<2);    // set PWM1.2 at P2.1
	LPC_IOCON->PINSEL[4] |= (1<<4);    // set PWM1.3 at P2.2
	LPC_IOCON->PINSEL[4] |= (1<<6);    // set PWM1.4 at P2.3
	LPC_PWM1->TCR = (1<<1);           // counter reset
	LPC_PWM1->PR  = 96UL-1;     // clock /96 / prescaler (= PR +1) = 0,001 s
	LPC_PWM1->MCR = (1<<1);           // reset on MR0
	LPC_PWM1->MR[0] = 1000;                // set PWM cycle 1kHz
	LPC_PWM1->MR[1] = 0;                // set duty
	LPC_PWM1->MR[2] = 0;                // set duty
	LPC_PWM1->MR[3] = 0;                // set duty
	LPC_PWM1->EMR[0] = 0;                // set duty
	LPC_PWM1->LER = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4);    // latch MR0 & MR1 & MR2 & MR3 & MR4
	LPC_PWM1->PCR = (1<<9)|(1<<10)|(1<<11)|(1<<12);           // PWM1 output enable
	LPC_PWM1->TCR = (1<<1)|(1<<0)|(1<<3);// counter enable, PWM enable
	LPC_PWM1->TCR = (1<<0)|(1<<3);    // counter enable, PWM enable

}

void setPWM(uint8_t index, int16_t value) {
	if(value>1000) value = 1000;
	if(value<0) value = 0;
	if(index>3) LPC_PWM1->EMR[index-4] = value;
	else LPC_PWM1->MR[index] = value;                // set duty
	LPC_PWM1->LER |= (1<<index);
}

void setPWM1(int16_t value) {
	if(value>1000) value = 1000;
	if(value<0) value = 0;
	LPC_PWM1->MR[1] = value;                // set duty
	LPC_PWM1->LER |= (1<<1);
}

void setPWM2(int16_t value) {
	if(value>1000) value = 1000;
	if(value<0) value = 0;
	LPC_PWM1->MR[2] = value;                // set duty
	LPC_PWM1->LER |= (1<<2);
}

void setPWM3(int16_t value) {
	if(value>1000) value = 1000;
	if(value<0) value = 0;
	LPC_PWM1->MR[3] = value;                // set duty
	LPC_PWM1->LER |= (1<<3);
}

void setPWM4(int16_t value) {
	if(value>1000) value = 1000;
	if(value<0) value = 0;
	LPC_PWM1->EMR[0] = value;                // set duty
	LPC_PWM1->LER |= (1<<4);
}
