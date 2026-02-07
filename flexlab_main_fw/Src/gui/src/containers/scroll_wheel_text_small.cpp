#include <gui/containers/scroll_wheel_text_small.hpp>

scroll_wheel_text_small::scroll_wheel_text_small()
{

}

void scroll_wheel_text_small::initialize()
{
    scroll_wheel_text_smallBase::initialize();
}

void scroll_wheel_text_small::updateText(int16_t value, uint8_t lenth)
{
		if(lenth==1)
			Unicode::snprintf(timeBuffer, TIME_SIZE, "%d", value);
		else if(lenth==2)
			Unicode::snprintf(timeBuffer, TIME_SIZE, "%02d", value);
    time.invalidate();
}
