#include <gui/containers/Double_slider.hpp>

Double_slider::Double_slider():on_time(0),off_time(5)
{ 	

}

void Double_slider::initialize()
{
    Double_sliderBase::initialize();
		custom_slider_handler_on1.set_handler_pos(on_time, 0);
		custom_slider_handler_off1.set_handler_pos(off_time, 0);
}

void Double_slider::set_slider(uint8_t set_ontime, uint8_t set_offtime)
{
		custom_slider_handler_on1.set_handler_pos(set_ontime, 0);
		custom_slider_handler_off1.set_handler_pos(set_offtime, 0);
		on_time=set_ontime;
		off_time=set_offtime;
}

void Double_slider::get_slider(uint8_t * get_ontime, uint8_t * get_offtime)
{
		on_time=custom_slider_handler_on1.hour;
		off_time=custom_slider_handler_off1.hour;
		*get_ontime=on_time;
		*get_offtime=off_time;
}

void Double_slider::handleTickEvent()
{
	if(custom_slider_handler_on1.getX()<0)
		custom_slider_handler_on1.moveTo(0,0);
	else if(custom_slider_handler_on1.getX()>407)
		custom_slider_handler_on1.moveTo(407,0);
	
	if(custom_slider_handler_off1.getX()<0)
		custom_slider_handler_off1.moveTo(0,0);
	else if(custom_slider_handler_off1.getX()>407)
		custom_slider_handler_off1.moveTo(407,0);
}

