#include "joystick.h"
#include <cmath>

#define TAG "joystick"
      
#define DEADZONE_RADIUS_UP 0.3   // 圆形死区半径
#define DEADZONE_RADIUS_DOWN 0.2   // 圆形死区半径
//
joystick_dir_t joystick::GetJoystickDir(float x, float y)
{
    joystick_dir_t dir = JOYSTICK_DIR_NONE;
    
    // 圆形死区处理 (向量模长小于阈值判定为中立)
    float distance = std::sqrtf(x*x + y*y);
    if (distance < DEADZONE_RADIUS_DOWN) {
        dir = JOYSTICK_DIR_CENTER;
    }else  if (distance > DEADZONE_RADIUS_UP) 
    {
        float angle = std::atan2f(x,y ) * 180.0 / M_PI;
        if (angle > -20 && angle <= 20) {
            dir = JOYSTICK_DIR_UP;
        }else if (angle > 25 && angle <= 65) {
            dir = JOYSTICK_DIR_UP_RIGHT;
        }else if (angle > 70 && angle <= 110) {
            dir = JOYSTICK_DIR_RIGHT;
        }else if (angle > 115 && angle <= 155) {
            dir = JOYSTICK_DIR_DOWN_RIGHT;
        }else if (angle > 160 || angle <= -160) {
            dir = JOYSTICK_DIR_DOWN;
        }else if (angle > -155 && angle <= -115) {
            dir = JOYSTICK_DIR_DOWN_LEFT;
        }else if (angle > -110 && angle <= -70) {
            dir = JOYSTICK_DIR_LEFT;
        }else if (angle > -65 && angle <= -25) {
            dir = JOYSTICK_DIR_UP_LEFT;
        }
    }
    return dir;
}

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

        // ESP_LOGI(TAG, "adc_pattern[%d].atten is :%x", i, adc_pattern[i].atten);
        // ESP_LOGI(TAG, "adc_pattern[%d].channel is :%x", i, adc_pattern[i].channel);
        // ESP_LOGI(TAG, "adc_pattern[%d].unit is :%x", i, adc_pattern[i].unit);
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

esp_err_t joystick::GetADCValues(float *adc_value,int16_t *offset,int16_t *gain) 
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
            adc_raw[i] = p->type2.data;
            adc_value[i] = (adc_raw[i]-offset[i])/static_cast<float>(gain[i]);
            if(adc_value[i]>1.0)adc_value[i]=1.0;
            if(adc_value[i]<-1.0)adc_value[i]=-1.0;
            ++p;
        }
    }else{
        ESP_LOGI(TAG, "GetADCValues failed");
    }
    return ret; // No data read
}
void joystick::calibration_setp1()
{
    for (int i = 0; i < JOYSTICK_CHANNEL_NUM; i++)
    {
        calibration_l[i]=calibration_h[i]=2000;
    }
}
void joystick::calibration_setp2()
{
    for (int i = 0; i < JOYSTICK_CHANNEL_NUM; i++)
    {
        if(adc_raw[i]<calibration_l[i])calibration_l[i]=adc_raw[i];
        if(adc_raw[i]>calibration_h[i])calibration_h[i]=adc_raw[i];
    }
}

void joystick::calibration_setp3(int16_t *offset,int16_t *gain)
{
    for (int i = 0; i < JOYSTICK_CHANNEL_NUM; i++)
    {
        offset[i]=(calibration_l[i]+calibration_h[i])/2;
        gain[i]=(calibration_h[i]-calibration_l[i])/2;
        ESP_LOGI(TAG, "calibration offset[%d] is :%d",i,offset[i]);
        ESP_LOGI(TAG, "calibration gain[%d] is :%d",i,gain[i]);
    }
    gain[2]=-gain[2];
}
joystick::joystick(adc_channel_t *channel, uint8_t channel_num) {
    ESP_LOGI(TAG, "Initializing controller input system");
    InitContinuousADC(channel, channel_num, &handle);
    ESP_ERROR_CHECK(adc_continuous_start(handle));
}


