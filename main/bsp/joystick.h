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

//枚举方向
enum joystick_dir_t 
{
    JOYSTICK_DIR_CENTER= 0,
    JOYSTICK_DIR_UP,
    JOYSTICK_DIR_UP_RIGHT,
    JOYSTICK_DIR_RIGHT,
    JOYSTICK_DIR_DOWN_RIGHT,
    JOYSTICK_DIR_DOWN,
    JOYSTICK_DIR_DOWN_LEFT,
    JOYSTICK_DIR_LEFT,
    JOYSTICK_DIR_UP_LEFT,
    JOYSTICK_DIR_NONE ,
};
class joystick {
private:
    adc_continuous_handle_t handle = NULL;
    void InitContinuousADC(adc_channel_t *channel, uint8_t channel_num, adc_continuous_handle_t *out_handle);
    int16_t calibration_l[JOYSTICK_CHANNEL_NUM] = {0};
    int16_t calibration_h[JOYSTICK_CHANNEL_NUM] = {0};

public:
    int16_t adc_raw[JOYSTICK_CHANNEL_NUM] = {0};
    joystick_dir_t GetJoystickDir(float x, float y);
    esp_err_t GetADCValues(float *adc_value,int16_t *offset,int16_t *gain);
    joystick(adc_channel_t *channel, uint8_t channel_num);
    void calibration_setp1();
    void calibration_setp2();
    void calibration_setp3(int16_t *offset,int16_t *gain);
};

#endif // JOYSTICK_C_H
