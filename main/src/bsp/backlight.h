#ifndef BACKLIGHT_H
#define BACKLIGHT_H

#include <cstdint>
#include <functional>

#include <driver/gpio.h>
#include <esp_timer.h>
#include <driver/ledc.h>




class PwmBacklight{
public:
    PwmBacklight(gpio_num_t pin, bool output_invert,ledc_timer_t  timer_num,ledc_channel_t channel);
    ~PwmBacklight();

    void SetBrightnessImpl(uint16_t brightness) ;
};
#endif // BACKLIGHT_H