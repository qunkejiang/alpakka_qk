#include "joystick.h"

#define TAG "joystick"
      


void joystick::InitContinuousADC(adc_channel_t *channel, uint8_t channel_num, adc_continuous_handle_t *out_handle)
{
    adc_continuous_handle_t handle_d = NULL;

    adc_continuous_handle_cfg_t adc_config = {
        .max_store_buf_size = JOYSTICK_READ_LEN * 4,
        .conv_frame_size = JOYSTICK_READ_LEN,
        .flags = {
            .flush_pool = 0,  // 初始化 flush_pool 为 0
        }
    };
    ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_config, &handle_d));

    adc_digi_pattern_config_t adc_pattern[SOC_ADC_PATT_LEN_MAX];
    for (int i = 0; i < channel_num; i++) {
        adc_pattern[i].atten = ADC_ATTEN_DB_6;
        adc_pattern[i].channel = channel[i] ;
        adc_pattern[i].unit = ADC_UNIT_1;
        adc_pattern[i].bit_width = SOC_ADC_DIGI_MAX_BITWIDTH;

        ESP_LOGI(TAG, "adc_pattern[%d].atten is :%x", i, adc_pattern[i].atten);
        ESP_LOGI(TAG, "adc_pattern[%d].channel is :%x", i, adc_pattern[i].channel);
        ESP_LOGI(TAG, "adc_pattern[%d].unit is :%x", i, adc_pattern[i].unit);
    }

    adc_continuous_config_t dig_cfg = {
        .sample_freq_hz = 4000,
        .conv_mode = ADC_CONV_SINGLE_UNIT_1,
        .format = ADC_DIGI_OUTPUT_FORMAT_TYPE2,
    };
    dig_cfg.pattern_num = channel_num;
    dig_cfg.adc_pattern = adc_pattern;
    ESP_ERROR_CHECK(adc_continuous_config(handle_d, &dig_cfg));

    *out_handle = handle_d;
}

esp_err_t joystick::ReadADCValues(int16_t *adc_value) 
{
    esp_err_t ret;
    uint8_t result[JOYSTICK_READ_LEN];
    uint32_t ret_num;
    ret = adc_continuous_read(handle, result, JOYSTICK_READ_LEN, &ret_num, 0);
    if (ret == ESP_OK)
    {
        adc_digi_output_data_t *p = reinterpret_cast<adc_digi_output_data_t*>(result);
        
        for (int i = 0; i < 4; ++i) {
            // 从 ADC 数据中提取值并存储到 adc_value 数组
            adc_value[i] = static_cast<int16_t>(p->type2.data);
            ++p;
        }
    }
    return ret; // No data read
}
joystick::joystick(adc_channel_t *channel, uint8_t channel_num) {
    ESP_LOGI(TAG, "Initializing controller input system");
    InitContinuousADC(channel, channel_num, &handle);
    ESP_ERROR_CHECK(adc_continuous_start(handle));
}


