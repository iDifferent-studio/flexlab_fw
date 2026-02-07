#include <gui/screen1_screen/Screen1View.hpp>

#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>

#ifndef SIMULATOR
#include "cmsis_os.h"

extern osMessageQueueId_t LayerStatusQueueHandle;
extern osMessageQueueId_t LayerNumQueueHandle;
extern osMessageQueueId_t LayerBLRGBQueueHandle;
extern osMessageQueueId_t CarbinStatusQueueHandle;
#endif

Screen1View::Screen1View():
	ContainerClickCallback(this, &Screen1View::ContainerClickHandler),//!
	layer_infoClickCallback(this, &Screen1View::layer_infoClickHandler)
{

}

void Screen1View::setupScreen()
{	
#ifndef SIMULATOR
		//加载外部Flash中的存储内容，缓存到SDRAM的(uint16_t *)0xd0300000之后的0x400000的大小空间，最多加载128个文件
		Bitmap::setCache((uint16_t *)0xc0600000,0x1000000,128);
		Bitmap::cacheAll();
#endif

		Screen1ViewBase::setupScreen();
	
		myLens1.setXY(140, 40);
		myLens1.setLensBackgroundBitmap(BITMAP_LENS_IMAGE_ID);
//		add(myLens1);
		
		presenter->get_layer_status(&layer_status[0], 0);
		presenter->get_layer_status(&layer_status[1], 1);
		presenter->get_layer_status(&layer_status[2], 2);
	
		sys_info.setClickAction(ContainerClickCallback);//!
		layer_info_1.setClickAction(layer_infoClickCallback);//!
		layer_info_2.setClickAction(layer_infoClickCallback);//!
		layer_info_3.setClickAction(layer_infoClickCallback);//!

#ifndef SIMULATOR			
		HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
#endif

		Unicode::snprintf(timeBuffer, TIME_SIZE, (const uint16_t*)L"%d:%02d", sTime.Hours, sTime.Minutes);
		Unicode::snprintf(dateBuffer, DATE_SIZE, (const uint16_t*)L"%04d年%d月%d日", sDate.Year, sDate.Month, sDate.Date);
		
		layer_info_1.set_layer_number(1);
		layer_info_2.set_layer_number(2);
		layer_info_3.set_layer_number(3);
		
//		layer_info_1.set_layer_state(layer_status[0].layer_state);
//		layer_info_2.set_layer_state(layer_status[1].layer_state);
//		layer_info_3.set_layer_state(layer_status[2].layer_state);

		layer_info_1.set_layer_infomation(-1, 0, 0, 0, 0);
		layer_info_2.set_layer_infomation(-1, 0, 0, 0, 0);
		layer_info_3.set_layer_infomation(-1, 0, 0, 0, 0);
		
		layer_info_1.invalidate();
		layer_info_2.invalidate();
		layer_info_3.invalidate();
		sys_info.invalidate();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::ContainerClickHandler(const Container& ct, const ClickEvent& evt)//!
{	
    if (&ct == &sys_info && evt.getType()==evt.PRESSED)
    {
		application().gotoScreen3ScreenWipeTransitionWest();
    }
}

void Screen1View::layer_infoClickHandler(const layer_info& li, const ClickEvent& evt)//!
{
	uint8_t layer_num;
	if (&li == &layer_info_1 && evt.getType()==evt.PRESSED && layer_info_1.is_available)
	{
			application().gotoScreen2ScreenWipeTransitionWest();
			layer_num=1;
	}
	else if (&li == &layer_info_2 && evt.getType()==evt.PRESSED && layer_info_2.is_available)
	{
			application().gotoScreen2ScreenWipeTransitionWest();
			layer_num=2;
	}
	else if (&li == &layer_info_3 && evt.getType()==evt.PRESSED && layer_info_3.is_available)
	{
			application().gotoScreen2ScreenWipeTransitionWest();
			layer_num=3;
	}

	presenter->set_enter_layer(layer_num);

#ifndef SIMULATOR	
	const uint32_t RGB=0x50505001;
	const uint8_t q_layer_num= layer_num;
	
	osMessageQueuePut(LayerBLRGBQueueHandle, &RGB, 1, 100);
	osMessageQueuePut(LayerNumQueueHandle, &q_layer_num, 1, 100);
#endif	
}

void Screen1View::handleTickEvent()
{
	static uint16_t t=0;
	t++;
	
	if(t%10==0)
	{
#ifndef SIMULATOR
		HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
#endif
	
		if(sTime.SubSeconds>127)
				Unicode::snprintf(timeBuffer, TIME_SIZE, (const uint16_t*)L"%d:%02d", sTime.Hours, sTime.Minutes);
		else
				Unicode::snprintf(timeBuffer, TIME_SIZE, (const uint16_t*)L"%d %02d", sTime.Hours, sTime.Minutes);
		
		Unicode::snprintf(dateBuffer, DATE_SIZE, (const uint16_t*)L"20%02d年%d月%d日", sDate.Year, sDate.Month, sDate.Date);

		sys_info.invalidate();
	}

	if(t%50==0)
	{
#ifndef SIMULATOR
		if(osMessageQueueGetSpace(LayerStatusQueueHandle)!=3
		)
		{	
			q_Layer_StatusTypeDef q_Layer_Status;
			
			osMessageQueueGet(LayerStatusQueueHandle, &q_Layer_Status, (uint8_t *)1, 200);

			switch(q_Layer_Status.layer_num)
			{
				case 1:
					layer_info_1.set_layer_infomation(q_Layer_Status.layer_state, q_Layer_Status.temp, (bool)q_Layer_Status.light_status, (bool)q_Layer_Status.cycle_status, 1);
				break;
				
				case 2:
					layer_info_2.set_layer_infomation(q_Layer_Status.layer_state, q_Layer_Status.temp, (bool)q_Layer_Status.light_status, (bool)q_Layer_Status.cycle_status, 1);
				break;
				
				case 3:
					layer_info_3.set_layer_infomation(q_Layer_Status.layer_state, q_Layer_Status.temp, (bool)q_Layer_Status.light_status, (bool)q_Layer_Status.cycle_status, 1);
				break;
			}
		}
		else
		{
			layer_info_1.set_layer_infomation(-1, 0, 0, 0, 0);
			layer_info_2.set_layer_infomation(-1, 0, 0, 0, 0);
			layer_info_3.set_layer_infomation(-1, 0, 0, 0, 0);

		}

		if(osMessageQueueGetSpace(CarbinStatusQueueHandle)!=3
		)
		{
			q_Carbin_StatusTypeDef q_Carbin_Status;

			osMessageQueueGet(CarbinStatusQueueHandle, &q_Carbin_Status, (uint8_t *)1, 200);

			set_cabinet_infomation(q_Carbin_Status.temp, q_Carbin_Status.level_status);
		}
		else
		{
			Unicode::snprintf(c_info_tempBuffer, C_INFO_TEMP_SIZE, (const uint16_t*)L"--");			
			Unicode::snprintf(c_info_wlevelBuffer, C_INFO_WLEVEL_SIZE, (const uint16_t*)L"--");			
			Unicode::snprintf(c_nus1_infoBuffer, C_NUS1_INFO_SIZE, (const uint16_t*)L"--");			
			Unicode::snprintf(c_nus2_infoBuffer, C_NUS2_INFO_SIZE, (const uint16_t*)L"--");
			Unicode::snprintf(c_nus3_infoBuffer, C_NUS3_INFO_SIZE, (const uint16_t*)L"--");
			
			cabinet_info.invalidate();
		}
#else
		layer_info_1.set_layer_infomation(1, 0, 0, 0, 0);
		layer_info_2.set_layer_infomation(1, 0, 0, 0, 0);
		layer_info_3.set_layer_infomation(1, 0, 0, 0, 0);	
#endif
		layer_info_1.invalidate();
		layer_info_2.invalidate();
		layer_info_3.invalidate();
	}
}

void Screen1View::set_cabinet_infomation(int8_t temp, uint8_t level_status)
{
		Unicode::snprintf(c_info_tempBuffer, C_INFO_TEMP_SIZE, (const uint16_t*)L"%d", temp);
	
		if((level_status&0x08)==0x08)
			Unicode::snprintf(c_info_wlevelBuffer, C_INFO_WLEVEL_SIZE, (const uint16_t*)L"正常");
		else
			Unicode::snprintf(c_info_wlevelBuffer, C_INFO_WLEVEL_SIZE, (const uint16_t*)L"缺水");
		
		if((level_status&0x04)==0x04)
			Unicode::snprintf(c_nus1_infoBuffer, C_NUS1_INFO_SIZE, (const uint16_t*)L"充足");
		else
			Unicode::snprintf(c_nus1_infoBuffer, C_NUS1_INFO_SIZE, (const uint16_t*)L"缺料");
		
		if((level_status&0x02)==0x02)
			Unicode::snprintf(c_nus2_infoBuffer, C_NUS2_INFO_SIZE, (const uint16_t*)L"充足");
		else                                        
			Unicode::snprintf(c_nus2_infoBuffer, C_NUS2_INFO_SIZE, (const uint16_t*)L"缺料");
		
		if((level_status&0x01)==0x01)
			Unicode::snprintf(c_nus3_infoBuffer, C_NUS3_INFO_SIZE, (const uint16_t*)L"充足");
		else                                        
			Unicode::snprintf(c_nus3_infoBuffer, C_NUS3_INFO_SIZE, (const uint16_t*)L"缺料");
		
		cabinet_info.invalidate();
}

void Screen1View::zoomButtonPressed()
{
	static bool zoom_flag;
	
	if(zoom_flag==false)
	{
		add(myLens1);
		zoom_flag=true;
	}
	else
	{
		remove(myLens1);
		zoom_flag=false;
	}	
	
	invalidate();	
}
