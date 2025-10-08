#ifndef HAPTIC_MOTOR_C_H
#define HAPTIC_MOTOR_C_H

#include <driver/ledc.h>

struct haptic_config_t
{
    int gpio_num_left;
    int gpio_num_right;
    ledc_channel_t ledc_channel_left;
    ledc_channel_t ledc_channel_right;
    ledc_timer_t  timer_num;
};


class Haptic_Motor {
public:
    void play_audio(const uint8_t *data, size_t wave_size, uint32_t rate, ledc_timer_bit_t bits, uint32_t ch);
    void set_volume(int8_t volume);
    Haptic_Motor(haptic_config_t *config);
    ~Haptic_Motor();
};

#endif
