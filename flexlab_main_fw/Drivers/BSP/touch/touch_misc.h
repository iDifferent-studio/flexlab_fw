#ifndef __TOUCH_MISC_H
#define __TOUCH_MISC_H

#ifdef __cplusplus
 extern "C" {
#endif

#define TP_PRES_DOWN 0x80  //触屏被按下	  
#define TP_CATH_PRES 0x40  //有按键按下了 
#define CT_MAX_TOUCH  5    //电容屏支持的点数,固定为5点 
	 
uint16_t t_x[CT_MAX_TOUCH]; 		//当前坐标
uint16_t t_y[CT_MAX_TOUCH];
static uint8_t sta=0;	 
	 
#ifdef __cplusplus
}
#endif
#endif