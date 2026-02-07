#ifndef _KEY_H
#define _KEY_H

#include "stm32f1xx_hal.h"

#define KEY1_PRES	1
#define KEY2_PRES	2
#define KEY3_PRES	3

#define KEY1	HAL_GPIO_ReadPin(key1_GPIO_Port,key1_Pin)  //KEY0°´¼üPH3
#define KEY2	HAL_GPIO_ReadPin(key2_GPIO_Port,key2_Pin)  //KEY1°´¼üPH2
#define KEY3	HAL_GPIO_ReadPin(key3_GPIO_Port,key3_Pin) //KEY2°´¼üPC13

uint8_t KEY_Scan(uint8_t mode);

#endif
