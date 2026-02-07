#ifndef SCROLL_WHEEL_TEXT_MID_HPP
#define SCROLL_WHEEL_TEXT_MID_HPP

#include <gui_generated/containers/scroll_wheel_text_midBase.hpp>

class scroll_wheel_text_mid : public scroll_wheel_text_midBase
{
public:
    scroll_wheel_text_mid();
    virtual ~scroll_wheel_text_mid() {}

    virtual void initialize();
			
		void updateText(int16_t value, uint8_t lenth);
			
protected:
};

#endif // SCROLL_WHEEL_TEXT_MID_HPP
