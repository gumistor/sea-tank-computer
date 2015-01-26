/*
 * kho_hp02s_freertos.c

 *
 *  Created on: 21 lis 2014
 *      Author: Krzysztof
 */
#include "kho_hp02s_freertos.h"
#include "kho_hp02s_inittask.h"

uint8_t init_hp02s(void) {
	uint8_t return_value = HP_INIT_READY;
	if(!xTaskCreate(HPTaskCalculate,NAME_HPTaskCalculate,STACK_HPTaskCalculate, NULL, PRIORITY_HPTaskCalculate,&handleHPTaskCalculate)) {
		return_value = HP_INIT_ERROR;
	} else {
		Pressure = 0;
		Pressure_bcd = 0;
		D0 = 0;
		T0 = 0;
		P0 = 0;
		S = 0;
		C = 0;

		Chip_GPIO_SetPinDIROutput(LPC_GPIO, HP02S_XCLR_PORT, HP02S_XCLR_PIN);
		Chip_Clock_SetCLKOUTSource(SYSCTL_CLKOUTSRC_RTC, 1);

		HPRequestMeasurement();

	}
	return return_value;
}

void HPStartADConverter(void) {
	Chip_Clock_EnableCLKOUT();
	Chip_GPIO_SetPinState(LPC_GPIO, HP02S_XCLR_PORT, HP02S_XCLR_PIN, HP02S_XCLR_ON);
}

void HPStopADConverter(void) {
	Chip_Clock_EnableCLKOUT();
	Chip_GPIO_SetPinState(LPC_GPIO, HP02S_XCLR_PORT, HP02S_XCLR_PIN, HP02S_XCLR_IDLE);
}

void HPRequestMeasurement(void) {
	i2c_struct data_to_send;
	if(0 == D0 || 0 == T0 || 0 == P0 || 0 == S || 0 == C) {
		HPStopADConverter();
		data_to_send.id = HP02S_I2C_EEPROM;
		data_to_send.size_of_send = 1;
		data_to_send.data_to_send[0] = 0x02;
		data_to_send.size_of_received = 10;
		data_to_send.read_notwrite = I2C_READ;
		xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);

	} else {
		HPStartADConverter();
		vTaskDelay(configTICK_RATE_HZ / 16);
		data_to_send.id = HP02S_I2C_ID;
		data_to_send.size_of_send = 2;
		data_to_send.data_to_send[0] = 0xFF;
		data_to_send.data_to_send[1] = 0xF0;
		data_to_send.size_of_received = 0;
		data_to_send.read_notwrite = I2C_WRITE;
		xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);
		vTaskDelay(configTICK_RATE_HZ / 16);
		data_to_send.size_of_send = 1;
		data_to_send.data_to_send[0] = 0xFD;
		data_to_send.size_of_received = 2;
		data_to_send.read_notwrite = I2C_READ;
		xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);
	}
}

void HPRequestCoef(void) {

}

void HPTaskCalculate(void *pvParameters) {
	i2c_struct data_to_send;
	portBASE_TYPE xStatus;
	uint16_t T1, D1;
	int16_t dUT, dUP;

	while (1) {
		xStatus = xQueuePeek(xRead_I2CQueue, &data_to_send, portMAX_DELAY);
		if(xStatus && (HP02S_I2C_EEPROM == data_to_send.id || HP02S_I2C_ID == data_to_send.id)) {
			xQueueReceive(xRead_I2CQueue, &data_to_send, portMAX_DELAY);
			if(xStatus) {
				if(HP02S_I2C_EEPROM == data_to_send.id) {
					D0 = 0x0000 | ((data_to_send.data_received[0]) << 8) | data_to_send.data_received[1];
					T0 = 0x0000 | ((data_to_send.data_received[2]) << 8) | data_to_send.data_received[3];
					P0 = 0x0000 | ((data_to_send.data_received[4]) << 8) | data_to_send.data_received[5];
					S = 0x0000 | ((data_to_send.data_received[6]) << 8) | data_to_send.data_received[7];
					C = 0x0000 | ((data_to_send.data_received[8]) << 8) | data_to_send.data_received[9];
				} else {

					D1 = 0x00000000 | (data_to_send.data_received[0] << 8) | data_to_send.data_received[1];

					data_to_send.id = HP02S_I2C_ID;
					data_to_send.size_of_send = 2;
					data_to_send.data_to_send[0] = 0xFF;
					data_to_send.data_to_send[1] = 0xE8;
					data_to_send.size_of_received = 0;
					data_to_send.read_notwrite = I2C_WRITE;
					xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);
					vTaskDelay(configTICK_RATE_HZ / 16);
					data_to_send.size_of_send = 1;
					data_to_send.data_to_send[0] = 0xFD;
					data_to_send.size_of_received = 2;
					data_to_send.read_notwrite = I2C_READ;
					xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);

					data_to_send.id = 0;

					while(HP02S_I2C_ID != data_to_send.id) {
						xStatus = xQueuePeek(xRead_I2CQueue, &data_to_send, portMAX_DELAY);
					}

					if(xStatus) {
						xQueueReceive(xRead_I2CQueue, &data_to_send, portMAX_DELAY);
						if(xStatus) {
							T1 = 0x00000000 | (data_to_send.data_received[0] << 8) | data_to_send.data_received[1];
							dUP = D1 - D0;
							dUT = (29*(T1-T0)/50-(T1-T0)*(T1-T0)/76666)*S/C;
							Pressure = P0 +100*(dUP+dUT)/S;
							Pressure_bcd = ((Pressure/10000) << 16) | (((Pressure%10000)/1000) << 12) | ((((Pressure%10000)%1000)/100) << 8) | (((((Pressure%10000)%1000)%100)/10) << 4) | ((((Pressure%10000)%1000)%100)%10);
							sprintf((char *)Pressure_str, "%4d.%d",Pressure/10,Pressure%10);
							HPStopADConverter();
						}
					}
				}
			}
		}
	}
}

uint16_t HPReturnPressure_dec(void) {
	return Pressure;
}

uint16_t HPReturnPressure_bcd(void) {
	return Pressure_bcd;
}

uint8_t * HPReturnPressure_str(void) {
	return Pressure_str;
}
