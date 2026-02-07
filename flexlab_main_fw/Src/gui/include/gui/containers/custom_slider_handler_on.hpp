#ifndef CUSTOM_SLIDER_HANDLER_ON_HPP
#define CUSTOM_SLIDER_HANDLER_ON_HPP

#include <gui_generated/containers/custom_slider_handler_onBase.hpp>

class custom_slider_handler_on : public custom_slider_handler_onBase
{
public:
		uint8_t hour;
		uint8_t minute;

    custom_slider_handler_on();
    virtual ~custom_slider_handler_on() {}
			
		virtual void handleDragEvent(const DragEvent&	evt);

    virtual void initialize();
			
		void set_handler_pos(uint8_t hour, uint8_t minute);
			
protected:
};

#endif // CUSTOM_SLIDER_HANDLER_ON_HPP
