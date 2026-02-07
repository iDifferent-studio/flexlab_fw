#ifndef MODEL_HPP
#define MODEL_HPP

#include <touchgfx/hal/types.hpp>
#include "layer_define.h"

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();
		
	void set_enter_layer(uint8_t in)
	{
			enter_layer_setting=in;
	}
	
	uint8_t get_enter_layer()
	{
			return enter_layer_setting;
	}
	
	void get_layer_setting(Layer_SettingTypeDef * settings, uint8_t layer)
	{
		settings->enable_status = layer_setting[layer].enable_status;
		
		settings->light_status = layer_setting[layer].light_status; 
		settings->light_ontime = layer_setting[layer].light_ontime;
		settings->light_offtime = layer_setting[layer].light_offtime;
		
		settings->cycle_schedule = layer_setting[layer].cycle_schedule;
		
		settings->water_change_mode = layer_setting[layer].water_change_mode;
		settings->water_change_schedule = layer_setting[layer].water_change_schedule;
		
		settings->nus1_volume = layer_setting[layer].nus1_volume;
		settings->nus2_volume = layer_setting[layer].nus2_volume;
		settings->nus3_volume = layer_setting[layer].nus3_volume;
		
		settings->vent_mode = layer_setting[layer].vent_mode;
		settings->vent_schedule = layer_setting[layer].vent_schedule;
		
		settings->cycle_schedule_hour_flag = layer_setting[layer].cycle_schedule_hour_flag;
		settings->water_change_schedule_hour_flag = layer_setting[layer].water_change_schedule_hour_flag;
		settings->vent_schedule_hour_flag = layer_setting[layer].vent_schedule_hour_flag;
	}
	
	void set_layer_setting(Layer_SettingTypeDef * settings, uint8_t layer)
	{
		layer_setting[layer].enable_status = settings->enable_status;
													               		     
		layer_setting[layer].light_status = settings->light_status; 
		layer_setting[layer].light_ontime = settings->light_ontime;
		layer_setting[layer].light_offtime = settings->light_offtime;
													                   		     
		layer_setting[layer].cycle_schedule = settings->cycle_schedule;
													                   				 
		layer_setting[layer].water_change_mode = settings->water_change_mode;
		layer_setting[layer].water_change_schedule = settings->water_change_schedule;
		                                             
		layer_setting[layer].nus1_volume = settings->nus1_volume;
		layer_setting[layer].nus2_volume = settings->nus2_volume;
		layer_setting[layer].nus3_volume = settings->nus3_volume;
		                                   
		layer_setting[layer].vent_mode = settings->vent_mode;
		layer_setting[layer].vent_schedule =  settings->vent_schedule;
		
		layer_setting[layer].cycle_schedule_hour_flag = settings->cycle_schedule_hour_flag;
		layer_setting[layer].water_change_schedule_hour_flag = settings->water_change_schedule_hour_flag;
		layer_setting[layer].vent_schedule_hour_flag = settings->vent_schedule_hour_flag;
	}
	
	void get_layer_status(Layer_StatusTypeDef * status, uint8_t layer)
	{
		status->layer_state=layer_status[layer].layer_state;
		
		status->light_status=layer_status[layer].light_status;
		
		status->cycle_status=layer_status[layer].cycle_status;
	}
	
	void set_layer_status(Layer_StatusTypeDef * status, uint8_t layer)
	{
		layer_status[layer].layer_state=status->layer_state;
		                                 
		layer_status[layer].light_status=status->light_status;
		                                 
		layer_status[layer].cycle_status=status->cycle_status;
	}
	
	void inject_layer_temp(int8_t input, uint8_t layer)
	{
		for(uint8_t i=0;i<100-1;i++)
		{
			layer_status[layer].temp[i]=layer_status[layer].temp[i+1];
		}
		
		layer_status[layer].temp[99]=input;
	}
	
	int8_t get_latest_layer_temp(uint8_t layer)
	{
		return layer_status[layer].temp[99];
	}
protected:
    ModelListener* modelListener;
		uint8_t enter_layer_setting;
		Layer_SettingTypeDef layer_setting[3];
		Layer_StatusTypeDef layer_status[3];
};

#endif // MODEL_HPP
