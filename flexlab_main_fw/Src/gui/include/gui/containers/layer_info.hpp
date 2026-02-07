#ifndef LAYER_INFO_HPP
#define LAYER_INFO_HPP

#include <gui_generated/containers/layer_infoBase.hpp>

class layer_info : public layer_infoBase
{
public:
    layer_info();
	layer_info(uint8_t layer_number);
    virtual ~layer_info() {}

    virtual void initialize();
		
	void set_layer_number(uint8_t layer_number);
	void set_layer_infomation(int8_t state, int8_t temp, bool light_s, bool cyc_s, bool wq_state);
			
	bool is_available;
protected:
};

#endif // LAYER_INFO_HPP
