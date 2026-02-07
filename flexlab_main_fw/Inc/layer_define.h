typedef struct __Layer_SettingTypeDef
{
	uint8_t enable_status;
	
	uint8_t light_status;
	uint8_t light_ontime;
	uint8_t light_offtime;
	
	uint16_t cycle_schedule;
	uint8_t cycle_schedule_hour_flag;
	
	uint8_t water_change_mode;
	uint16_t water_change_schedule;
	uint8_t water_change_schedule_hour_flag;
	
	uint32_t nus1_volume;
	uint32_t nus2_volume;
	uint32_t nus3_volume;
	
	uint8_t vent_mode;
	uint16_t vent_schedule;
	uint8_t vent_schedule_hour_flag;
} Layer_SettingTypeDef;

//typedef struct __Layer_Status
//{
//	int8_t temp[100];
//	
//	uint8_t layer_state;
//	
//	uint8_t light_status;

//	uint8_t cycle_status;
//} Layer_StatusTypeDef;

typedef struct __q_Layer_Status
{
	int8_t temp;
	
	int8_t layer_state;
	
	uint8_t light_status;

	uint8_t cycle_status;
	
	uint8_t layer_num;
} q_Layer_StatusTypeDef;

typedef struct __q_Carbin_Status
{
	int8_t temp;
	
	uint8_t level_status;
} q_Carbin_StatusTypeDef;
