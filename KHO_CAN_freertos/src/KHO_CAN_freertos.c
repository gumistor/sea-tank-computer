/*
 * KHO_CAN_freertos.c
 *
 *  Created on: 13 sie 2014
 *      Author: Krzysztof
 */

#include "kho_can_freertos.h"
#include "taskConfigData.h"
#include "task.h"

void CAN_Setup(void)
{
	Chip_CAN_Init(LPC_CAN2, LPC_CANAF, LPC_CANAF_RAM);
	Chip_CAN_SetBitRate(LPC_CAN2, 125000);
	Chip_CAN_EnableInt(LPC_CAN2, CAN_IER_BITMASK);
	NVIC_SetPriority(CAN_IRQn, GLOBAL_IRQ_PRIORITY_CAN);
	Chip_CAN_SetAFMode(LPC_CANAF, CAN_AF_BYBASS_MODE);
	NVIC_EnableIRQ(CAN_IRQn);

	//xQueueCANError = xQueueCreate( 1, sizeof( uint32_t ) );
	xQueueCAN_RxBuffor = xQueueCreate( 5, sizeof(CAN_MSG_T));
	if(NULL == xQueueCAN_RxBuffor)
	{
		//ERROR Queue not created
	}
	else
	{
		/* CAN */
		//xTaskCreate(vSendCANMessage, (signed char *) "vSendCANMessage", configMINIMAL_STACK_SIZE, NULL, PRIORITY_vSendCANMessage,(xTaskHandle *) NULL);
		//xTaskCreate(vCANReceivedMsgTask, (signed char *) "vCANReceivedMsgTask",configMINIMAL_STACK_SIZE, NULL, PRIORITY_vCANReceivedMsgTask, (xTaskHandle *) NULL);
		//xTaskCreate(vCANErrorDetectionTask, (signed char *) "vCANErrDetTask",configMINIMAL_STACK_SIZE, NULL, PRIORITY_vCANErrorDetectionTask,	(xTaskHandle *) NULL);
	}
}

void CAN_IRQHandler(void)
{
	uint32_t IntStatus = 0;
	uint32_t TaskWoken = 0;
	CAN_MSG_T RcvMsgBuf;

	IntStatus = Chip_CAN_GetIntStatus(LPC_CAN2);

	if (IntStatus & CAN_ICR_RI)
	{
		Chip_CAN_Receive(LPC_CAN2, &RcvMsgBuf);
		xQueueSendFromISR(xQueueCAN_RxBuffor, &RcvMsgBuf, 0 );
		//RcvMsgBuf to different function
	}
	else
	{
		if(xQueueCANError)
		{
			//xQueueSendFromISR( xQueueCANError, &IntStatus, &TaskWoken );
			if(TaskWoken)
			{
				//taskYeald();
			}
		}
		//IntStatus to error hadler
	}
}

void PrintCANErrorInfo(uint32_t Status)
{
	if (Status & CAN_ICR_EI) {
		//Chip_UART_SendRB(UART_SELECTION, &txring, "Error Warning!\r\n",16);
	}
	if (Status & CAN_ICR_DOI) {
		//Chip_UART_SendRB(UART_SELECTION, &txring, "Data Overrun!\r\n",15);
	}
	if (Status & CAN_ICR_EPI) {
		//Chip_UART_SendRB(UART_SELECTION, &txring, "Error Passive!\r\n",16);
	}
	if (Status & CAN_ICR_ALI) {
		//Chip_UART_SendRB(UART_SELECTION, &txring, "Arbitration lost in the bit: %d(th)\r\n", CAN_ICR_ALCBIT_VAL(Status));
		//Chip_UART_SendRB(UART_SELECTION, &txring, "Arbitration lost.\r\n", 19);
	}
	if (Status & CAN_ICR_BEI) {

		//Chip_UART_SendRB(UART_SELECTION, &txring, "Bus error !!!\r\n",15);

		if (Status & CAN_ICR_ERRDIR_RECEIVE) {
			//Chip_UART_SendRB(UART_SELECTION, &txring, "\tError Direction: Transmiting\r\n",31);
		}
		else {
			//Chip_UART_SendRB(UART_SELECTION, &txring, "\tError Direction: Receiving\r\n",29);
		}

		//Chip_UART_SendRB(UART_SELECTION, &txring, "\t Error Location.\r\n", 19);
		//Chip_UART_SendRB(UART_SELECTION, &txring, "\t Error Type.\r\n", 15);
	}
}

void vCANReceivedMsgTask(void *pvParameters) {
	CAN_MSG_T Rec_frame;
	uint8_t nixie_values[3];
	while (1)
	{
		// Check if xSemaphoreCANError is available
	    if( xQueueCAN_RxBuffor != NULL )
	    {
	        // Wait till the end of the world for xSemaphoreCANError
	        if( xQueueReceive( xQueueCAN_RxBuffor, & Rec_frame, portMAX_DELAY ) == pdTRUE )
	        {
	        	nixie_values[0] = Rec_frame.Data[0];
	        	nixie_values[1] = Rec_frame.Data[1];
	        	nixie_values[2] = Rec_frame.Data[2];


	        	SendNewNixieValue(nixie_values);
	        	//Chip_UART_SendRB(UART_SELECTION, &txring, "RIRQ.\n\r", 7);
	        	//PrintCANErrorInfo();
	        }
	        else
	        {
	            //nop
	        }
	    }
	}
}

void vSendCANMessage(void *pvParameters) {
	uint8_t i = 0;
	CAN_BUFFER_ID_T   TxBuf;
	CAN_MSG_T SendMsgBuf;

	while (1) {
		SendMsgBuf.ID = 0x666;
		SendMsgBuf.DLC = 4;
		SendMsgBuf.Type = 0;
		SendMsgBuf.Data[0] = 0x31;
		SendMsgBuf.Data[1] = 'Y';
		SendMsgBuf.Data[2] = 0x30 + (i & 0x07);
		SendMsgBuf.Data[3] = 'X';
		i++;
		TxBuf = Chip_CAN_GetFreeTxBuf(LPC_CAN2);
		Chip_CAN_Send(LPC_CAN2, TxBuf, &SendMsgBuf);
		while ((Chip_CAN_GetStatus(LPC_CAN2) & CAN_SR_TCS(TxBuf)) == 0) {}
		//Chip_UART_SendRB(UART_SELECTION, &txring, "CAN msg send.\n\r", 15);

		vTaskDelay(configTICK_RATE_HZ * 4);
	}
}

