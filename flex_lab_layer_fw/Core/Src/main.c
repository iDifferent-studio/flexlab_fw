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
#include "i2c.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "I2C_1602.h"
#include "relay_485.h"
#include "key.h"
#include "ds18b20.h"
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
uint8_t cabin_occ=0;
extern uint8_t Rx_Data[8];
uint8_t Tx_Data[8];

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

union RGB
{
	uint32_t data;
	uint8_t buf[4];
}RGB;

RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;

uint8_t minute_mark=0, hour_mark=0;
int32_t vent_lim=-1;
int32_t cyc_lim=-1;
uint8_t water_change_state=0;

uint8_t key;
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
	if(relay_state[num-1]!=state)
	{
		relay_state[num-1]=state;
		if(num>3)
			relay_485_OnOff(num-3,state,0xf1);
		else
			relay_485_OnOff(num,state,0xf0);
		
		HAL_Delay(100);
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	static uint8_t test=0x01;
	uint8_t relay_num=1;
	uint8_t t=0,p_min;
	int32_t vent_times=0;
	int32_t cyc_times=0;
	uint8_t p_Layerstate=0;
	uint32_t nus_vol_buf=0;
	
	static uint8_t level_sensor1=1,level_sensor2=0,level_sensor3=0;//debug
	static uint32_t tt_debug=0;//debug
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
  MX_USART1_UART_Init();
  MX_CAN_Init();
  MX_TIM1_Init();
  MX_I2C2_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
	lcd_1602_init();
	relay_485_init();
	HAL_TIM_Base_Start_IT(&htim1);
	
	DS18B20_Init();
	
	lcd_1602_Setbacklight(100,100,0);
	if(relay_state[relay_num-1])
		lcd_1602_printf(1,0,"relay %d off ",relay_num);
	else
		lcd_1602_printf(1,0,"relay %d on",relay_num);
	
	layer_status.member.layer_state=0x03;
	
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	p_min=sTime.Minutes;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
/*------------------------------------------------------------------*/
		switch(layer_status.member.layer_state)//state control
		{
			case 0:		//state wait	
			case 1:   //state vent
				if(layer_settings.member.light_status==0)//light control
				{
					set_relay(3,1);
					layer_status.member.light_status=0;
				}
				else if(layer_settings.member.light_status==1)
				{
					set_relay(3,0);
					layer_status.member.light_status=1;
				}
				else
				{
					if(layer_settings.member.light_ontime>layer_settings.member.light_offtime)
					{
						if(sTime.Hours>=layer_settings.member.light_offtime && sTime.Hours<layer_settings.member.light_ontime)
						{	
							set_relay(3,1);
							layer_status.member.light_status=0;
						}
						else
						{
							set_relay(3,0);
							layer_status.member.light_status=1;
						}
					}
					else if(layer_settings.member.light_ontime<layer_settings.member.light_offtime)
					{
						if(sTime.Hours<layer_settings.member.light_offtime && sTime.Hours>=layer_settings.member.light_ontime)
						{
							set_relay(3,0);
							layer_status.member.light_status=1;
						}
						else
						{	
							set_relay(3,1);
							layer_status.member.light_status=0;
						}
					}
					else
						set_relay(3,0);
				}
				
				if(layer_settings.member.vent_mode==0)//vent control
				{
					set_relay(6,1);
					layer_status.member.layer_state=0;
				}
				else if(layer_settings.member.vent_mode==1)
				{
					set_relay(6,0);
					layer_status.member.layer_state=1;
				}
				else
				{
					if(vent_times<=0)
					{
						set_relay(6,0);
						layer_status.member.layer_state=1;
					}
					else
					{
						set_relay(6,1);
						layer_status.member.layer_state=0;
					}
				}
				
				if(layer_settings.member.cycle_schedule!=0)//cycle control
				{
					if(cyc_times<=0)
					{
						set_relay(4,0);
						set_relay(5,0);
						layer_status.member.cycle_status=1;
					}
					else
					{
						set_relay(4,1);
						set_relay(5,1);
						layer_status.member.cycle_status=0;
					}
				}
				else
				{
					set_relay(4,1);
					set_relay(5,1);
					layer_status.member.cycle_status=0;
				}
			
				if(layer_settings.member.enable_status==0)
				{
					layer_status.member.layer_state=3;
				}
				
			break;
			
			case 2:  //state water change
			
				if(water_change_state!=0)
				{
					if(water_change_state<=4)
					{
						HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_RESET); //dosing pump enable
						HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);

						if(nus_vol_buf==0)
							water_change_state--;
					}
					else
					{						
						static uint8_t updown_state=0;
						
						switch(tt_debug)//debug
						{
							case 0:
								level_sensor1=1;level_sensor2=0;level_sensor3=0;
							break;
							
							case 2000:
								level_sensor1=0;level_sensor2=0;level_sensor3=0;
							break;
							
							case 4000:
								level_sensor1=1;level_sensor2=0;level_sensor3=0;
							break;
							
							case 8000:
								level_sensor1=1;level_sensor2=1;level_sensor3=0;
							break;
							
							case 10000:
								level_sensor1=1;level_sensor2=0;level_sensor3=0;
							break;
							
							case 12000:
								level_sensor1=0;level_sensor2=0;level_sensor3=0;
							break;
							
							case 14000:
								level_sensor1=1;level_sensor2=0;level_sensor3=0;
							break;
							
							case 16000:
								level_sensor1=1;level_sensor2=1;level_sensor3=0;
							break;
							
							case 18000:
								level_sensor1=1;level_sensor2=0;level_sensor3=0;
							break;
							
							case 20000:
								level_sensor1=1;level_sensor2=0;level_sensor3=1;
								tt_debug=0;
						}
						tt_debug++;
						
						switch(updown_state)
						{
							case 0://water down
								set_relay(1,1);//valve open out
								set_relay(2,0);
								set_relay(4,1);//cyc pump out
								set_relay(5,0);
								water_change_state=5;//pump down
								if(level_sensor1==0 && level_sensor2==0)//buffer empty
									updown_state=1;
							break;
							
							case 1://water up
								set_relay(1,0);//valve open in
								set_relay(2,1);
								set_relay(4,1);//cyc pump off
								set_relay(5,1);
								water_change_state=6;//pump up
								if(level_sensor1==1 && level_sensor2==1)//buffer full
									updown_state=2;
							break;
							
							case 2://fill piple
								set_relay(1,0);//valve open in
								set_relay(2,1);
								set_relay(4,0);//cyc pump in
								set_relay(5,1);
								water_change_state=7;//pump stop
								if(level_sensor1==0 && level_sensor2==0)//buffer empty
									updown_state=1;
								else if(level_sensor3==1)//piple full
								{
									set_relay(1,1);//valve off
									set_relay(2,1);
									set_relay(4,0);//cyc 
									set_relay(5,0);
									water_change_state=4;
									updown_state=0;
								}
								
							break;
						}
					}
					
					switch(water_change_state)
					{
						case 7://stop
							Tx_Data[0]=0x01;
							Can_TxMessage(0,0x200,8,Tx_Data);
						break;
						
						case 6://up pump
							Tx_Data[0]=0x00;
							Tx_Data[1]=0x01;
							Tx_Data[2]=0xFF;
							Tx_Data[3]=0x10;
							Can_TxMessage(0,0x200,8,Tx_Data);
						break;
						
						case 5://down pump
							Tx_Data[0]=0x00;
							Tx_Data[1]=0x01;
							Tx_Data[2]=0xFF;
							Tx_Data[3]=0x20;
							Can_TxMessage(0,0x200,8,Tx_Data);
						break;
						
						case 4:
							Tx_Data[0]=0x00;
							Tx_Data[1]=0x01;
							Tx_Data[2]=0xFF;
							Tx_Data[3]=0x01;
							Can_TxMessage(0,0x200,8,Tx_Data);
						break;
						
						case 3:
							if(nus_vol_buf==0)
							{
								if(layer_settings.member.nus1_volume==0)
									break;
								nus_vol_buf=layer_settings.member.nus1_volume;
							}
								
							Tx_Data[0]=0x00;
							Tx_Data[1]=0x01;
							Tx_Data[2]=0xFF;
							Tx_Data[3]=0x02;
							Can_TxMessage(0,0x200,8,Tx_Data);
						break;
						
						case 2:
							if(nus_vol_buf==0)
							{
								if(layer_settings.member.nus2_volume==0)
									break;
								nus_vol_buf=layer_settings.member.nus2_volume;
							}

							Tx_Data[0]=0x00;
							Tx_Data[1]=0x01;
							Tx_Data[2]=0xFF;
							Tx_Data[3]=0x04;
							Can_TxMessage(0,0x200,8,Tx_Data);
						break;
						
						case 1:
							if(nus_vol_buf==0)
							{
								if(layer_settings.member.nus3_volume==0)
									break;
								nus_vol_buf=layer_settings.member.nus3_volume;
							}

							Tx_Data[0]=0x00;
							Tx_Data[1]=0x01;
							Tx_Data[2]=0xFF;
							Tx_Data[3]=0x08;
							Can_TxMessage(0,0x200,8,Tx_Data);
						break;
					}

					if(nus_vol_buf!=0 && water_change_state<=4)
						nus_vol_buf--;
				}
				else
				{
					Tx_Data[0]=0x01;
					Can_TxMessage(0,0x200,8,Tx_Data);
					
					HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_SET); //dosing pump disable
					HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_4);
					
					layer_status.member.layer_state=p_Layerstate;
				}
			break;
			
			case 3:  //state disable
				if(layer_settings.member.enable_status==1)
				{
					layer_status.member.layer_state=0;
				}
			break;
			
			default:
				layer_status.member.layer_state=3;
			break;
		}
/*------------------------------------------------------------------*/		
		switch(key)//process key input
		{
			case 1:
				relay_num++;
				if(relay_num>6)
					relay_num=1;
			break;
			
			case 2:
				if(relay_state[relay_num-1]==0)
					relay_state[relay_num-1]=1;
				else
					relay_state[relay_num-1]=0;
				
				if(relay_num<=3)
					relay_485_OnOff(relay_num,relay_state[relay_num-1],0xf0);
				else
					relay_485_OnOff(relay_num-3,relay_state[relay_num-1],0xf1);
			break;
			
			case 3:
				water_change_state=0;
			break;
			
			default:break;
		}

		if(key)
		{
			if(relay_state[relay_num-1])
				lcd_1602_printf(1,0,"relay %d off ",relay_num);
			else
				lcd_1602_printf(1,0,"relay %d on ",relay_num);
			key=0;
		}
/*------------------------------------------------------------------*/	
		if(CAN_command==0x07)//temp report command
		{
			layer_status.member.temp=DS18B20_Get_Temp()/10;
			Tx_Data[0]=0x07;
			Tx_Data[1]=layer_status.member.temp;
			Tx_Data[2]=layer_status.member.layer_state;
			Tx_Data[3]=layer_status.member.light_status;
			Tx_Data[4]=layer_status.member.cycle_status;
			
			if(relay_state[2]==0)
				layer_status.member.light_status=1;
			else
				layer_status.member.light_status=0;
			
			
			Can_TxMessage(0,0x001,8,Tx_Data);
			CAN_command=0x00;
			
//			layer_status.member.temp++;//debug use
//			if(layer_status.member.temp>100)
//				layer_status.member.temp=0;
		}
		else if(CAN_command==0x09 && cabin_occ==0 && water_change_state==0)//water change command
		{
			water_change_state=7;
			set_relay(1,0);
			set_relay(2,1);
			set_relay(4,1);//cyc
			set_relay(5,0);
			nus_vol_buf=10000;
			p_Layerstate=layer_status.member.layer_state;
			layer_status.member.layer_state=2;
			
			CAN_command=0x00;
		}
/*------------------------------------------------------------------*/		
		if(RGB.buf[0]==0x01)//backlight control
		{
			lcd_1602_Setbacklight(RGB.buf[3],RGB.buf[2],RGB.buf[1]);
		}
		else
		{
			if(layer_status.member.layer_state==0)
				lcd_1602_Setbacklight(0,100,0);
			else if(layer_status.member.layer_state==1)
				lcd_1602_Setbacklight(0,100,0);
			else if(layer_status.member.layer_state==2)
				lcd_1602_Setbacklight(0,0,100);
			else if(layer_status.member.layer_state==3)
				lcd_1602_Setbacklight(100,100,0);
		}
/*------------------------------------------------------------------*/		
		if(hour_mark==1)//timing service
		{
			if(layer_settings.member.vent_schedule_hour_flag==1 && layer_settings.member.vent_mode==2)//vent contdown in hour
			{
				vent_times--;
				if((vent_times*60)<vent_lim)
					vent_times=layer_settings.member.vent_schedule;
			}
			
			if(layer_settings.member.cycle_schedule_hour_flag==1 && layer_settings.member.cycle_schedule!=0)//cycle contdown in hour
			{
				cyc_times--;
				if((cyc_times*60)<cyc_lim)
					cyc_times=layer_settings.member.cycle_schedule;
			}
			
			hour_mark=0;
		}
		
		if(minute_mark==1)
		{
			if(layer_settings.member.vent_schedule_hour_flag==0 && layer_settings.member.vent_mode==2)//vent contdown in minute
			{
				vent_times--;
				if((vent_times)<vent_lim)
					vent_times=layer_settings.member.vent_schedule;
			}
			
			if(layer_settings.member.cycle_schedule_hour_flag==0 && layer_settings.member.cycle_schedule!=0)//cycle contdown in minute
			{
				cyc_times--;
				if((cyc_times)<cyc_lim)
					cyc_times=layer_settings.member.cycle_schedule;
			}

			minute_mark=0;
		}
/*------------------------------------------------------------------*/
		if(t%500==0)//time display
		{
			HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
			HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
			if(p_min!=sTime.Minutes||reflash_flag==1)
			{
				if(sTime.Hours>9)
					lcd_1602_printf(0,1,"20%02d-%d-%d %d:%02d",sDate.Year, sDate.Month, sDate.Date, sTime.Hours, sTime.Minutes);
				else
					lcd_1602_printf(0,1,"20%02d-%d-%d %d:%02d ",sDate.Year, sDate.Month, sDate.Date, sTime.Hours, sTime.Minutes);
				p_min=sTime.Minutes;
			}
		}
/*------------------------------------------------------------------*/		
		t++;
		HAL_Delay(1);
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
	static uint8_t i=0;
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

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{	
		HAL_Delay(10);

		if(KEY1==0)       key=1;
		else if(KEY2==0)  key=2;
		else if(KEY3==0)  key=3;        

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

