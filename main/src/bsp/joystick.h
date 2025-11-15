#ifndef JOYSTICK_C_H
#define JOYSTICK_C_H

#include "esp_adc/adc_continuous.h"

#define JOYSTICK_CHANNEL_NUM 4
#define JOYSTICK_READ_LEN (JOYSTICK_CHANNEL_NUM*4)
#define THUMBSTICK_BASELINE_SATURATION 1.65

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


typedef struct _joystick_position_t {
    float x;
    float y;
    float angle;
    float radius;
} joystick_position_t;

class joystick {
private:
    adc_continuous_handle_t handle = NULL;
    void InitContinuousADC(adc_channel_t *channel, uint8_t channel_num, adc_continuous_handle_t *out_handle);

public:
    uint8_t calibration_step;
    float smoothed[4]={0.0f};
    int16_t adc_raw[JOYSTICK_CHANNEL_NUM] = {0};
    joystick_position_t position[JOYSTICK_CHANNEL_NUM/2];
    joystick_dir_t GetJoystickDir(float x, float y);
    esp_err_t update(int16_t *offset);
    joystick(adc_channel_t *channel, uint8_t channel_num);
    void calibration(int16_t *off);
};

#endif // JOYSTICK_C_H
