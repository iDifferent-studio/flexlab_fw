#include <gui/screen3_screen/Screen3View.hpp>

//#include "rtc.h"

Screen3View::Screen3View() :
	ContainerClickCallback(this, &Screen3View::ContainerClickHandler),
    scrollWheel_yearAnimateToCallback(this, &Screen3View::scrollWheel_yearAnimateToHandler),
    scrollWheel_monthAnimateToCallback(this, &Screen3View::scrollWheel_monthAnimateToHandler),
    scrollWheel_dayAnimateToCallback(this, &Screen3View::scrollWheel_dayAnimateToHandler)
{

}

#ifdef SIMULATOR
void Screen3View::setupScreen()
{
    Screen3ViewBase::setupScreen();
	
	system_time_box.setClickAction(ContainerClickCallback);
	system_date_box.setClickAction(ContainerClickCallback);
	
	scrollWheel_year.setAnimateToCallback(scrollWheel_yearAnimateToCallback);
	scrollWheel_month.setAnimateToCallback(scrollWheel_monthAnimateToCallback);
	scrollWheel_day_31.setAnimateToCallback(scrollWheel_dayAnimateToCallback);
	scrollWheel_day_30.setAnimateToCallback(scrollWheel_dayAnimateToCallback);
	scrollWheel_day_29.setAnimateToCallback(scrollWheel_dayAnimateToCallback);
	scrollWheel_day_28.setAnimateToCallback(scrollWheel_dayAnimateToCallback);
	
	Unicode::snprintf(system_time_setBuffer, SYSTEM_TIME_SET_SIZE, (const uint16_t*)L"%d:%02d:%02d", sTime.Hours, sTime.Minutes, sTime.Seconds);
	Unicode::snprintf(system_date_setBuffer, SYSTEM_DATE_SET_SIZE, (const uint16_t*)L"20%02d年%d月%d日", sDate.Year, sDate.Month, sDate.Date);
	
	invalidate();
		
}
#else
void Screen3View::setupScreen()
{
    Screen3ViewBase::setupScreen();
	
	system_time_box.setClickAction(ContainerClickCallback);
	system_date_box.setClickAction(ContainerClickCallback);
	
	scrollWheel_year.setAnimateToCallback(scrollWheel_yearAnimateToCallback);
	scrollWheel_month.setAnimateToCallback(scrollWheel_monthAnimateToCallback);
	scrollWheel_day_31.setAnimateToCallback(scrollWheel_dayAnimateToCallback);
	scrollWheel_day_30.setAnimateToCallback(scrollWheel_dayAnimateToCallback);
	scrollWheel_day_29.setAnimateToCallback(scrollWheel_dayAnimateToCallback);
	scrollWheel_day_28.setAnimateToCallback(scrollWheel_dayAnimateToCallback);
	
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	
	Unicode::snprintf(system_time_setBuffer, SYSTEM_TIME_SET_SIZE, (uint16_t*)L"%d:%02d:%02d", sTime.Hours, sTime.Minutes, sTime.Seconds);
	Unicode::snprintf(system_date_setBuffer, SYSTEM_DATE_SET_SIZE, (uint16_t*)L"20%02d年%d月%d日", sDate.Year, sDate.Month, sDate.Date);
	
	invalidate();
		
}
#endif

void Screen3View::tearDownScreen()
{
    Screen3ViewBase::tearDownScreen();
}

#ifdef SIMULATOR
void Screen3View::handleTickEvent()
{
	if(input_s.isVisible()==false)
	{
		Unicode::snprintf(system_time_setBuffer, SYSTEM_TIME_SET_SIZE, (const uint16_t*)L"%d:%02d:%02d", sTime.Hours, sTime.Minutes, sTime.Seconds);
		Unicode::snprintf(system_date_setBuffer, SYSTEM_DATE_SET_SIZE, (const uint16_t*)L"20%02d年%d月%d日", sDate.Year, sDate.Month, sDate.Date);
		
		invalidate();
	}
}
#else
void Screen3View::handleTickEvent()
{
	if(input_s.isVisible()==false)
	{
		HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	
		Unicode::snprintf(system_time_setBuffer, SYSTEM_TIME_SET_SIZE, (uint16_t*)L"%d:%02d:%02d", sTime.Hours, sTime.Minutes, sTime.Seconds);
		Unicode::snprintf(system_date_setBuffer, SYSTEM_DATE_SET_SIZE, (uint16_t*)L"20%02d年%d月%d日", sDate.Year, sDate.Month, sDate.Date);
		
		invalidate();
	}
}
#endif

void Screen3View::ContainerClickHandler(const Container& ct, const ClickEvent& evt)//!
{	
	if(&ct==&system_time_box)
	{
		enter_ct=0;
		
		scrollWheel_hour.animateToItem(sTime.Hours,0);
		scrollWheel_minute.animateToItem(sTime.Minutes,0);
		
		hour=sTime.Hours;
		minute=sTime.Minutes;
		
		input_s.show();
		
		textArea1.setVisible(true);
		scrollWheel_minute.setVisible(true);
		scrollWheel_hour.setVisible(true);
		
		textArea2.setVisible(false);
		textArea3.setVisible(false);
		scrollWheel_year.setVisible(false);
		scrollWheel_month.setVisible(false);
		scrollWheel_day_31.setVisible(false);
		scrollWheel_day_30.setVisible(false);
		scrollWheel_day_29.setVisible(false);
		scrollWheel_day_28.setVisible(false);
	}	
	else if(&ct==&system_date_box)
	{
		enter_ct=1;
		
		scrollWheel_year.animateToItem(sDate.Year,0);
		scrollWheel_month.animateToItem(sDate.Month-1,0);
		
		year=sDate.Year;
		month=sDate.Month;
		date=sDate.Date;
		
		input_s.show();
		
		textArea1.setVisible(false);
		scrollWheel_minute.setVisible(false);
		scrollWheel_hour.setVisible(false);
		
		textArea2.setVisible(true);
		textArea3.setVisible(true);
		scrollWheel_year.setVisible(true);
		scrollWheel_month.setVisible(true);
		
		switch(sDate.Month)
		{
			case 1:
			case 3:
			case 5:
			case 7:
			case 8:
			case 10:
			case 12:	
				scrollWheel_day_31.animateToItem(sDate.Date-1,0);
				
				scrollWheel_day_31.setVisible(true);
				scrollWheel_day_30.setVisible(false);
				scrollWheel_day_29.setVisible(false);
				scrollWheel_day_28.setVisible(false);
			
				scrollWheel_day_31.invalidate();
			break;
			
			case 4:
			case 6:
			case 9:
			case 11:
				scrollWheel_day_30.animateToItem(sDate.Date-1,0);
				
				scrollWheel_day_31.setVisible(false);
				scrollWheel_day_30.setVisible(true);
				scrollWheel_day_29.setVisible(false);
				scrollWheel_day_28.setVisible(false);
				
				scrollWheel_day_30.invalidate();
			break;
			
			case 2:
				if ((sDate.Year % 4 == 0 && sDate.Year % 100 != 0) || sDate.Year % 400 == 0)
				{
					scrollWheel_day_29.animateToItem(sDate.Date-1,0);
			
					scrollWheel_day_31.setVisible(false);
					scrollWheel_day_30.setVisible(false);
					scrollWheel_day_29.setVisible(true);
					scrollWheel_day_28.setVisible(false);
				
					scrollWheel_day_29.invalidate();
				}
				else
				{
					scrollWheel_day_28.animateToItem(sDate.Date-1,0);
			
					scrollWheel_day_31.setVisible(false);
					scrollWheel_day_30.setVisible(false);
					scrollWheel_day_29.setVisible(false);
					scrollWheel_day_28.setVisible(true);
				
					scrollWheel_day_28.invalidate();
				}
				
		}

	}
}

void Screen3View::scrollWheel_yearAnimateToHandler(int16_t itemSelected)
{
	year=itemSelected;
	if(month==2)
	{
		if ((year% 4 == 0 && year % 100 != 0) || year % 400 == 0)
		{
			if(date>29)
				date=29;
			scrollWheel_day_29.animateToItem(date-1,0);
		
			scrollWheel_day_31.setVisible(false);
			scrollWheel_day_30.setVisible(false);
			scrollWheel_day_29.setVisible(true);
			scrollWheel_day_28.setVisible(false);
		
			scrollWheel_day_29.invalidate();
		}
		else
		{
			if(date>28)
				date=28;
			scrollWheel_day_28.animateToItem(date-1,0);
		
			scrollWheel_day_31.setVisible(false);
			scrollWheel_day_30.setVisible(false);
			scrollWheel_day_29.setVisible(false);
			scrollWheel_day_28.setVisible(true);
		
			scrollWheel_day_28.invalidate();
		}
	}
}

void Screen3View::scrollWheel_monthAnimateToHandler(int16_t itemSelected)
{
	month=itemSelected+1;
	switch(month)
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:	
			scrollWheel_day_31.animateToItem(date-1,0);
			
			scrollWheel_day_31.setVisible(true);
			scrollWheel_day_30.setVisible(false);
			scrollWheel_day_29.setVisible(false);
			scrollWheel_day_28.setVisible(false);
		
			scrollWheel_day_31.invalidate();
		break;
		
		case 4:
		case 6:
		case 9:
		case 11:
			if(date>30)
				date=30;
			scrollWheel_day_30.animateToItem(date-1,0);
		
			scrollWheel_day_31.setVisible(false);
			scrollWheel_day_30.setVisible(true);
			scrollWheel_day_29.setVisible(false);
			scrollWheel_day_28.setVisible(false);
			
			scrollWheel_day_30.invalidate();
		break;
		
		case 2:
			if ((scrollWheel_year.getSelectedItem()% 4 == 0 && scrollWheel_year.getSelectedItem() % 100 != 0) || scrollWheel_year.getSelectedItem() % 400 == 0)
			{
				if(date>29)
					date=29;
				scrollWheel_day_29.animateToItem(date-1,0);
			
				scrollWheel_day_31.setVisible(false);
				scrollWheel_day_30.setVisible(false);
				scrollWheel_day_29.setVisible(true);
				scrollWheel_day_28.setVisible(false);
			
				scrollWheel_day_29.invalidate();
			}
			else
			{
				if(date>28)
					date=28;
				scrollWheel_day_28.animateToItem(date-1,0);
			
				scrollWheel_day_31.setVisible(false);
				scrollWheel_day_30.setVisible(false);
				scrollWheel_day_29.setVisible(false);
				scrollWheel_day_28.setVisible(true);
			
				scrollWheel_day_28.invalidate();
			}
	}
}

void Screen3View::scrollWheel_dayAnimateToHandler(int16_t itemSelected)
{
	date=itemSelected+1;
}

void Screen3View::scrollWheel_hourUpdateItem(scroll_wheel_text& item, int16_t itemIndex)
{
    item.updateText(itemIndex,1);
}

void Screen3View::scrollWheel_hourUpdateCenterItem(scroll_wheel_text_mid& item, int16_t itemIndex)
{
    item.updateText(itemIndex,1);
}

void Screen3View::scrollWheel_minuteUpdateItem(scroll_wheel_text& item, int16_t itemIndex)
{
    item.updateText(itemIndex,2);
}

void Screen3View::scrollWheel_minuteUpdateCenterItem(scroll_wheel_text_mid& item, int16_t itemIndex)
{
    item.updateText(itemIndex,2);
}

void Screen3View::scrollWheel_yearUpdateItem(scroll_wheel_text_small& item, int16_t itemIndex)
{
    item.updateText(itemIndex,2);
}

void Screen3View::scrollWheel_yearUpdateCenterItem(scroll_wheel_text_small_mid& item, int16_t itemIndex)
{
    item.updateText(itemIndex,2);
}

void Screen3View::scrollWheel_monthUpdateItem(scroll_wheel_text_small& item, int16_t itemIndex)
{
    item.updateText(itemIndex+1,1);
}

void Screen3View::scrollWheel_monthUpdateCenterItem(scroll_wheel_text_small_mid& item, int16_t itemIndex)
{
    item.updateText(itemIndex+1,1);
}

void Screen3View::scrollWheel_day_31UpdateItem(scroll_wheel_text_small& item, int16_t itemIndex)
{
    item.updateText(itemIndex+1,1);
}

void Screen3View::scrollWheel_day_31UpdateCenterItem(scroll_wheel_text_small_mid& item, int16_t itemIndex)
{
    item.updateText(itemIndex+1,1);
}

void Screen3View::scrollWheel_day_30UpdateItem(scroll_wheel_text_small& item, int16_t itemIndex)
{
    item.updateText(itemIndex+1,1);
}

void Screen3View::scrollWheel_day_30UpdateCenterItem(scroll_wheel_text_small_mid& item, int16_t itemIndex)
{
    item.updateText(itemIndex+1,1);
}

void Screen3View::scrollWheel_day_29UpdateItem(scroll_wheel_text_small& item, int16_t itemIndex)
{
    item.updateText(itemIndex+1,1);
}

void Screen3View::scrollWheel_day_29UpdateCenterItem(scroll_wheel_text_small_mid& item, int16_t itemIndex)
{
    item.updateText(itemIndex+1,1);
}

void Screen3View::scrollWheel_day_28UpdateItem(scroll_wheel_text_small& item, int16_t itemIndex)
{
    item.updateText(itemIndex+1,1);
}

void Screen3View::scrollWheel_day_28UpdateCenterItem(scroll_wheel_text_small_mid& item, int16_t itemIndex)
{
    item.updateText(itemIndex+1,1);
}

#ifdef SIMULATOR
void Screen3View::button_confirmButtonPressed()
{	
	switch(enter_ct)
	{
		case 0:
			hour=scrollWheel_hour.getSelectedItem();
			minute=scrollWheel_minute.getSelectedItem();
			
			input_s.hide();
		break;
		
		case 1:
			
			input_s.hide();
		break;
	}
    
}
#else
void Screen3View::button_confirmButtonPressed()
{	
	switch(enter_ct)
	{
		case 0:
			hour=scrollWheel_hour.getSelectedItem();
			minute=scrollWheel_minute.getSelectedItem();
			RTC_Set_Time(hour,minute,00,RTC_HOURFORMAT12_PM);
			
			input_s.hide();
		break;
		
		case 1:
			uint8_t t_year, t_month, weekday;
			if((month == 1) || (month == 2)){
				t_month += 12;
				t_year--;
			}
			weekday = (date+2*t_month+3*(t_month + 1)/5+t_year+t_year/4-t_year/100+t_year/400)%7;
		
			RTC_Set_Date(year,month,date,weekday);	
			
			input_s.hide();
		break;
	}
    
}
#endif

void Screen3View::button_cancelButtonPressed()
{
    input_s.hide();
}


