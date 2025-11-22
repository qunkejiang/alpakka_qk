#include "joystick.h"
#include "logging.h"
#include "board.h"
#include "common.h"

      

void joystick::InitContinuousADC(adc_channel_t *channel, uint8_t channel_num, adc_continuous_handle_t *out_handle)
{
    adc_continuous_handle_t handle_d = NULL;

    adc_continuous_handle_cfg_t adc_config = {
        .max_store_buf_size = JOYSTICK_READ_LEN * 16,
        .conv_frame_size = JOYSTICK_READ_LEN,
        .flags = {
            .flush_pool = 1,  
        }
    };
    ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_config, &handle_d));

    adc_digi_pattern_config_t adc_pattern[SOC_ADC_PATT_LEN_MAX];
    for (int i = 0; i < channel_num; i++) {
        adc_pattern[i].atten = ADC_ATTEN_DB_6;
        adc_pattern[i].channel = channel[i] ;
        adc_pattern[i].unit = ADC_UNIT_1;
        adc_pattern[i].bit_width = SOC_ADC_DIGI_MAX_BITWIDTH;

        // ESP_LOGI(TAG, "adc_pattern[%d].atten is :%x", i, adc_pattern[i].atten);
        // ESP_LOGI(TAG, "adc_pattern[%d].channel is :%x", i, adc_pattern[i].channel);
        // ESP_LOGI(TAG, "adc_pattern[%d].unit is :%x", i, adc_pattern[i].unit);
    }

    adc_continuous_config_t dig_cfg = {
        .pattern_num = channel_num,
        .adc_pattern = adc_pattern,
        .sample_freq_hz = 4000,
        .conv_mode = ADC_CONV_SINGLE_UNIT_1,
        .format = ADC_DIGI_OUTPUT_FORMAT_TYPE2,
    };
    ESP_ERROR_CHECK(adc_continuous_config(handle_d, &dig_cfg));

    *out_handle = handle_d;
}

esp_err_t joystick::update(int16_t *offset) 
{
    esp_err_t ret;
    uint8_t result[JOYSTICK_READ_LEN];
    uint32_t ret_num;
    float adc_value[4];
    ret = adc_continuous_read(handle, result, JOYSTICK_READ_LEN, &ret_num, 0);
    if (ret == ESP_OK)
    {
        adc_digi_output_data_t *p = reinterpret_cast<adc_digi_output_data_t*>(result);
        
        uint8_t thumbstick_smooth_samples = Board::GetInstance().nvm_->nvm_data.thumbstick_smooth_samples;
        for (int i = 0; i < 4; ++i) {
            // 从 ADC 数据中提取值并存储到 adc_value 数组
            adc_raw[i] = p[i].type2.data;
            adc_value[i] = (adc_raw[i]-offset[i])*(THUMBSTICK_BASELINE_SATURATION/2048.f);
            if(thumbstick_smooth_samples>0)
                smoothed[i] = smooth(smoothed[i],adc_value[i], (float)(thumbstick_smooth_samples));
            else
                smoothed[i]=adc_value[i];
        }
        position[0].x=smoothed[0];
        position[0].y=smoothed[1];
        position[1].x=-smoothed[2];
        position[1].y=smoothed[3];
        for (int i = 0; i < 2; ++i) {
            position[i].angle=degrees(atan2f(position[i].x,position[i].y));
            position[i].radius=sqrtf(position[i].x*position[i].x+position[i].y*position[i].y);
        }
        calibration(offset);
    }
    return ret; // No data read
}
void joystick::calibration(int16_t *off)
{
    static int32_t count = 0;
    static int32_t temporary[JOYSTICK_CHANNEL_NUM];
    switch (calibration_step)
    {
    case 1:
        calibration_step = 2;
        count = 0;
        for (int i = 0; i < JOYSTICK_CHANNEL_NUM; i++)
        {
            temporary[i] = 0.0f;
        }
        break;
    case 2:
        count++;
        if(count>=5000)
        {
            for (int i = 0; i < JOYSTICK_CHANNEL_NUM; i++)
            {
                off[i] = temporary[i]/5000;
                logging::info("joystick offset %d : %d\n", i, off[i]);
            }
            //Board::GetInstance().nvm_->save_nvm_data();
            calibration_step = 3;
        }else
        {
            for (int i = 0; i < JOYSTICK_CHANNEL_NUM; i++)
            {
                temporary[i] += adc_raw[i];
            }
        }
        break;
    default:
        break;
    }
}
joystick::joystick(adc_channel_t *channel, uint8_t channel_num) {
    logging::info("Initializing joystick\n");
    InitContinuousADC(channel, channel_num, &handle);
    ESP_ERROR_CHECK(adc_continuous_start(handle));
}


