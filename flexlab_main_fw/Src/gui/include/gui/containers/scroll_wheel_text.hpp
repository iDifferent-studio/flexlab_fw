#ifndef SCROLL_WHEEL_TEXT_HPP
#define SCROLL_WHEEL_TEXT_HPP

#include <gui_generated/containers/scroll_wheel_textBase.hpp>

class scroll_wheel_text : public scroll_wheel_textBase
{
public:
    scroll_wheel_text();
    virtual ~scroll_wheel_text() {}

    virtual void initialize();
			
		void updateText(int16_t value, uint8_t lenth);
protected:
};

#endif // SCROLL_WHEEL_TEXT_HPP
