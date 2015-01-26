
#include "kho_hih6131_freertos.h"
#include "kho_hih6131_inittask.h"

uint8_t init_hih61(void) {
	uint8_t return_value = HIH_INIT_READY;
	if(!xTaskCreate(HIHTaskCalculate,NAME_HIHTaskCalculate,STACK_HIHTaskCalculate,NULL,PRIORITY_HIHTaskCalculate,&handleHIHTaskCalculate)) {
		return_value = HIH_INIT_ERROR;
		configASSERT(0);
	} else {
		Humidity = 0;
		Temperature = 0;
		Humidity_bcd = 0;
		Temperature_bcd = 0;
		HIHTaskRequestMeasurement();
	}
	return return_value;
}

uint8_t setConfig_hih61(void) {
	uint8_t return_value = HIH_INIT_READY;
	i2c_struct data_to_send;

	data_to_send.id = HIH61_I2C_ID;
	data_to_send.size_of_send = 3;
	data_to_send.data_to_send[0] = 0xA0;
	data_to_send.data_to_send[1] = 0x00;
	data_to_send.data_to_send[2] = 0x00;
	data_to_send.size_of_received = 0;
	data_to_send.read_notwrite = I2C_WRITE;
	xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);

	vTaskDelay(configTICK_RATE_HZ / 100);

	data_to_send.data_to_send[0] = 0x18;
	data_to_send.data_to_send[1] = 0x00;
	data_to_send.data_to_send[2] = 0x00;
	data_to_send.size_of_received = 0;
	data_to_send.read_notwrite = I2C_WRITE;
	xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);

	vTaskDelay(configTICK_RATE_HZ / 10);

	data_to_send.size_of_send = 0;
	data_to_send.data_to_send[0] = 0x81;
	data_to_send.data_to_send[1] = 0x00;
	data_to_send.data_to_send[2] = 0x00;
	data_to_send.size_of_received = 3;
	data_to_send.read_notwrite = I2C_READ;
	xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);

	vTaskDelay(configTICK_RATE_HZ / 10);
	data_to_send.size_of_send = 3;
	data_to_send.data_to_send[0] = 0x19;
	data_to_send.data_to_send[1] = 0x00;
	data_to_send.data_to_send[2] = 0x00;
	data_to_send.size_of_received = 0;
	data_to_send.read_notwrite = I2C_WRITE;
	xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);

	vTaskDelay(configTICK_RATE_HZ / 10);

	data_to_send.size_of_send = 0;
	data_to_send.data_to_send[0] = 0x81;
	data_to_send.data_to_send[1] = 0x00;
	data_to_send.data_to_send[2] = 0x00;
	data_to_send.size_of_received = 3;
	data_to_send.read_notwrite = I2C_READ;
	xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);

	vTaskDelay(configTICK_RATE_HZ / 100);
	data_to_send.size_of_send = 3;
	data_to_send.data_to_send[0] = 0x1A;
	data_to_send.data_to_send[1] = 0x00;
	data_to_send.data_to_send[2] = 0x00;
	data_to_send.size_of_received = 0;
	data_to_send.read_notwrite = I2C_WRITE;
	xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);

	vTaskDelay(configTICK_RATE_HZ / 10);

	data_to_send.size_of_send = 0;
	data_to_send.data_to_send[0] = 0x81;
	data_to_send.data_to_send[1] = 0x00;
	data_to_send.data_to_send[2] = 0x00;
	data_to_send.size_of_received = 3;
	data_to_send.read_notwrite = I2C_READ;
	xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);

	vTaskDelay(configTICK_RATE_HZ / 100);
	data_to_send.size_of_send = 3;
	data_to_send.data_to_send[0] = 0x1B;
	data_to_send.data_to_send[1] = 0x00;
	data_to_send.data_to_send[2] = 0x00;
	data_to_send.size_of_received = 0;
	data_to_send.read_notwrite = I2C_WRITE;
	xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);

	vTaskDelay(configTICK_RATE_HZ / 10);

	data_to_send.size_of_send = 0;
	data_to_send.data_to_send[0] = 0x81;
	data_to_send.data_to_send[1] = 0x00;
	data_to_send.data_to_send[2] = 0x00;
	data_to_send.size_of_received = 3;
	data_to_send.read_notwrite = I2C_READ;
	xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);
	data_to_send.size_of_send = 3;
	data_to_send.data_to_send[0] = 0x1C;
	data_to_send.data_to_send[1] = 0x00;
	data_to_send.data_to_send[2] = 0x00;
	data_to_send.size_of_received = 0;
	data_to_send.read_notwrite = I2C_WRITE;
	xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);

	vTaskDelay(configTICK_RATE_HZ / 10);

	data_to_send.size_of_send = 0;
	data_to_send.data_to_send[0] = 0x81;
	data_to_send.data_to_send[1] = 0x00;
	data_to_send.data_to_send[2] = 0x00;
	data_to_send.size_of_received = 3;
	data_to_send.read_notwrite = I2C_READ;
	xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);

	data_to_send.size_of_send = 3;
	data_to_send.data_to_send[0] = 0x58;
	data_to_send.data_to_send[1] = 0x23;
	data_to_send.data_to_send[2] = 0x32;
	data_to_send.size_of_received = 0;
	data_to_send.read_notwrite = I2C_WRITE;
	xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);

	vTaskDelay(configTICK_RATE_HZ / 1);

	data_to_send.size_of_send = 3;
	data_to_send.data_to_send[0] = 0x59;
	data_to_send.data_to_send[1] = 0x1F;
	data_to_send.data_to_send[2] = 0xFF;
	data_to_send.size_of_received = 0;
	data_to_send.read_notwrite = I2C_WRITE;
	xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);

	vTaskDelay(configTICK_RATE_HZ / 1);

	data_to_send.size_of_send = 3;
	data_to_send.data_to_send[0] = 0x5A;
	data_to_send.data_to_send[1] = 0x13;
	data_to_send.data_to_send[2] = 0x32;
	data_to_send.size_of_received = 0;
	data_to_send.read_notwrite = I2C_WRITE;
	xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);

	vTaskDelay(configTICK_RATE_HZ / 1);

	data_to_send.size_of_send = 3;
	data_to_send.data_to_send[0] = 0x5B;
	data_to_send.data_to_send[1] = 0x16;
	data_to_send.data_to_send[2] = 0x66;
	data_to_send.size_of_received = 0;
	data_to_send.read_notwrite = I2C_WRITE;
	xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);

	vTaskDelay(configTICK_RATE_HZ / 1);

	data_to_send.size_of_send = 3;
	data_to_send.data_to_send[0] = 0x5C;
	data_to_send.data_to_send[1] = 0x07;
	data_to_send.data_to_send[2] = 0xA7;
	data_to_send.size_of_received = 0;
	data_to_send.read_notwrite = I2C_WRITE;
	xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);

	vTaskDelay(configTICK_RATE_HZ / 1);

	vTaskDelay(configTICK_RATE_HZ / 1);

	return return_value;
}

void HIHTaskRequestMeasurement(void) {
	i2c_struct data_to_send;
	data_to_send.id = HIH61_I2C_ID;
	data_to_send.size_of_send = 1;
	data_to_send.data_to_send[0] = 0x00;
	data_to_send.size_of_received = 4;
	data_to_send.read_notwrite = I2C_READ;
	xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);
}

void HIHTaskCalculate(void *pvParameters) {
	i2c_struct data_to_send;
	portBASE_TYPE xStatus;
	uint16_t humCount;
	uint16_t temCount;

	while (1) {
		xStatus = xQueuePeek(xRead_I2CQueue, &data_to_send, portMAX_DELAY);
		if(xStatus && HIH61_I2C_ID == data_to_send.id) {
			xQueueReceive(xRead_I2CQueue, &data_to_send, portMAX_DELAY);
			if(xStatus) {
				humCount = 0x0000 | ((data_to_send.data_received[0] & 0x3F) << 8) | data_to_send.data_received[1];
				temCount = 0x0000 | ((data_to_send.data_received[2] & 0x3F) << 6) | data_to_send.data_received[3] >> 2;
				Humidity = 1000*humCount/16382;
				Humidity_bcd = ((Humidity/10000) << 16) | (((Humidity%10000)/1000) << 12) | ((((Humidity%10000)%1000)/100) << 8) | (((((Humidity%10000)%1000)%100)/10) << 4) | ((((Humidity%10000)%1000)%100)%10);
				sprintf(Humidity_str, "%3d.%1d",Humidity/10,Humidity%10);
				Temperature = (16500*temCount/16382) - 40;
				if(Temperature < 0) {
					Temperature *= -1;
					Temperature_bcd = ((Temperature/10000) << 16) | (((Temperature%10000)/1000) << 12) | ((((Temperature%10000)%1000)/100) << 8) | (((((Temperature%10000)%1000)%100)/10) << 4) | ((((Temperature%10000)%1000)%100)%10);
					Temperature_bcd = ~Temperature_bcd;
					Temperature *= -1;
				} else {
					Temperature_bcd = ((Temperature/10000) << 16) | (((Temperature%10000)/1000) << 12) | ((((Temperature%10000)%1000)/100) << 8) | (((((Temperature%10000)%1000)%100)/10) << 4) | ((((Temperature%10000)%1000)%100)%10);
				}
				sprintf(Temperature_str, "%3d.%1d",Temperature/100,abs(Temperature)%100);
			}
		}
	}
}

uint16_t HIHReturnHumidity(void) {
	return Humidity;
}

int16_t HIHReturnTemperature(void) {
	return Temperature;
}

uint16_t HIHReturnHumidity_bcd(void) {
	return Humidity_bcd;
}

uint16_t HIHReturnTemperature_bcd(void) {
	return Temperature_bcd;
}

uint8_t * HIHReturnHumidity_str(void) {
	return Humidity_str;
}

uint8_t * HIHReturnTemperature_str(void) {
	return Temperature_str;
}
