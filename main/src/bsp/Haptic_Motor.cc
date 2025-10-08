#include "Haptic_Motor.h"
#include "pwm_audio.h"
#include <esp_log.h>

static const char *TAG = "Haptic_Motor";

void Haptic_Motor::set_volume(int8_t volume)
{
    pwm_audio_set_volume(volume);
}

void Haptic_Motor::play_audio(const uint8_t *data, size_t wave_size, uint32_t rate, ledc_timer_bit_t bits, uint32_t ch)
{
    uint32_t index = 0;
    size_t cnt;
    uint32_t block_w = 2048;
    //ESP_LOGI(TAG, "parameter: samplerate:%"PRIu32", bits:%"PRIu32", channel:%"PRIu32"", rate, bits, ch);
    pwm_audio_set_param(rate, bits, ch);
    pwm_audio_start();

    while (1) {
        if (index < wave_size) {
            if ((wave_size - index) < block_w) {
                block_w = wave_size - index;
            }
            pwm_audio_write((uint8_t*)data + index, block_w, &cnt, 1000 / portTICK_PERIOD_MS);
            //ESP_LOGD(TAG, "write [%"PRIu32"] [%ld]", block_w, cnt);
            index += cnt;
        } else {
            ESP_LOGI(TAG, "play completed");
            vTaskDelay(500 / portTICK_PERIOD_MS);
            break;
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    pwm_audio_stop();
}
Haptic_Motor::Haptic_Motor(haptic_config_t *config)
{
    pwm_audio_config_t pwm_config = {
        .gpio_num_left = config->gpio_num_left,
        .gpio_num_right = config->gpio_num_right,
        .ledc_channel_left = config->ledc_channel_left,
        .ledc_channel_right = config->ledc_channel_right,
        .ledc_timer_sel = config->timer_num,
        .duty_resolution = LEDC_TIMER_10_BIT,    /*!< ledc pwm bits */
        .ringbuf_len = 1024 * 8                /*!< ringbuffer size */
    };
    pwm_audio_init(&pwm_config);

    pwm_audio_set_volume(15);
    // play_audio(wave_array_32000_8_1, 64000, 32000, LEDC_TIMER_8_BIT, 1);
    // vTaskDelay(1000 / portTICK_PERIOD_MS);
    // play_audio(wave_array_32000_8_2, 128000, 32000, LEDC_TIMER_8_BIT, 2);
    // vTaskDelay(1000 / portTICK_PERIOD_MS);
    // play_audio(wave_array_32000_8_1, 64000, 32000, LEDC_TIMER_8_BIT, 2);
    // vTaskDelay(1000 / portTICK_PERIOD_MS);
    // play_audio(wave_array_32000_8_2, 128000, 32000, LEDC_TIMER_8_BIT, 2);
}

Haptic_Motor::~Haptic_Motor()
{
    pwm_audio_deinit();
}