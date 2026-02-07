#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

#include <gui/lens/Lens.hpp>
#include <touchgfx/mixins/Draggable.hpp>

#include <touchgfx/mixins/ClickListener.hpp>

#ifndef SIMULATOR
#include "rtc.h"
#endif

class Screen1View : public Screen1ViewBase
{
public:
    Screen1View();
    virtual ~Screen1View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
			
		virtual void zoomButtonPressed();
			
		void ContainerClickHandler(const Container& ct, const ClickEvent& evt);//!
		void layer_infoClickHandler(const layer_info& li, const ClickEvent& evt);//!

		virtual void handleTickEvent();
			
		void set_cabinet_infomation(int8_t temp, uint8_t level_status);
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
	
		Draggable<Lens> myLens1;

		Layer_StatusTypeDef layer_status[3];
		
		touchgfx::Callback<Screen1View, const Container&, const ClickEvent&> ContainerClickCallback;//!
		touchgfx::Callback<Screen1View, const layer_info&, const ClickEvent&> layer_infoClickCallback;//!
};

#endif // SCREEN1VIEW_HPP
