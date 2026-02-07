#ifndef SCROLL_WHEEL_TEXT_SMALL_HPP
#define SCROLL_WHEEL_TEXT_SMALL_HPP

#include <gui_generated/containers/scroll_wheel_text_smallBase.hpp>

class scroll_wheel_text_small : public scroll_wheel_text_smallBase
{
public:
    scroll_wheel_text_small();
    virtual ~scroll_wheel_text_small() {}

    virtual void initialize();
			
		void updateText(int16_t value, uint8_t lenth);
protected:
};

#endif // SCROLL_WHEEL_TEXT_SMALL_HPP
