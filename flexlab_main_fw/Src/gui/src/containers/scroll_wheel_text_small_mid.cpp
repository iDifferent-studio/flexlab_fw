#include <gui/containers/scroll_wheel_text_small_mid.hpp>

scroll_wheel_text_small_mid::scroll_wheel_text_small_mid()
{

}

void scroll_wheel_text_small_mid::initialize()
{
    scroll_wheel_text_small_midBase::initialize();
}

void scroll_wheel_text_small_mid::updateText(int16_t value, uint8_t lenth)
{
		if(lenth==1)
			Unicode::snprintf(timeBuffer, TIME_SIZE, "%d", value);
		else if(lenth==2)
			Unicode::snprintf(timeBuffer, TIME_SIZE, "%02d", value);
    time.invalidate();
}
