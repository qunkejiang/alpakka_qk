#include "backlight.h"
// #include "settings.h"

#include <esp_log.h>
#include <driver/ledc.h>

#define TAG "Backlight"



PwmBacklight::PwmBacklight(gpio_num_t pin, bool output_invert,ledc_timer_t  timer_num,ledc_channel_t channel) 
{
    const ledc_timer_config_t backlight_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .timer_num = timer_num,
        .freq_hz = 25000, 
        .clk_cfg = LEDC_AUTO_CLK,
        .deconfigure = false
    };
    ESP_ERROR_CHECK(ledc_timer_config(&backlight_timer));

    // Setup LEDC peripheral for PWM backlight control
    const ledc_channel_config_t backlight_channel = {
        .gpio_num = pin,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = channel,//
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = timer_num,
        .duty = 0,
        .hpoint = 0,
        .sleep_mode = LEDC_SLEEP_MODE_NO_ALIVE_NO_PD,
        .flags = {
            .output_invert = output_invert,
        }
    };
    ESP_ERROR_CHECK(ledc_channel_config(&backlight_channel));
}

PwmBacklight::~PwmBacklight() {
    ledc_stop(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
}

void PwmBacklight::SetBrightnessImpl(uint16_t brightness) {
    // LEDC resolution set to 10bits, thus: 100% = 1023
    //uint32_t duty_cycle = (1023 * brightness) / 100;
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, brightness);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

