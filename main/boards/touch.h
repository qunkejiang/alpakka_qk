#ifndef TOUCH_C_H
#define TOUCH_C_H

#include "touch_sensor_lowlevel.h"
class Touch {
private:
    uint8_t channel;
public:
    esp_err_t GetTouchValue(uint32_t *data);
    Touch(touch_lowlevel_config_t *config);
    ~Touch();
};

#endif
