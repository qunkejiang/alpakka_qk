#ifndef NVM_C_H
#define NVM_C_H

#include "nvs_flash.h"

#include "nvs.h"
#include "ctrl.h"

#define NVM_PROFILE_SLOTS 14

typedef struct  {
    int16_t Joystick_offset[4];
    int16_t Joystick_gain[4];
    float gyro_offset[3];
    uint32_t touch_trigger;
    uint8_t language;
    uint8_t backlight;
    uint8_t mode;
    uint8_t Configuration;
    uint8_t bt_state;
    uint8_t reserved[3];
    uint16_t CRC;
}nvm_data_t;


class NVM {
private:
    nvs_handle_t nvs_handle;
    void data_init();
    uint16_t get_crc();
public:
    nvm_data_t nvm_data;
    CtrlProfile *config_profiles;
    esp_err_t save();
    NVM();
    ~NVM();
};

#endif
