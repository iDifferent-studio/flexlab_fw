#include "relay_485.h"
#include "usart.h"
#include "gpio.h"

void relay_485_init(void)
{
	uint8_t re=0x5a;
	HAL_GPIO_WritePin(DE_GPIO_Port, DE_Pin, GPIO_PIN_SET);
	HAL_UART_Transmit(&huart1, &re, 1, 1000);
}

void relay_485_OnOff(uint8_t sw_num,uint8_t state,uint8_t dev_addr)
{
	uint8_t data[3];
	data[0]=sw_num;
	data[1]=state;
	data[2]=dev_addr;
	HAL_GPIO_WritePin(DE_GPIO_Port, DE_Pin, GPIO_PIN_SET);
	HAL_UART_Transmit(&huart1, data, 3, 1000);
}
