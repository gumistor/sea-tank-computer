/*
 * kho_nixie_freertos.h
 *
 *  Created on: 3 sie 2014
 *      Author: Krzysztof
 */

#ifndef KHO_NIXIE_FREERTOS_H_
#define KHO_NIXIE_FREERTOS_H_

#include "board.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "kho_mbr3_freertos.h"

#define PCA_NIXIE_A_ID							0x3F
#define PCA_NIXIE_B_ID							0x3C
#define PCA_NIXIE_C_ID							0x38
#define PCA_NIXIE_D_ID							0x3E

volatile static uint8_t nixieNum0 = 0xFF;
volatile static uint8_t nixieNum1 = 0xFF;
volatile static uint8_t nixieNum2 = 0xFF;

volatile static uint8_t nixieOffBuffer;

void SendNewNixieValue(uint8_t *);
void SendNewNixieValue_str(uint8_t *, uint8_t);

#endif /* KHO_NIXIE_FREERTOS_H_ */
