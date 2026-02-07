/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.c
  * @brief   This file provides code for the configuration
  *          of the CAN instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "can.h"

/* USER CODE BEGIN 0 */
#include "I2C_1602.h"
#include "layer_define.h"
#include "rtc.h"

extern union	layer_settings
{
	Layer_SettingTypeDef menber;
	uint8_t buf[27];
}layer_settings;

extern union	layer_status
{
	Layer_StatusTypeDef menber;
	uint8_t buf[4];
}layer_status;

extern union RGB
{
	uint32_t data;
	uint8_t buf[4];
}RGB;

extern RTC_TimeTypeDef sTime;
extern RTC_DateTypeDef sDate;

uint8_t Rx_Data[8];
extern uint8_t CAN_command;
extern uint8_t reflash_flag;
extern uint8_t cabin_occ;

/* 列表模式只接受0x02200000和0x1e000000 */
void CAN1_IDLISTConfig(void)
{
	uint32_t EXTID = 0x02200000;
	uint32_t EXTID1 = 0x1e000000;
	
	CAN_FilterTypeDef CAN_FilterType;
	
	CAN_FilterType.FilterBank=0;
	CAN_FilterType.FilterIdHigh=(EXTID<<3)>>16;
	CAN_FilterType.FilterIdLow=((EXTID<<3)&0xffff)|CAN_ID_EXT;
	CAN_FilterType.FilterMaskIdHigh=0xffff;
	CAN_FilterType.FilterMaskIdLow=0xffff<<3|CAN_ID_EXT;
	CAN_FilterType.FilterFIFOAssignment=CAN_RX_FIFO0;
	CAN_FilterType.FilterMode=CAN_FILTERMODE_IDLIST;
	CAN_FilterType.FilterScale=CAN_FILTERSCALE_32BIT;
	CAN_FilterType.FilterActivation=ENABLE;
	CAN_FilterType.SlaveStartFilterBank=14;
	if(HAL_CAN_ConfigFilter(&hcan,&CAN_FilterType)!=HAL_OK)
	{
		Error_Handler();
	}
	CAN_FilterType.FilterBank=1;
	CAN_FilterType.FilterIdHigh=(EXTID1<<3)>>16;
	CAN_FilterType.FilterIdLow=((EXTID1<<3)&0xffff)|CAN_ID_EXT;
	CAN_FilterType.FilterMaskIdHigh=0xffff;
	CAN_FilterType.FilterMaskIdLow=0xffff<<3|CAN_ID_EXT;
	CAN_FilterType.FilterFIFOAssignment=CAN_RX_FIFO0;
	CAN_FilterType.FilterMode=CAN_FILTERMODE_IDLIST;//列表模式
	CAN_FilterType.FilterScale=CAN_FILTERSCALE_32BIT;
	CAN_FilterType.FilterActivation=ENABLE;
	CAN_FilterType.SlaveStartFilterBank=14;
	if(HAL_CAN_ConfigFilter(&hcan,&CAN_FilterType)!=HAL_OK)
	{
		Error_Handler();
	}
	if(HAL_CAN_ActivateNotification(&hcan,CAN_IT_RX_FIFO0_MSG_PENDING)!=HAL_OK)
	{
		Error_Handler();
	}
	if(HAL_CAN_Start(&hcan)!=HAL_OK)
	{
		Error_Handler();
	}
}
/* 掩码模式全部接收 */
void CAN1_IDMASKConfig(uint8_t layer_num)
{
	CAN_FilterTypeDef CAN_FilterType;
	
	CAN_FilterType.FilterBank=0;
	CAN_FilterType.FilterIdHigh=(0x100+layer_num)<<5;
	CAN_FilterType.FilterIdLow=0x0000;
	CAN_FilterType.FilterMaskIdHigh=0xFFE0;
	CAN_FilterType.FilterMaskIdLow=0x0000;
	CAN_FilterType.FilterFIFOAssignment=CAN_RX_FIFO0;
	CAN_FilterType.FilterMode=CAN_FILTERMODE_IDMASK;//掩码模式
	CAN_FilterType.FilterScale=CAN_FILTERSCALE_32BIT;
	CAN_FilterType.FilterActivation=ENABLE;
	CAN_FilterType.SlaveStartFilterBank=14;
	if(HAL_CAN_ConfigFilter(&hcan,&CAN_FilterType)!=HAL_OK)
	{
		Error_Handler();
	}
	if(HAL_CAN_ActivateNotification(&hcan,CAN_IT_RX_FIFO0_MSG_PENDING)!=HAL_OK)
	{
		Error_Handler();
	}
	if(HAL_CAN_Start(&hcan)!=HAL_OK)
	{
		Error_Handler();
	}
}
/* USER CODE END 0 */

CAN_HandleTypeDef hcan;

/* CAN init function */
void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 18;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_5TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_5TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = ENABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */
	CAN1_IDMASKConfig(1);
  /* USER CODE END CAN_Init 2 */

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN GPIO Configuration
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN GPIO Configuration
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/***********************************************
   函数功能：can发送数据
   入口参数：
   			ide：	0：标准帧
   					1：扩展帧
   			id：	帧ID
   			len：	数据长度
   			data：	数据
   返回值：0：成功。1：失败
************************************************/
uint8_t Can_TxMessage(uint8_t ide,uint32_t id,uint8_t len,uint8_t *data)
{
	uint32_t   TxMailbox;
	CAN_TxHeaderTypeDef CAN_TxHeader;
	HAL_StatusTypeDef   HAL_RetVal; 
	uint16_t i=0;
	if(ide == 0)
	{
		CAN_TxHeader.IDE = CAN_ID_STD;	//标准帧
		CAN_TxHeader.StdId = id;
	}
	else 
	{
		CAN_TxHeader.IDE = CAN_ID_EXT;			//扩展帧
		CAN_TxHeader.ExtId = id;
	}
	CAN_TxHeader.DLC = len;
	CAN_TxHeader.RTR = CAN_RTR_DATA;//数据帧,CAN_RTR_REMOTE遥控帧
	CAN_TxHeader.TransmitGlobalTime = DISABLE;
	while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan) == 0)
	{
		i++;
		if(i>0xfffe)
			return 1;
	}
//	HAL_Delay(50);
	HAL_RetVal = HAL_CAN_AddTxMessage(&hcan,&CAN_TxHeader,data,&TxMailbox);
	if(HAL_RetVal != HAL_OK)
		return 1;
	return 0;
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef CAN_RxHeader;
	HAL_StatusTypeDef HAL_Retval;
	
	uint8_t Data_Len=0;
	uint32_t ID=0;
	
	HAL_Retval = HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&CAN_RxHeader,Rx_Data);
	if(HAL_Retval == HAL_OK)
	{
		Data_Len = CAN_RxHeader.DLC;
		if(CAN_RxHeader.IDE)
			ID = CAN_RxHeader.ExtId;
		else
			ID = CAN_RxHeader.StdId;
		
		CAN_command=Rx_Data[0];
		
		switch(CAN_command)
		{
			case 0:
				layer_settings.buf[0]=Rx_Data[1];
				layer_settings.buf[1]=Rx_Data[4];
				layer_settings.buf[2]=Rx_Data[5];
				layer_settings.buf[3]=Rx_Data[6];
			break;
			
			case 1:
				layer_settings.buf[4]=Rx_Data[1];
				layer_settings.buf[5]=Rx_Data[2];
				layer_settings.buf[6]=Rx_Data[3];
			break;
			
			case 2:
				layer_settings.buf[7]=Rx_Data[1];
				layer_settings.buf[8]=Rx_Data[2];
				layer_settings.buf[9]=Rx_Data[3];
				layer_settings.buf[10]=Rx_Data[4];
			break;
			
			case 3:
				layer_settings.buf[11]=Rx_Data[1];
				layer_settings.buf[12]=Rx_Data[2];
				layer_settings.buf[13]=Rx_Data[3];
				layer_settings.buf[14]=Rx_Data[4];
			break;
			
			case 4:
				layer_settings.buf[15]=Rx_Data[1];
				layer_settings.buf[16]=Rx_Data[2];
				layer_settings.buf[17]=Rx_Data[3];
				layer_settings.buf[18]=Rx_Data[4];
			break;
			
			case 5:
				layer_settings.buf[19]=Rx_Data[1];
				layer_settings.buf[20]=Rx_Data[2];
				layer_settings.buf[21]=Rx_Data[3];
				layer_settings.buf[22]=Rx_Data[4];
			break;
			
			case 6:
				layer_settings.buf[23]=Rx_Data[1];
				layer_settings.buf[24]=Rx_Data[2];
				layer_settings.buf[25]=Rx_Data[3];
				layer_settings.buf[26]=Rx_Data[4];
				RGB.buf[0]=0x00;
			break;
			
			case 7:
			break;
			
			case 8:
				RGB.buf[0]=Rx_Data[1];			
				RGB.buf[1]=Rx_Data[2];			
				RGB.buf[2]=Rx_Data[3];			
				RGB.buf[3]=Rx_Data[4];			
			break;
			
			case 0x80:
				cabin_occ=1;
			break;
			
			case 0x81:
				cabin_occ=0;
			break;
			
			default:break;
		}
		
		
		if(Rx_Data[0]==0x07)
		{
//			lcd_1602_printf(1,1,"%d:%02d",Rx_Data[1],Rx_Data[2]);
//			Can_TxMessage(0,0x001,8,buf);
			if(sTime.Hours!=Rx_Data[1] || sTime.Minutes!=Rx_Data[2] ||
				 sDate.Year!=Rx_Data[3] || sDate.Month!=Rx_Data[4] || sDate.Date!=Rx_Data[5]
			)
			{
				RTC_Set_Time(Rx_Data[1],Rx_Data[2],00);
				RTC_Set_Date(Rx_Data[3],Rx_Data[4],Rx_Data[5],Rx_Data[6]);	
				reflash_flag=1;
			}
		}
		else if(Rx_Data[0]==0x08)
		{
//			lcd_1602_Setbacklight(Rx_Data[2],Rx_Data[3],Rx_Data[4]);
		}
		else 
		{
			
		}	

	}
}
/* USER CODE END 1 */
