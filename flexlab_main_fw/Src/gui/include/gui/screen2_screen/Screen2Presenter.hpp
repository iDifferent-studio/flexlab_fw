#ifndef SCREEN2PRESENTER_HPP
#define SCREEN2PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen2View;

class Screen2Presenter : public touchgfx::Presenter, public ModelListener
{
public:
    Screen2Presenter(Screen2View& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

		uint8_t get_enter_layer()
		{
				return model->get_enter_layer();
		}
		
		void get_layer_setting(Layer_SettingTypeDef * settings, uint8_t layer)
		{
				model->get_layer_setting(settings, layer);
		}
		
		void set_layer_setting(Layer_SettingTypeDef * settings, uint8_t layer)
		{
				model->set_layer_setting(settings, layer);
		}

    virtual ~Screen2Presenter() {};

private:
    Screen2Presenter();

    Screen2View& view;
};

#endif // SCREEN2PRESENTER_HPP
