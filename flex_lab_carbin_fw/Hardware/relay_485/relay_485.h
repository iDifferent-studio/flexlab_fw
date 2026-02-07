#ifndef _relay_485
#define _relay_485

#include "stm32f1xx_hal.h"

void relay_485_init(void);
void relay_485_OnOff(uint8_t sw_num,uint8_t state,uint8_t dev_addr);

#endif
