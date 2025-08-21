#ifndef NVM_C_H
#define NVM_C_H

#include "nvs_flash.h"
#include "nvs.h"



typedef struct  {
    int16_t Joystick_offset[4];
}nvm_data_t;

class NVM {
private:
    nvs_handle_t nvs_handle;
    void data_init();
public:
    nvm_data_t nvm_data;
    esp_err_t save();
    NVM();
    ~NVM();
};

#endif
