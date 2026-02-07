#ifndef DOUBLE_SLIDER_HPP
#define DOUBLE_SLIDER_HPP

#include <gui_generated/containers/Double_sliderBase.hpp>
#include <touchgfx/mixins/ClickListener.hpp>

class Double_slider : public Double_sliderBase
{
public:
    Double_slider();
    virtual ~Double_slider() {}
		virtual void handleTickEvent();
    virtual void initialize();
			
		void set_slider(uint8_t set_ontime, uint8_t set_offtime);
		void get_slider(uint8_t * get_ontime, uint8_t * get_offtime);
			
		uint8_t on_time;
		uint8_t off_time;
protected:
};

#endif // DOUBLE_SLIDER_HPP
