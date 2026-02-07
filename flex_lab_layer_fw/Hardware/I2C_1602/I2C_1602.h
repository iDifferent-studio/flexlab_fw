#ifndef _I2C_1602
#define _I2C_1602

#include "stm32f1xx_hal.h"

#define MODE1 0x00	   	 // PCA9633 Mode register 1
#define MODE2 0x01		   // PCA9633 Mode register 2
#define PWM0 0x02		     // PCA9633 brightness control LED0
#define PWM1 0x03		     // PCA9633 brightness control LED1
#define PWM2 0x04		     // PCA9633 brightness control LED2
#define PWM3 0x05		     // PCA9633 brightness control LED3
#define GRPPWM 0x06		   // PCA9633 group duty cycle control
#define GRPFREQ 0x07	   // PCA9633 group frequency
#define LEDOUT  0x08	   // PCA9633 LED output state

#define SUBADR1 0x09	   //PCA9633 I2C-bus subaddress 1
#define SUBADR2 0x0A	   //PCA9633 I2C-bus subaddress 2
#define SUBADR3 0x0B	   //PCA9633 I2C-bus subaddress 3
#define ALLCALLADR 0x0C	 //PCA9633 LED All Call I2C-bus address

//#define LCD_SCLK_H()  HAL_GPIO_WritePin(LCD_SCLK_GPIO_Port, LCD_SCLK_Pin, GPIO_PIN_SET)		  // SCL = 1 
//#define LCD_SCLK_L()  HAL_GPIO_WritePin(LCD_SCLK_GPIO_Port, LCD_SCLK_Pin, GPIO_PIN_RESET)		// SCL = 0 
//	
//#define LCD_SDIN_H()  HAL_GPIO_WritePin(LCD_SDIN_GPIO_Port, LCD_SDIN_Pin, GPIO_PIN_SET)			 // SDA = 1 
//#define LCD_SDIN_L()  HAL_GPIO_WritePin(LCD_SDIN_GPIO_Port, LCD_SDIN_Pin, GPIO_PIN_RESET)		// SDA = 0 

void lcd_1602_Setbacklight(uint8_t red,uint8_t green,uint8_t blue);
void lcd_1602_init(void);
void lcd_1602_DispOneChar(uint8_t x,uint8_t y,uint8_t Wdata);
void lcd_1602_printf(uint8_t x,uint8_t y,char* fmt,...);

#endif
