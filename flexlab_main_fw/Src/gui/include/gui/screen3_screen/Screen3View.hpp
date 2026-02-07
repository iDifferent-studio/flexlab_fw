#ifndef SCREEN3VIEW_HPP
#define SCREEN3VIEW_HPP

#include <gui_generated/screen3_screen/Screen3ViewBase.hpp>
#include <gui/screen3_screen/Screen3Presenter.hpp>

#ifndef SIMULATOR
#include "rtc.h"
#endif

class Screen3View : public Screen3ViewBase
{
public:
    Screen3View();
    virtual ~Screen3View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
			
		virtual void handleTickEvent();
			
		virtual void button_confirmButtonPressed();
		virtual void button_cancelButtonPressed();
			
    virtual void scrollWheel_hourUpdateItem(scroll_wheel_text& item, int16_t itemIndex);
    virtual void scrollWheel_hourUpdateCenterItem(scroll_wheel_text_mid& item, int16_t itemIndex);
    virtual void scrollWheel_minuteUpdateItem(scroll_wheel_text& item, int16_t itemIndex);
    virtual void scrollWheel_minuteUpdateCenterItem(scroll_wheel_text_mid& item, int16_t itemIndex);

		virtual void scrollWheel_yearUpdateItem(scroll_wheel_text_small& item, int16_t itemIndex);
    virtual void scrollWheel_yearUpdateCenterItem(scroll_wheel_text_small_mid& item, int16_t itemIndex);
    virtual void scrollWheel_monthUpdateItem(scroll_wheel_text_small& item, int16_t itemIndex);
    virtual void scrollWheel_monthUpdateCenterItem(scroll_wheel_text_small_mid& item, int16_t itemIndex);
    virtual void scrollWheel_day_31UpdateItem(scroll_wheel_text_small& item, int16_t itemIndex);
    virtual void scrollWheel_day_31UpdateCenterItem(scroll_wheel_text_small_mid& item, int16_t itemIndex);
    virtual void scrollWheel_day_30UpdateItem(scroll_wheel_text_small& item, int16_t itemIndex);
    virtual void scrollWheel_day_30UpdateCenterItem(scroll_wheel_text_small_mid& item, int16_t itemIndex);
    virtual void scrollWheel_day_29UpdateItem(scroll_wheel_text_small& item, int16_t itemIndex);
    virtual void scrollWheel_day_29UpdateCenterItem(scroll_wheel_text_small_mid& item, int16_t itemIndex);
    virtual void scrollWheel_day_28UpdateItem(scroll_wheel_text_small& item, int16_t itemIndex);
    virtual void scrollWheel_day_28UpdateCenterItem(scroll_wheel_text_small_mid& item, int16_t itemIndex);
			
		void ContainerClickHandler(const Container& ct, const ClickEvent& evt);//!
protected:

#ifdef SIMULATOR
		typedef struct
		{
		uint8_t Hours;           
		uint8_t Minutes;
		uint8_t Seconds;
		uint8_t TimeFormat;
		uint32_t SubSeconds;
		uint32_t SecondFraction;
		uint32_t DayLightSaving;
		uint32_t StoreOperation;       
		}RTC_TimeTypeDef;

		typedef struct
		{
		uint8_t WeekDay;
		uint8_t Month;
		uint8_t Date;
		uint8_t Year;
		}RTC_DateTypeDef;
#endif

		RTC_TimeTypeDef sTime;
		RTC_DateTypeDef sDate;

		uint8_t year, month, date, hour, minute; 
		
		uint8_t enter_ct;

		touchgfx::Callback<Screen3View, const Container&, const ClickEvent&> ContainerClickCallback;//!

    touchgfx::Callback<Screen3View, int16_t> scrollWheel_yearAnimateToCallback;
    void scrollWheel_yearAnimateToHandler(int16_t itemSelected);

    touchgfx::Callback<Screen3View, int16_t> scrollWheel_monthAnimateToCallback;
    void scrollWheel_monthAnimateToHandler(int16_t itemSelected);

    touchgfx::Callback<Screen3View, int16_t> scrollWheel_dayAnimateToCallback;
    void scrollWheel_dayAnimateToHandler(int16_t itemSelected);
};

#endif // SCREEN3VIEW_HPP
