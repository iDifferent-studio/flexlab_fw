#include "key.h"
#include "gpio.h"

uint8_t KEY_Scan(uint8_t mode)
{
	static uint8_t key_up=1;     //按键松开标志
	if(mode==1)key_up=1;    //支持连按
	if(key_up&&(KEY1==0||KEY1==0||KEY2==0))
	{
		HAL_Delay(5);
		key_up=0;
		if(KEY1==0)       return KEY1_PRES;
		else if(KEY1==0)  return KEY1_PRES;
		else if(KEY2==0)  return KEY2_PRES;        
	}
	else if(KEY1==1&&KEY1==1&&KEY2==1)key_up=1;
	return 0;   //无按键按下
}
