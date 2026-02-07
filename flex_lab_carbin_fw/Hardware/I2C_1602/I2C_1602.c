#include "I2C_1602.h"
#include "i2c.h"
#include "string.h"	
#include "stdarg.h"		 
#include "stdio.h"

uint8_t LCD1602_PRINTF_Buffer[200];	//usb_printf发送缓冲区

//写指令到LCD
void LCD_command(uint8_t com)
{
	uint8_t buf[]={0x80, com};
	HAL_I2C_Master_Transmit(&hi2c2, 0x7C, buf, 2, 1000);
}
//写数据到LCD
void LCD_data(uint8_t dat)
{
	uint8_t buf[]={0xC0, dat};
	HAL_I2C_Master_Transmit(&hi2c2, 0x7C, buf, 2, 1000);
}

void write9633(uint8_t address2,uint8_t info)
{
	HAL_I2C_Mem_Write(&hi2c2, 0xC0,address2, 1, &info, 1, 10);
}

void lcd_1602_init(void)
{
	LCD_command(0x28); // Function set 
	HAL_Delay(5);
	LCD_command(0x28); // Function set 
	HAL_Delay(5);
	LCD_command(0x28); // Function set 
	HAL_Delay(5);
  LCD_command(0x0c); // Display ON/OFF
	HAL_Delay(5);
  LCD_command(0x01); // Clear display
	HAL_Delay(10);
	LCD_command(0x06); // set display mode
	HAL_Delay(5);
	
	write9633(MODE1,0x00);	   //写模式寄存器1，0xC0为从机地址
	write9633(MODE2,0x22);      //写模式寄存器2，LEDn高阻抗，灯不亮

	write9633(PWM0,0);
	write9633(PWM1,0);
	write9633(PWM2,0);
	write9633(PWM3,0);
	HAL_Delay(10);
	//  write9633(0xc0,GRPPWM,0xa0);     //改变占空因数，值越小，亮的时间越短
	write9633(GRPPWM,0xff);
	write9633(GRPFREQ,0x00);    //控制4个灯的闪烁频率，值越小，闪烁越快

	write9633(LEDOUT,0xff);
	HAL_Delay(10);
}

uint8_t R=0,G=0,B=0;
void lcd_1602_Setbacklight(uint8_t red,uint8_t green,uint8_t blue)
{
	if(R==red && G==green && B==blue)
		return;
	else
	{
		write9633(PWM2,red);          //红光
		write9633(PWM1,green);		   //绿光
		write9633(PWM0,blue);	       //蓝光
		R=red;
		G=green;
		B=blue;
	}
}

static void Disp_XY( uint8_t posx,uint8_t posy)
{ 
    uint8_t temp=0x80+posy*0x40+posx;
//    temp=posx&0x1f;                     //&0x1f; 原为限制的条件
//    posy&=0x1;
//    if ( posy )temp|=0xc0;
//    else temp|=0x80;
    LCD_command(temp);
		HAL_Delay(1);
}

void lcd_1602_DispOneChar(uint8_t x,uint8_t y,uint8_t Wdata)
{   
//    Disp_XY( x, y );            // 定位显示地址
//    LCD_data( Wdata );         // 写字符
	
	uint8_t buf[]={0x80, 0x80+y*0x40+x, 0x40, Wdata};
	HAL_I2C_Master_Transmit(&hi2c2, 0x7C, buf, 4, 1000);
	
//	HAL_Delay(5);
}

void lcd_1602_printf(uint8_t x,uint8_t y,char* fmt,...)  
{  
	uint16_t i,j;
	uint8_t bbx;
	va_list ap;
	va_start(ap,fmt);
	
//	static uint8_t dis_buf[2][16]={"                ","                "};
	
	bbx=x;
	vsprintf((char*)LCD1602_PRINTF_Buffer,fmt,ap);
	va_end(ap);
	i=strlen((const char*)LCD1602_PRINTF_Buffer);//此次发送数据的长度
	Disp_XY( x, y );
	for(j=0;j<i;j++)//循环发送数据
	{
		if(y>1)continue;		
		if(LCD1602_PRINTF_Buffer[j]=='\n')
		{
			y++;
			x=bbx;
			Disp_XY( x, y );
		}
		else
		{
			if(x > 15)
			{
				y++;
				x=bbx;
				Disp_XY( x, y );
			}
			x++;
//			dis_buf[y][x]=LCD1602_PRINTF_Buffer[j];
			LCD_data(LCD1602_PRINTF_Buffer[j]);
			
		}
	}
//	uint8_t s_buf[19];
//	s_buf[0]= 0x80;
//	s_buf[1]= 0x80+0*0x40+0;
//	s_buf[2]= 0x40;
//	for(int ii=0; ii<16; ii++)
//		s_buf[3+ii]= dis_buf[0][ii];
//	HAL_I2C_Master_Transmit(&hi2c2, 0x7C, s_buf, 19, 1000);
//	HAL_Delay(1);
//	
//	s_buf[0]= 0x80;
//	s_buf[1]= 0x80+1*0x40+0;
//	s_buf[2]= 0x40;
//	for(int ii=0; ii<16; ii++)
//		s_buf[3+ii]= dis_buf[1][ii];
//	HAL_I2C_Master_Transmit(&hi2c2, 0x7C, s_buf, 19, 1000);
	HAL_Delay(1);
} 
