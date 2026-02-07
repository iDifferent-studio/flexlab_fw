/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "app_touchgfx.h"
#include "can.h"
#include "layer_define.h"
#include "rtc.h"

#ifdef FT5206_TOUCH_DRIVER
#include "ft5206.h"
#else
#include "gt9147.h"
#endif
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define the_x 0
#define the_y 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern uint8_t Rx_Data[8];
extern uint32_t ID;
extern uint8_t CAN_RX_Flag;
/* USER CODE END Variables */
/* Definitions for guiTask */
osThreadId_t guiTaskHandle;
const osThreadAttr_t guiTask_attributes = {
  .name = "guiTask",
  .stack_size = 4096 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for CANTransmitTask */
osThreadId_t CANTransmitTaskHandle;
const osThreadAttr_t CANTransmitTask_attributes = {
  .name = "CANTransmitTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for getTouchTask */
osThreadId_t getTouchTaskHandle;
const osThreadAttr_t getTouchTask_attributes = {
  .name = "getTouchTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for SettingTxQueue */
osMessageQueueId_t SettingTxQueueHandle;
const osMessageQueueAttr_t SettingTxQueue_attributes = {
  .name = "SettingTxQueue"
};
/* Definitions for LayerNumQueue */
osMessageQueueId_t LayerNumQueueHandle;
const osMessageQueueAttr_t LayerNumQueue_attributes = {
  .name = "LayerNumQueue"
};
/* Definitions for LayerStatusQueue */
osMessageQueueId_t LayerStatusQueueHandle;
const osMessageQueueAttr_t LayerStatusQueue_attributes = {
  .name = "LayerStatusQueue"
};
/* Definitions for LayerBLRGBQueue */
osMessageQueueId_t LayerBLRGBQueueHandle;
const osMessageQueueAttr_t LayerBLRGBQueue_attributes = {
  .name = "LayerBLRGBQueue"
};
/* Definitions for LayerCMDQueue */
osMessageQueueId_t LayerCMDQueueHandle;
const osMessageQueueAttr_t LayerCMDQueue_attributes = {
  .name = "LayerCMDQueue"
};
/* Definitions for touch_posQueue */
osMessageQueueId_t touch_posQueueHandle;
const osMessageQueueAttr_t touch_posQueue_attributes = {
  .name = "touch_posQueue"
};
/* Definitions for CarbinStatusQueue */
osMessageQueueId_t CarbinStatusQueueHandle;
const osMessageQueueAttr_t CarbinStatusQueue_attributes = {
  .name = "CarbinStatusQueue"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void GUITask(void *argument);
void CAN_TransmitTask(void *argument);
void get_touchTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of SettingTxQueue */
  SettingTxQueueHandle = osMessageQueueNew (3, sizeof(Layer_SettingTypeDef), &SettingTxQueue_attributes);

  /* creation of LayerNumQueue */
  LayerNumQueueHandle = osMessageQueueNew (3, sizeof(uint8_t), &LayerNumQueue_attributes);

  /* creation of LayerStatusQueue */
  LayerStatusQueueHandle = osMessageQueueNew (3, sizeof(q_Layer_StatusTypeDef), &LayerStatusQueue_attributes);

  /* creation of LayerBLRGBQueue */
  LayerBLRGBQueueHandle = osMessageQueueNew (3, sizeof(uint32_t), &LayerBLRGBQueue_attributes);

  /* creation of LayerCMDQueue */
  LayerCMDQueueHandle = osMessageQueueNew (3, sizeof(uint32_t), &LayerCMDQueue_attributes);

  /* creation of touch_posQueue */
  touch_posQueueHandle = osMessageQueueNew (1, sizeof(uint32_t), &touch_posQueue_attributes);

  /* creation of CarbinStatusQueue */
  CarbinStatusQueueHandle = osMessageQueueNew (3, sizeof(q_Carbin_StatusTypeDef), &CarbinStatusQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of guiTask */
  guiTaskHandle = osThreadNew(GUITask, NULL, &guiTask_attributes);

  /* creation of CANTransmitTask */
  CANTransmitTaskHandle = osThreadNew(CAN_TransmitTask, NULL, &CANTransmitTask_attributes);

  /* creation of getTouchTask */
  getTouchTaskHandle = osThreadNew(get_touchTask, NULL, &getTouchTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_GUITask */
/**
  * @brief  Function implementing the guiTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_GUITask */
void GUITask(void *argument)
{
  /* USER CODE BEGIN GUITask */
  /* Infinite loop */
  for(;;)
  {
    MX_TouchGFX_Process();
    osDelay(5);
  }
  /* USER CODE END GUITask */
}

/* USER CODE BEGIN Header_CAN_TransmitTask */
/**
* @brief Function implementing the CANTransmitTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CAN_TransmitTask */
void CAN_TransmitTask(void *argument)
{
  /* USER CODE BEGIN CAN_TransmitTask */
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	uint8_t layer_num;
	
	union	layer_settings
	{
		Layer_SettingTypeDef menber;
		uint8_t buf[27];
	}layer_settings;
	
	union	layer_status
	{
		q_Layer_StatusTypeDef menber;
		uint8_t buf[5];
	}layer_status;
	
	union	carbin_status
	{
		q_Carbin_StatusTypeDef menber;
		uint8_t buf[2];
	}carbin_status;
	
	union RGB
	{
		uint32_t data;
		uint8_t buf[4];
	}RGB;
	
	union CMD
	{
		uint32_t data;
		uint8_t buf[4];
	}CMD;
	
	uint8_t buf[7][8];
  /* Infinite loop */
  for(;;)
  {
		if(CAN_RX_Flag)
		{
			if(Rx_Data[0]==0x07)
			{
				for(uint8_t i=0;i<4;i++)
					layer_status.buf[i]=Rx_Data[1+i];
				
				layer_status.buf[4]=ID;
				const q_Layer_StatusTypeDef q_Layer_Status=layer_status.menber;
				osMessageQueuePut(LayerStatusQueueHandle, &q_Layer_Status, 1, 0);
			}
			else if(Rx_Data[0]==0x82)
			{
				for(uint8_t i=0;i<2;i++)
					carbin_status.buf[i]=Rx_Data[1+i];
				
				const q_Carbin_StatusTypeDef q_Carbin_Status=carbin_status.menber;
				osMessageQueuePut(CarbinStatusQueueHandle, &q_Carbin_Status, 1, 0);
			}
			
			CAN_RX_Flag=0;
		}
		else if(osMessageQueueGetSpace(SettingTxQueueHandle)!=3&&
						osMessageQueueGetSpace(LayerNumQueueHandle)!=3
					 )
		{
			osMessageQueueGet(SettingTxQueueHandle, &layer_settings.menber, (uint8_t *)1, 50);
			osMessageQueueGet(LayerNumQueueHandle, &layer_num, (uint8_t *)1, 50);
			
			for(uint8_t i=0;i<7;i++)
			{
				buf[i][0]=i;
				switch(i)
				{
					case 0:
						buf[i][1]=layer_settings.buf[0];
						buf[i][2]=sTime.Hours;
						buf[i][3]=sTime.Minutes;
						buf[i][4]=layer_settings.buf[1];
						buf[i][5]=layer_settings.buf[2];
						buf[i][6]=layer_settings.buf[3];
					break;
					
					case 1:
						buf[i][1]=layer_settings.buf[4];
						buf[i][2]=layer_settings.buf[5];
						buf[i][3]=layer_settings.buf[6];
					break;
					
					case 2:
						buf[i][1]=layer_settings.buf[7];
						buf[i][2]=layer_settings.buf[8];
						buf[i][3]=layer_settings.buf[9];
						buf[i][4]=layer_settings.buf[10];
					break;
					
					case 3:
						buf[i][1]=layer_settings.buf[11];
						buf[i][2]=layer_settings.buf[12];
						buf[i][3]=layer_settings.buf[13];
						buf[i][4]=layer_settings.buf[14];
					break;
					
					case 4:
						buf[i][1]=layer_settings.buf[15];
						buf[i][2]=layer_settings.buf[16];
						buf[i][3]=layer_settings.buf[17];
						buf[i][4]=layer_settings.buf[18];
					break;
					
					case 5:
						buf[i][1]=layer_settings.buf[19];
						buf[i][2]=layer_settings.buf[20];
						buf[i][3]=layer_settings.buf[21];
						buf[i][4]=layer_settings.buf[22];
					break;
					
					case 6:
						buf[i][1]=layer_settings.buf[23];
						buf[i][2]=layer_settings.buf[24];
						buf[i][3]=layer_settings.buf[25];
						buf[i][4]=layer_settings.buf[26];
					break;
					
					default:break;
				}
				Can_TxMessage(0,0x100+layer_num,8,buf[i]);
			}
		}
		else if(osMessageQueueGetSpace(LayerBLRGBQueueHandle)!=3&&
						osMessageQueueGetSpace(LayerNumQueueHandle)!=3
					 )
		{
			osMessageQueueGet(LayerBLRGBQueueHandle, &RGB.data, (uint8_t *)1, 50);
			osMessageQueueGet(LayerNumQueueHandle, &layer_num, (uint8_t *)1, 50);
			
			buf[0][0]=0x08;
			buf[0][1]=RGB.buf[0];
			buf[0][2]=RGB.buf[1];
			buf[0][3]=RGB.buf[2];
			buf[0][4]=RGB.buf[3];
			
			Can_TxMessage(0,0x100+layer_num,8,buf[0]);
		}
		else if(osMessageQueueGetSpace(LayerCMDQueueHandle)!=3&&
						osMessageQueueGetSpace(LayerNumQueueHandle)!=3
					 )
		{
			osMessageQueueGet(LayerCMDQueueHandle, &CMD.data, (uint8_t *)1, 50);
			osMessageQueueGet(LayerNumQueueHandle, &layer_num, (uint8_t *)1, 50);
			
			buf[0][0]=0x09;
			buf[0][1]=CMD.buf[0];
			buf[0][2]=CMD.buf[1];
			buf[0][3]=CMD.buf[2];
			buf[0][4]=CMD.buf[3];
			
			Can_TxMessage(0,0x100+layer_num,8,buf[0]);
		}
		else
		{
			if(osMessageQueueGetSpace(LayerStatusQueueHandle)==3)
			{
				static uint8_t i=1;
				HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
				HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
				buf[0][0]=0x07;
				buf[0][1]=sTime.Hours;
				buf[0][2]=sTime.Minutes;
				buf[0][3]=sDate.Year;
				buf[0][4]=sDate.Month;
				buf[0][5]=sDate.Date;
//				for(uint8_t i=1;i<=3;i++)   //layer state report cmd
				Can_TxMessage(0,0x100+i,8,buf[0]);
				i++;
				if(i>3)
				{
					i=1;
					buf[0][0]=0x02;
					Can_TxMessage(0,0x200,8,buf[0]);
				}
			}
		}

    osDelay(5);
  }
  /* USER CODE END CAN_TransmitTask */
}

/* USER CODE BEGIN Header_get_touchTask */
/**
* @brief Function implementing the getTouchTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_get_touchTask */
void get_touchTask(void *argument)
{
  /* USER CODE BEGIN get_touchTask */
  /* Infinite loop */
	union t_pos
	{
		uint32_t data;
		uint16_t buf[2];
	}t_pos;
  for(;;)
  {
#ifdef FT5206_TOUCH_DRIVER
		if(get_touch(&t_pos.buf[the_x],&t_pos.buf[the_y])==1)
		{
#else
		if(gt_get_touch(&t_pos.buf[the_x],&t_pos.buf[the_y])==1)
		{
#endif
			osMessageQueuePut(touch_posQueueHandle, &t_pos.data, 1, 1);
		}
			osDelay(5);
  }
  /* USER CODE END get_touchTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

