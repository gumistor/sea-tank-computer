/*
 * kho_ds18b20_freertos.c
 *
 *  Created on: 15 gru 2014
 *      Author: Krzysztof
 */

#include "kho_ds18b20_freertos.h"
#include "kho_ds18b20_taskinit.h"
#include "queue.h"

uint8_t init_ds18b20(void) {
	uint8_t return_value = DS18B20_INIT_READY;

	if(!xTaskCreate(DS18B20TaskCalculate,NAME_DS18B20TaskCalculate,STACK_DS18B20TaskCalculate,NULL,PRIORITY_DS18B20TaskCalculate,&handleDS18B20TaskCalculate)) {
		return_value = DS18B20_INIT_TASK_ERROR;
	} else {
		//DS18B20RequestMeasurement(TANK_SENSOR);
	}
	return return_value;
}

void DS18B20TaskCalculate(void *pvParameters) {
	uint8_t sensorNo = 0;
	int16_t calculated_value;
	onewire_struct data_to_send;
	portBASE_TYPE xStatus;

	taskDS18B20ready = 1;

	while(1) {
		xStatus = xQueuePeek(xRead_oneWireQueue, &data_to_send, portMAX_DELAY);
		if(xStatus && TANK_SENSOR == data_to_send.id || xStatus && EXTR_SENSOR == data_to_send.id) {
			xQueueReceive(xRead_oneWireQueue, &data_to_send, portMAX_DELAY);
			if(xStatus) {
				if(data_to_send.data_to_send[9] == 0xBE) {
					sensorNo = data_to_send.id;
					calculated_value = 0x0000 | (data_to_send.data_to_send[11] << 8) | data_to_send.data_to_send[10];
					//[10] 50
					 //[11] 5
					DS18B20_Temperature[sensorNo] = (calculated_value * 100)/16;
					DS18B20_Temperature_bcd[sensorNo] = ((DS18B20_Temperature[sensorNo]/10000) << 16) | (((DS18B20_Temperature[sensorNo]%10000)/1000) << 12) | ((((DS18B20_Temperature[sensorNo]%10000)%1000)/100) << 8) | (((((DS18B20_Temperature[sensorNo]%10000)%1000)%100)/10) << 4) | (((((DS18B20_Temperature[sensorNo]%10000)%1000)%100)%10) << 0);
					if(DS18B20_Temperature[sensorNo] < 0) sprintf((char *)DS18B20_Temperature_str[sensorNo], "-%3d.%02d",abs(DS18B20_Temperature[sensorNo]/100),abs(DS18B20_Temperature[sensorNo]%100));
					else sprintf((char *)DS18B20_Temperature_str[sensorNo], " %3d.%02d",DS18B20_Temperature[sensorNo]/100,DS18B20_Temperature[sensorNo]%100);
				} else if(data_to_send.data_to_send[9] == 0x44) {
					vTaskDelay(configTICK_RATE_HZ);
					DS18B20RequestData(data_to_send.id);
				} else {
					//
				}

			}
		}
	}
}

void DS18B20RequestMeasurementx(int sensorNo) {
	onewire_struct data_to_send;
	uint8_t i;

/*	data_to_send.id = sensorNo;
	data_to_send.data_to_send[0] = 0xCC;
	data_to_send.data_to_send[1] = 0x44;
	data_to_send.size_of_send = 2;
	xQueueSendToBack(xWrite_oneWireQueue, &data_to_send, 0);

	data_to_send.id = sensorNo;
	data_to_send.data_to_send[0] = 0xCC;
	data_to_send.data_to_send[1] = 0xBE;
	for(i = 2; i < 10; i++)
		data_to_send.data_to_send[i] = 0xFF;
	data_to_send.size_of_send = 10;
	xQueueSendToBack(xWrite_oneWireQueue, &data_to_send, 0);*/

	data_to_send.id = sensorNo;
		data_to_send.data_to_send[0] = 0x33;
		//data_to_send.data_to_send[1] = 0xBE;
		for(i = 1; i < 9; i++)
			data_to_send.data_to_send[i] = 0xFF;
		data_to_send.size_of_send = 9;
		xQueueSendToBack(xWrite_oneWireQueue, &data_to_send, 0);
}

void DS18B20RequestMeasurement(int sensorNo) {
	onewire_struct data_to_send;
	uint8_t i;

	data_to_send.id = sensorNo;
	data_to_send.data_to_send[0] = 0x55;
	for(i = 1; i < 9; i++)
		data_to_send.data_to_send[i] = DS18B20_IDs[sensorNo][i-1];
	data_to_send.data_to_send[i] = 0x44;
	data_to_send.size_of_send = 10;
	xQueueSendToBack(xWrite_oneWireQueue, &data_to_send, 0);
}

void DS18B20RequestData(int sensorNo) {
	onewire_struct data_to_send;
	uint8_t i;
	data_to_send.id = sensorNo;
	data_to_send.data_to_send[0] = 0x55;
	//data_to_send.data_to_send[1] = 0x28;
	//data_to_send.data_to_send[2] = 0x81;
	//data_to_send.data_to_send[3] = 0xE6;
	//data_to_send.data_to_send[4] = 0xB2;
	//data_to_send.data_to_send[5] = 0x04;
	//data_to_send.data_to_send[6] = 0x00;
	//data_to_send.data_to_send[7] = 0x00;
	//data_to_send.data_to_send[8] = 0x8D;
	//data_to_send.data_to_send[9] = 0xBE;
	//data_to_send.data_to_send[10] = 0xFF;
	//data_to_send.data_to_send[11] = 0xFF;
	//data_to_send.data_to_send[12] = 0xFF;
	//data_to_send.data_to_send[13] = 0xFF;
	//data_to_send.size_of_send = 14;
	for(i = 1; i < 9; i++)
		data_to_send.data_to_send[i] = DS18B20_IDs[sensorNo][i-1];
	data_to_send.data_to_send[i] = 0xBE;
	for(i=10; i < 18; i++)
		data_to_send.data_to_send[i] = 0xFF;
	data_to_send.size_of_send = 18;
	xQueueSendToBack(xWrite_oneWireQueue, &data_to_send, 0);
}

uint16_t DS18B20Temperature_dec(int sensorNo) {
	return DS18B20_Temperature[sensorNo];
}

uint16_t DS18B20Temperature_bcd(int sensorNo) {
	return DS18B20_Temperature_bcd[sensorNo];
}

uint8_t * DS18B20Temperature_str(int sensorNo) {
	return DS18B20_Temperature_str[sensorNo];
}
