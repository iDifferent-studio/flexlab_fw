#ifndef CUSTOM_SLIDER_HANDLER_OFF_HPP
#define CUSTOM_SLIDER_HANDLER_OFF_HPP

#include <gui_generated/containers/custom_slider_handler_offBase.hpp>

class custom_slider_handler_off : public custom_slider_handler_offBase
{
public:
		uint8_t hour;
		uint8_t minute;

    custom_slider_handler_off();
    virtual ~custom_slider_handler_off() {}
			
		virtual void handleDragEvent(const DragEvent&	evt);

    virtual void initialize();
			
		void set_handler_pos(uint8_t hour, uint8_t minute);
			
protected:
};

#endif // CUSTOM_SLIDER_HANDLER_OFF_HPP
