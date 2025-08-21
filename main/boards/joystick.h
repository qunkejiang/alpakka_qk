#ifndef JOYSTICK_C_H
#define JOYSTICK_C_H

#include <cstdint>
#include <cstring>
#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_adc/adc_continuous.h"

#define JOYSTICK_CHANNEL_NUM 4
#define JOYSTICK_READ_LEN (JOYSTICK_CHANNEL_NUM*4)

class joystick {
private:
    adc_continuous_handle_t handle = NULL;
    void InitContinuousADC(adc_channel_t *channel, uint8_t channel_num, adc_continuous_handle_t *out_handle);
public:
    int16_t adc_channel_offset[JOYSTICK_CHANNEL_NUM]; // Offset for each channel
    esp_err_t ReadADCValues(int16_t *adc_value);
    joystick(adc_channel_t *channel, uint8_t channel_num);
};

#endif // JOYSTICK_C_H
