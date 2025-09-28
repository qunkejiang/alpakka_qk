#ifndef TOUCH_C_H
#define TOUCH_C_H

#include "touch_sensor_lowlevel.h"



struct touch_data_t
{
    uint32_t touch_value = 0;
    bool touch_status = false;
};

class Touch {
private:
    uint8_t channel;
public:
    void GetTouchValue(touch_data_t *data,uint32_t trigger);
    Touch(touch_lowlevel_config_t *config);
    ~Touch();
};

#endif
