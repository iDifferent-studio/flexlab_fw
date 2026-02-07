/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
#include "rtc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "layer_define.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t CAN_command;
extern uint8_t Rx_Data[8];

uint8_t reflash_flag=0;

union	layer_settings
{
	Layer_SettingTypeDef member;
	uint8_t buf[27];
}layer_settings;

union	layer_status
{
	Layer_StatusTypeDef member;
	uint8_t buf[4];
}layer_status;

RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;

uint8_t minute_mark=0, hour_mark=0;
int32_t vent_lim=-1;
int32_t cyc_lim=-1;
uint8_t water_change_state=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t relay_state[]={1,1,1,1,1,1};
void set_relay(uint8_t num,uint8_t state)
{

}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	uint8_t t=0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RTC_Init();
  MX_CAN_Init();
  MX_TIM1_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim1);

	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
/*------------------------------------------------------------------*/			
		if(CAN_command==0x00)
		{
			uint8_t canbuf[8]={0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
				
			switch(Rx_Data[1])
			{
				case 1:
					Can_TxMessage(0,0x102,8,canbuf);
					HAL_Delay(10);
					Can_TxMessage(0,0x103,8,canbuf);
				break;
				
				case 2:
					Can_TxMessage(0,0x101,8,canbuf);
					HAL_Delay(10);
					Can_TxMessage(0,0x103,8,canbuf);
				break;
				
				case 3:
					Can_TxMessage(0,0x101,8,canbuf);
					HAL_Delay(10);
					Can_TxMessage(0,0x102,8,canbuf);
					
				break;
			}
			CAN_command=0x00;
		}
		else if(CAN_command==0x01)
		{
			uint8_t canbuf[8]={0x81,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
				
			switch(Rx_Data[1])
			{
				case 1:
					Can_TxMessage(0,0x102,8,canbuf);
					HAL_Delay(10);
					Can_TxMessage(0,0x103,8,canbuf);
				break;
				
				case 2:
					Can_TxMessage(0,0x101,8,canbuf);
					HAL_Delay(10);
					Can_TxMessage(0,0x103,8,canbuf);
				break;
				
				case 3:
					Can_TxMessage(0,0x101,8,canbuf);
					HAL_Delay(10);
					Can_TxMessage(0,0x102,8,canbuf);
					
				break;
			}
			CAN_command=0x00;
		}
		else if(CAN_command==0x02)
		{
			uint8_t canbuf[8]={0x82,0x0A,0x0F,0x00,0x00,0x00,0x00,0x00};
			uint8_t w_level=0,nus1_level=0,nus2_level=0,nus3_level=0;//debug
			canbuf[1]=25;//temp
			canbuf[2]&=0xF7;canbuf[2]|=w_level<<3;
			canbuf[2]&=0xFB;canbuf[2]|=nus1_level<<2;
			canbuf[2]&=0xFD;canbuf[2]|=nus2_level<<1;
			canbuf[2]&=0xFE;canbuf[2]|=nus3_level<<0;
			
			
			Can_TxMessage(0,0x101,8,canbuf);
			HAL_Delay(10);
			Can_TxMessage(0,0x102,8,canbuf);
			HAL_Delay(10);
			Can_TxMessage(0,0x103,8,canbuf);
			HAL_Delay(10);
			
			Can_TxMessage(0,0x00F,8,canbuf);
			
			CAN_command=0x00;
		}
	
/*------------------------------------------------------------------*/		
		if(hour_mark==1)
		{
			
			hour_mark=0;
		}
		
		if(minute_mark==1)
		{
			
			minute_mark=0;
		}
/*------------------------------------------------------------------*/
		if(t%50==0)
		{
			
		}
		
		t++;
		HAL_Delay(10);
		
/*------------------------------------------------------------------*/
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV2;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL15;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint64_t Tim1_cnt;
	Tim1_cnt++;
	
	if(Tim1_cnt%60==0)
	{
		minute_mark=1;
	}
	
	if(Tim1_cnt==3600)
	{
		hour_mark=1;
		Tim1_cnt=0;
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

