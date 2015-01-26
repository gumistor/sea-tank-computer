/*
 * kho_ds1340_freertos.c
 *
 *  Created on: 4 sie 2014
 *      Author: Krzysztof
 */

#include "kho_ds1340_freertos.h"
#include "taskConfigData.h"
//#include "aqua_main.h"

void RTC_Init(void) {
	vSemaphoreCreateBinary(xRTCAlarmSemaphore);

	//RTCsetDateTime(0x19, 0x21, 0x00, 0x30, 0x08, 0x14, 0x06);

	if(xRTCAlarmSemaphore != NULL) {

		//xTaskCreate(RTCTask,nameRTCTask,configMINIMAL_STACK_SIZE,NULL,PRIORITY_RTCTask,handleRTCTask);
	}
	else {
		while(1) { }
	}
}

void RTCTask(void *pvParameters) {
	i2c_struct data_to_send;
	portBASE_TYPE xStatus;

	while (1) {
		data_to_send.id = RTC_I2C_ID;
		data_to_send.size_of_send = 1;
		data_to_send.data_to_send[0] = 0x00;
		data_to_send.size_of_received = 9;
		data_to_send.read_notwrite = I2C_READ;
		xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);

		//xStatus = xQueuePeek(xRead_I2CQueue, &data_to_send, portMAX_DELAY);
		data_to_send.id = 0;
		while(RTC_I2C_ID != data_to_send.id) {
			xStatus = xQueuePeek(xRead_I2CQueue, &data_to_send, portMAX_DELAY);
		}

		if( xStatus == pdPASS && RTC_I2C_ID == data_to_send.id) {
			//if() {
				xQueueReceive(xRead_I2CQueue, &data_to_send, portMAX_DELAY);
				//
				//
				currentTime[0] = data_to_send.data_received[0] & 0x7F;
				currentTime[1] = data_to_send.data_received[1] & 0x7F;
				currentTime[2] = data_to_send.data_received[2] & 0x3F;
				currentDate[3] = data_to_send.data_received[3] & 0x07;
				currentDate[2] = data_to_send.data_received[4] & 0x3F;
				currentDate[1] = data_to_send.data_received[5] & 0x1F;
				currentDate[0] = data_to_send.data_received[6] & 0xFF;

				if(alarmTime[0] != 0xFF || alarmTime[1] != 0xFF || alarmTime[2] != 0xFF ||
						alarmTime[3] != 0xFF || alarmTime[4] != 0xFF || alarmTime[5] != 0xFF) {
					if((alarmTime[0] == 0xFF || (alarmTime[0] == currentTime[0])) &&
							(alarmTime[1] == 0xFF || (alarmTime[1] == currentTime[1])) &&
							(alarmTime[2] == 0xFF || (alarmTime[2] == currentTime[2])) &&
							(alarmTime[3] == 0xFF || (alarmTime[3] == currentDate[2])) &&
							(alarmTime[4] == 0xFF || (alarmTime[4] == currentDate[1])) &&
							(alarmTime[5] == 0xFF || (alarmTime[5] == currentDate[0]))){
							xSemaphoreGive(xRTCAlarmSemaphore);
							alarmTime[0] = 0xFF;
							alarmTime[1] = 0xFF;
							alarmTime[2] = 0xFF;
							alarmTime[3] = 0xFF;
							alarmTime[4] = 0xFF;
							alarmTime[5] = 0xFF;
					}
				}
		}
		else {
		}
		vTaskDelay(configTICK_RATE_HZ / 2);
	}
}



void ReturnCurrentTime(uint8_t *sec, uint8_t *min, uint8_t *hour) {
	*sec = currentTime[0];
	*min = currentTime[1];
	*hour = currentTime[2];
}

void ReturnCurrentDate(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *week) {
	*year = currentDate[0];
	*month = currentDate[1];
	*day = currentDate[2];
	*week = currentDate[3];
}

void SetAlarmTime(uint8_t hour, uint8_t minute, uint8_t seconds, uint8_t day, uint8_t month, uint8_t year) {
	alarmTime[0] = seconds;
	alarmTime[1] = minute;
	alarmTime[2] = hour;
	alarmTime[3] = day;
	alarmTime[4] = month;
	alarmTime[5] = year;
}

void RTCsetDateTime(uint8_t hour, uint8_t minute, uint8_t seconds, uint8_t day, uint8_t month, uint8_t year, uint8_t week) {
	i2c_struct data_to_send;

	data_to_send.id = RTC_I2C_ID;
	data_to_send.size_of_send = 8;
	data_to_send.data_to_send[0] = 0x00; //start addres
	data_to_send.data_to_send[1] = seconds;
	data_to_send.data_to_send[2] = minute;
	data_to_send.data_to_send[3] = hour;
	data_to_send.data_to_send[4] = week;
	data_to_send.data_to_send[5] = day;
	data_to_send.data_to_send[6] = month;
	data_to_send.data_to_send[7] = year;
	data_to_send.size_of_received = 0;
	data_to_send.read_notwrite = I2C_WRITE;
	xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);
}
