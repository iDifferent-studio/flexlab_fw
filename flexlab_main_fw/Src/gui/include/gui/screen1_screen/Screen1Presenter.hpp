#ifndef SCREEN1PRESENTER_HPP
#define SCREEN1PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen1View;

class Screen1Presenter : public touchgfx::Presenter, public ModelListener
{
public:
    Screen1Presenter(Screen1View& v);

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

		void set_enter_layer(uint8_t in)
		{
				model->set_enter_layer(in);
		}
		
	void get_layer_status(Layer_StatusTypeDef * status, uint8_t layer)
	{
		model->get_layer_status(status, layer);
	}
	
	void set_layer_status(Layer_StatusTypeDef * status, uint8_t layer)
	{
		model->set_layer_status(status, layer);
	}
	
	void inject_layer_temp(int8_t input, uint8_t layer)
	{
		model->inject_layer_temp(input, layer);
	}
	
	int8_t get_latest_layer_temp(uint8_t layer)
	{
		return model->get_latest_layer_temp(layer);
	}

    virtual ~Screen1Presenter() {};

private:
    Screen1Presenter();

    Screen1View& view;
};

#endif // SCREEN1PRESENTER_HPP
