#ifndef SCREEN2VIEW_HPP
#define SCREEN2VIEW_HPP

#include <gui_generated/screen2_screen/Screen2ViewBase.hpp>
#include <gui/screen2_screen/Screen2Presenter.hpp>

#include <gui/lens/Lens.hpp>
#include <touchgfx/mixins/Draggable.hpp>

class Screen2View : public Screen2ViewBase
{
public:
		uint8_t	layer_enable;
		uint8_t light_mode;
		uint8_t light_on_time[2];
		uint8_t light_off_time[2];
		uint8_t auto_layer_cycle;
		uint8_t auto_water_change;
		uint16_t auto_cycle_schedule;
		uint16_t nus_v[3];
		uint8_t vent_mode;
		uint16_t vent_schedule;
		
		Screen2View();
		virtual ~Screen2View() {}
		virtual void setupScreen();
		virtual void tearDownScreen();
				
		void ContainerClickHandler(const Container& ct, const ClickEvent& evt);//!
		void ButtonWithLabelClickHandler(const ButtonWithLabel& bt, const ClickEvent& evt);//!
		virtual void handleTickEvent();
				
		virtual void layer_enable_ButtonPressed();
		virtual void light_ButtonPressed();
		virtual void button_set_spectrumButtonPressed();
		virtual void auto_waterButtonPressed();
		virtual void ventButtonPressed();
		virtual void zoomButtonPressed();

		virtual void input_l_cancelButtonPressed();
		virtual void input_s_cancelButtonPressed();
		virtual void input_spectrum_cancelButtonPressed();
		virtual void input_l_confirmButtonPressed();
		virtual void input_s_confirmButtonPressed();
		virtual void input_spectrum_confirmButtonPressed();
		
		virtual void set_hourButtonPressed();
		virtual void set_minuteButtonPressed();
		virtual void button_backButtonPressed_save();
		virtual void water_change_nowButtonPressed();	
		
protected:		
		int8_t b_num;
		char in_buf[21];

		uint8_t source_ct;
		Layer_SettingTypeDef layer_settings;

		touchgfx::Callback<Screen2View, const Container&, const ClickEvent&> ContainerClickCallback;//!
		touchgfx::Callback<Screen2View, const ButtonWithLabel&, const ClickEvent&> ButtonWithLabelClickCallback;//!

		Draggable<Lens> myLens1;

		void input_buf(char in,Unicode::UnicodeChar* buffer);
};

#endif // SCREEN2VIEW_HPP
