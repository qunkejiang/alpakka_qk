/*
 * SPDX-FileCopyrightText: 2020-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/gpio.h"
#include "esp_private/gpio.h"
#include "matrix_keyboard.h"


static const char *TAG = "mkbd";

void get_matrix_kbd_updata(uint8_t *data)
{
    //memcpy(data, key_value, config->nr_col_gpios * sizeof(uint8_t));
}
bool matrix_kbd::ScanKeyboardColumns(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data) 
{    
    static uint8_t i=0;
    if (user_data) {
        matrix_kbd* self = static_cast<matrix_kbd*>(user_data);

        self->key_value[i] = dedic_gpio_bundle_read_in(self->mkbd->row_bundle);
        if(++i>=self->mkbd->nr_col_gpios)
        {
            i=0;
        }
        dedic_gpio_bundle_write(self->mkbd->col_bundle, (1 << self->mkbd->nr_col_gpios) - 1, 1<<i);
        return 0;
    }
    return 1;
}



void matrix_kbd::InitializeKeyboardTimer(uint16_t count) {
    gptimer_handle_t gptimer_handle = NULL; // 定时器句柄

    // 配置定时器
    gptimer_config_t gptimer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT, // 配置时钟源
        .direction = GPTIMER_COUNT_UP,      // 选择模式为向上计数
        .resolution_hz = 1000000,      // 分辨率
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&gptimer_config, &gptimer_handle));

    // 配置警报
    gptimer_alarm_config_t gptimer_alarm_config = {
        .alarm_count = (uint64_t)(1000/count),
        .reload_count = 0,                       // 重装值
        .flags=
        {
            .auto_reload_on_alarm = true,     // 开启循环报警模式（重加载）
        },
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer_handle, &gptimer_alarm_config));

    // 配置回调函数
    gptimer_event_callbacks_t ecallbacks = {
        .on_alarm =  matrix_kbd::ScanKeyboardColumns,
    };
    
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer_handle, &ecallbacks, this));

    // 使能定时器
    ESP_ERROR_CHECK(gptimer_enable(gptimer_handle));

    // 开启定时器
    ESP_ERROR_CHECK(gptimer_start(gptimer_handle));

}



matrix_kbd::matrix_kbd(matrix_kbd_config_t *config)//
{
    esp_err_t ret = ESP_OK;
    key_value = (uint8_t *)malloc(config->nr_col_gpios * sizeof(uint8_t));

    mkbd = (matrix_kbd_t *)calloc(1, sizeof(matrix_kbd_t) + (config->nr_row_gpios) * sizeof(uint32_t));

    mkbd->nr_row_gpios = config->nr_row_gpios;
    mkbd->nr_col_gpios = config->nr_col_gpios;

    dedic_gpio_bundle_config_t bundle_row_config = {
        .gpio_array = config->row_gpios,
        .array_size = config->nr_row_gpios,
        // Each GPIO used in matrix key board should be able to input and output
        .flags = {
            .in_en = 1,
            .in_invert = 1,
            .out_en = 0,
            .out_invert = 0,
        },
    };
    dedic_gpio_bundle_config_t bundle_col_config = {
        .gpio_array = config->col_gpios,
        .array_size = config->nr_col_gpios,
        .flags = {
            .in_en = 0,
            .in_invert = 0,
            .out_en = 1,
            .out_invert = 1,
        },
    };
    ESP_GOTO_ON_ERROR(dedic_gpio_new_bundle(&bundle_row_config, &mkbd->row_bundle), err, TAG, "create row bundle failed");
    ESP_GOTO_ON_ERROR(dedic_gpio_new_bundle(&bundle_col_config, &mkbd->col_bundle), err, TAG, "create col bundle failed");


    for (int i = 0; i < config->nr_row_gpios; i++) {
        gpio_pullup_en((gpio_num_t)config->row_gpios[i]);
        gpio_od_enable((gpio_num_t)config->row_gpios[i]);
    }
    for (int i = 0; i < config->nr_col_gpios; i++) {
        gpio_pullup_en((gpio_num_t)config->col_gpios[i]);
        gpio_od_enable((gpio_num_t)config->col_gpios[i]);
    }    

    InitializeKeyboardTimer(config->nr_col_gpios);

    return;
    
err:

    if (mkbd->col_bundle) {
        dedic_gpio_del_bundle(mkbd->col_bundle);
    }
    if (mkbd->row_bundle) {
        dedic_gpio_del_bundle(mkbd->row_bundle);
    }
    free(mkbd);
}

