#include "touch.h"



void Touch::GetTouchValue(touch_data_t *data,uint32_t trigger)
{
    touch_sensor_lowlevel_get_data(channel, &data->touch_value);
    if(data->touch_value > trigger+50)
    {
        data->touch_status = true;
    }
    else if(data->touch_value < trigger-50)
    {
        data->touch_status = false;
    }
}


Touch::Touch(touch_lowlevel_config_t *config)
{
    channel = config->channel_list[0];
    ESP_ERROR_CHECK(touch_sensor_lowlevel_create(config));
    ESP_ERROR_CHECK(touch_sensor_lowlevel_start());
}
Touch::~Touch()
{
    ESP_ERROR_CHECK(touch_sensor_lowlevel_stop());
    ESP_ERROR_CHECK(touch_sensor_lowlevel_delete());
}