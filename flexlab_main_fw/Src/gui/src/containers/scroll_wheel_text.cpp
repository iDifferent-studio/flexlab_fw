#include <gui/containers/scroll_wheel_text.hpp>

scroll_wheel_text::scroll_wheel_text()
{

}

void scroll_wheel_text::initialize()
{
    scroll_wheel_textBase::initialize();
}

void scroll_wheel_text::updateText(int16_t value, uint8_t lenth)
{
		if(lenth==1)
			Unicode::snprintf(timeBuffer, TIME_SIZE, "%d", value);
		else if(lenth==2)
			Unicode::snprintf(timeBuffer, TIME_SIZE, "%02d", value);
    time.invalidate();
}
