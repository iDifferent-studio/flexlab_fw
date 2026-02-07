#include <gui/containers/layer_info.hpp>
#include "BitmapDatabase.hpp"

layer_info::layer_info()
{

}

layer_info::layer_info(uint8_t layer_number)
{
		Unicode::snprintf(title_l1Buffer, TITLE_L1_SIZE, (const uint16_t*)L"%d", layer_number);
}

void layer_info::initialize()
{
    layer_infoBase::initialize();
}

void layer_info::set_layer_number(uint8_t layer_number)
{
		Unicode::snprintf(title_l1Buffer, TITLE_L1_SIZE, (const uint16_t*)L"%d", layer_number);
}

void layer_info::set_layer_infomation(int8_t state, int8_t temp, bool light_s, bool cyc_s, bool wq_state)
{
	touchgfx::Unicode::UnicodeChar str1[3],str2[3],str3[3];

	switch(state)
	{
		case 0:
			state_icon_1.setBitmap(touchgfx::Bitmap(BITMAP_ICON_F_CLOCK_ID));
			Unicode::snprintf(state_l1Buffer, STATE_L1_SIZE, (const uint16_t*)L"就绪");
			is_available=1;
		break;
				
		case 1:
			state_icon_1.setBitmap(touchgfx::Bitmap(BITMAP_ICON_VENT_ID));
			Unicode::snprintf(state_l1Buffer, STATE_L1_SIZE, (const uint16_t*)L"正在通风");
			is_available=1;
		break;
		
		case 2:
			state_icon_1.setBitmap(touchgfx::Bitmap(BITMAP_ICON_RECYCLE_ID));
			Unicode::snprintf(state_l1Buffer, STATE_L1_SIZE, (const uint16_t*)L"正在换水");
			is_available=1;
		break;
		
		case 3:
			state_icon_1.setBitmap(touchgfx::Bitmap(BITMAP_ICON_STANDBY_ID));
			Unicode::snprintf(state_l1Buffer, STATE_L1_SIZE, (const uint16_t*)L"未使用");
			is_available=1;
		break;
		
		default:
			state_icon_1.setBitmap(touchgfx::Bitmap(BITMAP_ICON_STANDBY_ID));
			Unicode::snprintf(state_l1Buffer, STATE_L1_SIZE, (const uint16_t*)L"--");
			is_available=0;
		break;
	}

	if(is_available)
	{
		if(light_s)
			Unicode::snprintf(str1,3,(const uint16_t*)L"开");
		else 
			Unicode::snprintf(str1,3,(const uint16_t*)L"关");
		
		if(cyc_s)
			Unicode::snprintf(str2,3,(const uint16_t*)L"开");
		else 
			Unicode::snprintf(str2,3,(const uint16_t*)L"关");

		if(wq_state)
		{
			Unicode::snprintf(str3,3,(const uint16_t*)L"正常");
			remove(line1);
			remove(line2);
			add(line1);
		}
		else 
		{
			Unicode::snprintf(str3,3,(const uint16_t*)L"异常");
			remove(line1);
			remove(line2);
			add(line2);
		}
			
		
		Unicode::snprintf(l1_infoBuffer, L1_INFO_SIZE, (const uint16_t*)L"水温:%d℃  补光-%s  循环-%s", temp, str1, str2);
		Unicode::snprintf(l1_wq_stateBuffer, L1_WQ_STATE_SIZE, (const uint16_t*)L"水质-%s", str3);
	}
	else
	{
		Unicode::snprintf(l1_infoBuffer, L1_INFO_SIZE, (const uint16_t*)L"水温:℃  补光-  循环-");
		Unicode::snprintf(l1_wq_stateBuffer, L1_WQ_STATE_SIZE, (const uint16_t*)L"水质-");
		remove(line1);
		remove(line2);
		add(line2);
	}
	
	
}

