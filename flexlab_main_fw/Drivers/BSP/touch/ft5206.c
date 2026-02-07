#include "ft5206.h"
#include "ctiic.h"
#include "string.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//7寸电容触摸屏-FT5206 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/12/28
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
 
static uint16_t t_x[CT_MAX_TOUCH]; 		//当前坐标
static uint16_t t_y[CT_MAX_TOUCH];
static uint8_t sta=0;
//向FT5206写入一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:写数据长度
//返回值:0,成功;1,失败.
uint8_t FT5206_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
	uint8_t i;
	uint8_t ret=0;
	CT_IIC_Start();	 
	CT_IIC_Send_Byte(FT_CMD_WR);	//发送写命令 	 
	CT_IIC_Wait_Ack(); 	 										  		   
	CT_IIC_Send_Byte(reg&0XFF);   	//发送低8位地址
	CT_IIC_Wait_Ack();  
	for(i=0;i<len;i++)
	{	   
    CT_IIC_Send_Byte(buf[i]);  	//发数据
		ret=CT_IIC_Wait_Ack();
		if(ret)break;  
	}
    CT_IIC_Stop();					//产生一个停止条件	    
	return ret; 
}
//从FT5206读出一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:读数据长度			  
void FT5206_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
	uint8_t i; 
 	CT_IIC_Start();	
 	CT_IIC_Send_Byte(FT_CMD_WR);   	//发送写命令 	 
	CT_IIC_Wait_Ack(); 	 										  		   
 	CT_IIC_Send_Byte(reg&0XFF);   	//发送低8位地址
	CT_IIC_Wait_Ack();  
 	CT_IIC_Start();  	 	   
	CT_IIC_Send_Byte(FT_CMD_RD);   	//发送读命令		   
	CT_IIC_Wait_Ack();	   
	for(i=0;i<len;i++)
	{	   
    	buf[i]=CT_IIC_Read_Byte(i==(len-1)?0:1); //发数据	  
	} 
    CT_IIC_Stop();//产生一个停止条件     
} 
uint8_t CIP[5]; //用来存放触摸IC-GT911
//初始化FT5206触摸屏
//返回值:0,初始化成功;1,初始化失败 
uint8_t FT5206_Init(void)
{
	uint8_t temp[5];
	uint8_t res=1;
	GPIO_InitTypeDef GPIO_Initure;

	__HAL_RCC_GPIOH_CLK_ENABLE();			//开启GPIOH时钟
	__HAL_RCC_GPIOI_CLK_ENABLE();			//开启GPIOI时钟
							
	//PH7
	GPIO_Initure.Pin=GPIO_PIN_7;            //PH7
	GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
	GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
	HAL_GPIO_Init(GPIOH,&GPIO_Initure);     //初始化
					
	//PI8
	GPIO_Initure.Pin=GPIO_PIN_8;            //PI8
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
	HAL_GPIO_Init(GPIOI,&GPIO_Initure);     //初始化
        
	CT_IIC_Init();      	//初始化电容屏的I2C总线  
	FT_RST=0;				//复位
	HAL_Delay(20);
 	FT_RST=1;				//释放复位		    
	HAL_Delay(50);  	
	temp[0]=0;

	//读取版本号，参考值：0x3003
//	FT5206_RD_Reg(FT_ID_G_LIB_VERSION,&temp[0],2);  

	FT5206_WR_Reg(FT_DEVIDE_MODE,temp,1);	//进入正常操作模式 
	FT5206_WR_Reg(FT_ID_G_MODE,temp,1);		//查询模式 
	temp[0]=22;								//触摸有效值，22，越小越灵敏	
	FT5206_WR_Reg(FT_ID_G_THGROUP,temp,1);	//设置触摸有效值
	temp[0]=12;								//激活周期，不能小于12，最大14
	FT5206_WR_Reg(FT_ID_G_PERIODACTIVE,temp,1); 
	res = 0;

	return res;
}
const uint16_t FT5206_TPX_TBL[5]={FT_TP1_REG,FT_TP2_REG,FT_TP3_REG,FT_TP4_REG,FT_TP5_REG};

uint8_t g_gt_tnum=5;      //默认支持的触摸屏点数(5点触摸)
//扫描触摸屏(采用查询方式)
//mode:0,正常扫描.
//返回值:当前触屏状态.
//0,触屏无触摸;1,触屏有触摸
uint8_t FT5206_Scan(uint8_t mode)
{
	uint8_t buf[4];
	uint8_t i=0;
	uint8_t res=0;
	uint8_t temp;
	FT5206_RD_Reg(FT_REG_NUM_FINGER,&mode,1);//读取触摸点的状态  
	if((mode&0XF)&&((mode&0XF)<6))
	{
		temp=0XFF<<(mode&0XF);//将点的个数转换为1的位数,匹配tp_dev.sta定义 
		sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES; 
		for(i=0;i<5;i++)
		{
			if(sta&(1<<i))		//触摸有效?
			{
				FT5206_RD_Reg(FT5206_TPX_TBL[i],buf,4);	//读取XY坐标值 
				if(1)//横屏
				{
					t_y[i]=((uint16_t)(buf[0]&0X0F)<<8)+buf[1];
					t_x[i]=((uint16_t)(buf[2]&0X0F)<<8)+buf[3];
				}
				else
				{
					t_x[i]=1024-(((uint16_t)(buf[0]&0X0F)<<8)+buf[1]);
					t_y[i]=((uint16_t)(buf[2]&0X0F)<<8)+buf[3];
				}  
//				if((buf[0]&0XF0)!=0X80)t_x[i]=t_y[i]=0;//必须是contact事件，才认为有效
			}			
		} 
		res=1;
		if(t_x[0]==0 && t_y[0]==0)mode=0;	//读到的数据都是0,则忽略此次数据
	}
	if((mode&0X1F)==0)//无触摸点按下
	{ 
		if(sta&TP_PRES_DOWN)	//之前是被按下的
		{
			sta&=~(1<<7);	//标记按键松开
		}else						//之前就没有被按下
		{ 
			t_x[0]=0xffff;
			t_y[0]=0xffff;
			sta&=0XE0;	//清除点有效标记	
		}	 
	} 	
	return res;
}
 
uint8_t get_touch(uint16_t *x,uint16_t *y)
{
	uint8_t press;
	press=FT5206_Scan(0);
	*x=t_x[0];
	*y=t_y[0];
  return press;
}


























