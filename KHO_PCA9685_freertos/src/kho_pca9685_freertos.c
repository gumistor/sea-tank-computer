/*
 * kho_pca9685_freertos.c
 *
 *  Created on: 2 sie 2014
 *      Author: Krzysztof
 */

#include "kho_pca9685_freertos.h"

void PCA_Init(void)
{
	uint8_t dataToSend[5];
	dataToSend[0] = PCA_MODE1_ADDR;
	dataToSend[1] = PCA_MODE1 | PCA_SLEEP_MODE;
	Chip_I2C_MasterSend(I2C_ID, PCA_I2C_ID, dataToSend, 2);
	dataToSend[0] = PCA_MODE2_ADDR;
	dataToSend[1] = PCA_MODE2;
	Chip_I2C_MasterSend(I2C_ID, PCA_I2C_ID, dataToSend, 2);
	dataToSend[0] = PCA_PRE_SCALE_ADDR;
	dataToSend[1] = PCA_PRE_SCALE;
	Chip_I2C_MasterSend(I2C_ID, PCA_I2C_ID, dataToSend, 2);
	dataToSend[0] = PCA_MODE1_ADDR;
	dataToSend[1] = PCA_MODE1;
	Chip_I2C_MasterSend(I2C_ID, PCA_I2C_ID, dataToSend, 2);

	dataToSend[0] =0x06;
	dataToSend[1] =0x00;
	Chip_I2C_MasterSend(I2C_ID, PCA_I2C_ID, dataToSend, 2);
	dataToSend[0] =0x07;
	dataToSend[1] =0x00;
	Chip_I2C_MasterSend(I2C_ID, PCA_I2C_ID, dataToSend, 2);
	dataToSend[0] =0x08;
	dataToSend[1] = (ledOutput[0]) & 0xFF;
	Chip_I2C_MasterSend(I2C_ID, PCA_I2C_ID, dataToSend, 2);
	dataToSend[0] =0x09;
	dataToSend[1] = (ledOutput[0] >> 8) & 0xFF;
	Chip_I2C_MasterSend(I2C_ID, PCA_I2C_ID, dataToSend, 2);
	dataToSend[0] =0x0A;
	dataToSend[1] =0x00;
	Chip_I2C_MasterSend(I2C_ID, PCA_I2C_ID, dataToSend, 2);
	dataToSend[0] =0x0B;
	dataToSend[1] =0x00;
	Chip_I2C_MasterSend(I2C_ID, PCA_I2C_ID, dataToSend, 2);
	dataToSend[0] =0x0C;
	dataToSend[1] = (ledOutput[1]) & 0xFF;
	Chip_I2C_MasterSend(I2C_ID, PCA_I2C_ID, dataToSend, 2);
	dataToSend[0] =0x0D;
	dataToSend[1] = (ledOutput[1] >> 8) & 0xFF;
	Chip_I2C_MasterSend(I2C_ID, PCA_I2C_ID, dataToSend, 2);
	dataToSend[0] =0x0E;
	dataToSend[1] =0x00;
	Chip_I2C_MasterSend(I2C_ID, PCA_I2C_ID, dataToSend, 2);
	dataToSend[0] =0x0F;
	dataToSend[1] =0x00;
	Chip_I2C_MasterSend(I2C_ID, PCA_I2C_ID, dataToSend, 2);
	dataToSend[0] =0x10;
	dataToSend[1] = (ledOutput[2]) & 0xFF;
	Chip_I2C_MasterSend(I2C_ID, PCA_I2C_ID, dataToSend, 2);
	dataToSend[0] =0x11;
	dataToSend[1] = (ledOutput[2] >> 8) & 0xFF;
	Chip_I2C_MasterSend(I2C_ID, PCA_I2C_ID, dataToSend, 2);
}

void SendNewPWM(uint8_t LEDno) {
	i2c_struct data_to_send;

	data_to_send.id = PCA_I2C_ID;
	data_to_send.data_to_send[0] = 0x08 + (LEDno << 2);
	data_to_send.data_to_send[1] = ledOutput[LEDno] & 0xFF;
	data_to_send.size_of_send = 2;
	data_to_send.size_of_received = 0;
	data_to_send.read_notwrite = I2C_WRITE;
	xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);
	data_to_send.data_to_send[0] = 0x09 + (LEDno << 2);
	data_to_send.data_to_send[1] = (ledOutput[LEDno] >> 8) & 0xFF;
	data_to_send.size_of_send = 2;
	data_to_send.size_of_received = 0;
	data_to_send.read_notwrite = I2C_WRITE;
	xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);
}

void SendOneNewPWM(uint8_t LEDno, uint16_t valueToSend) {
	i2c_struct data_to_send;

	data_to_send.id = PCA_I2C_ID;
	data_to_send.data_to_send[0] = 0x08 + (LEDno << 2);
	data_to_send.data_to_send[1] = (valueToSend << 2 )& 0xFF;
	data_to_send.size_of_send = 2;
	data_to_send.size_of_received = 0;
	data_to_send.read_notwrite = I2C_WRITE;
	xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);
	data_to_send.data_to_send[0] = 0x09 + (LEDno << 2);
	data_to_send.data_to_send[1] = (valueToSend >> 6) & 0xFF;
	data_to_send.size_of_send = 2;
	data_to_send.size_of_received = 0;
	data_to_send.read_notwrite = I2C_WRITE;
	xQueueSendToBack(xWrite_I2CQueue, &data_to_send, 0);
}

void ChangeOnePWMBy(uint8_t LEDno, int8_t value) {

	ledOutput_percent[LEDno] = ledOutput_percent[LEDno] + value;
	ledOutput[LEDno] = ledOutput[LEDno] + (value * 41);

	if(ledOutput[LEDno] < 0) {
		ledOutput_percent[LEDno] = 0;
		ledOutput[LEDno] = 0;
	}
	if(ledOutput[LEDno] > 4095) {
		ledOutput_percent[LEDno] = 100;
		ledOutput[LEDno] = 4095;
	}
	SendNewPWM(LEDno);
}

void ChangePWMBy(uint8_t LEDno, int8_t value) {

	ledOutput_percent[LEDno] = ledOutput_percent[LEDno] + value;
	ledOutput[LEDno] = ledOutput[LEDno] + (value * 41);

	if(ledOutput[LEDno] < 0) {
		ledOutput_percent[LEDno] = 0;
		ledOutput[LEDno] = 0;
	}
	if(ledOutput[LEDno] > 4095) {
		ledOutput_percent[LEDno] = 100;
		ledOutput[LEDno] = 4095;
	}
	SendNewPWM(LEDno);
}

void ChangePWM(uint8_t LEDno, uint8_t value) {

	ledOutput_percent[LEDno] = value;
	ledOutput[LEDno] = (value * 41);

	SendNewPWM(LEDno);
}

void ChangeLED(uint8_t selLed) {
	ledSelected = (0x03 & selLed);
}

uint8_t getCurrentLedSelected(void) {
	return ledSelected;
}

uint8_t getCurrentLedPercent(uint8_t LedRow) {
	return  ledOutput_percent[LedRow];
}
