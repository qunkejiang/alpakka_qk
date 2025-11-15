#ifndef NVM_C_H
#define NVM_C_H

#include "nvs_flash.h"

#include "nvs.h"
#include "ctrl.h"

#define NVM_CONTROL_BYTE 0b01010101

#define MAJOR 1000 * 1000
#define MINOR 1000
#define PATCH 1
#define NVM_PROFILE_VERSION       ((MAJOR * 1) + (MINOR * 0) + (PATCH * 0))
#define NVM_HOME_PROFILE_VERSION  ((MAJOR * 1) + (MINOR * 1) + (PATCH * 0))

#define NVM_PROFILE_SLOTS 14


typedef struct
{
    uint8_t usb:1;
    uint8_t cpu:1;
    uint8_t bt:1;
}LogMask;


typedef struct  {
    int16_t Joystick_offset[4];
    float gyro_offset[3];
    uint32_t touch_trigger;
    uint8_t language;
    uint8_t backlight;
    uint8_t log_level;
    LogMask log_mask;

    uint8_t profile_index;
    uint8_t usb_protocol;
    uint8_t sens_mouse;
    uint8_t deadzone;

    bool long_calibration;
    bool swap_gyros;
    bool touch_invert_polarity;
    bool nc;
    int8_t offset_gyro_user_x;
    int8_t offset_gyro_user_y;
    int8_t offset_gyro_user_z;
    uint8_t thumbstick_smooth_samples;
    float sens_mouse_values[3];
    float deadzone_values[3];
    uint16_t CRC;
}Config_t;


class NVM {
private:
    nvs_handle_t nvs_handle;
    void data_init();
public:
    CtrlProfile *config_profiles;
    Config_t nvm_data;
    esp_err_t save_nvm_data();
    void data_default();
    void profile_overwrite(uint8_t indexTo, int8_t indexFrom);
    void config_profile_default(uint8_t indexTo, int8_t indexFrom);
    void save_profile(uint8_t index);
    void config_profile_default_all();

    NVM();
    ~NVM();
};




#endif
