#include <gui/containers/custom_slider_handler_on.hpp>

#include <texts/TextKeysAndLanguages.hpp>

custom_slider_handler_on::custom_slider_handler_on()
{

}

void custom_slider_handler_on::initialize()
{
    custom_slider_handler_onBase::initialize();
		hour=0;
		minute=0;
		set_handler_pos(hour, minute);
}

#ifdef SIMULATOR
void custom_slider_handler_on::handleDragEvent(const DragEvent&	evt)
{
		for(int i=0; i<23; i++)
		{
				if(getX()>=i*17 && getX()<(i+1)*17)
						hour=i;
		}
		if(getX()>=391 && getX()<=407)
			hour=23;
		
		Unicode::snprintf(on_timeBuffer, ON_TIME_SIZE, (const uint16_t*)L"%d:%02d", hour, minute);
}

void custom_slider_handler_on::set_handler_pos(uint8_t s_hour, uint8_t s_minute)
{
	if(s_hour<23)
		setX(s_hour*17);
	else if(s_hour==23)
		setX(407);
	
	hour=s_hour;
	minute=s_minute;
	
	Unicode::snprintf(on_timeBuffer, ON_TIME_SIZE, (const uint16_t*)L"%d:%02d", s_hour, s_minute);
	
	invalidate();
}
#else
void custom_slider_handler_on::handleDragEvent(const DragEvent&	evt)
{
		for(int i=0; i<23; i++)
		{
				if(getX()>=i*17 && getX()<(i+1)*17)
						hour=i;
		}
		if(getX()>=391 && getX()<=407)
			hour=23;
		
		Unicode::snprintf(on_timeBuffer, ON_TIME_SIZE, (uint16_t*)L"%d:%02d", hour, minute);
}

void custom_slider_handler_on::set_handler_pos(uint8_t s_hour, uint8_t s_minute)
{
	if(s_hour<23)
		setX(s_hour*17);
	else if(s_hour==23)
		setX(407);
	
	hour=s_hour;
	minute=s_minute;
	
	Unicode::snprintf(on_timeBuffer, ON_TIME_SIZE, (uint16_t*)L"%d:%02d", s_hour, s_minute);
	
	invalidate();
}
#endif
