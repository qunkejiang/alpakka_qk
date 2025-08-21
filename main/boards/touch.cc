#include "touch.h"


esp_err_t Touch::GetTouchValue(uint32_t *data)

{
    return touch_sensor_lowlevel_get_data(channel, data);
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