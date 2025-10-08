#include "rotary.h"
#include "esp_log.h"
#include <cstring>

#define PCNT_HIGH_LIMIT 100
#define PCNT_LOW_LIMIT  -100


#define TAG "Rotary"


void Rotary::set_mode(uint8_t value) {
    self.mode = value;
}

void Rotary::report() {
    int count;
    pcnt_unit_get_count(pcnt_unit, &count);
    int8_t diff = count - self.count_old;
    if (diff >25) diff-=49;
    else if (diff <-25) diff+=49;
    for(int8_t i=0; i<abs(diff); i++)
    {
        uint8_t *actions = (
            diff > 0 ?
            self.actions[self.mode][ROTARY_UP] :
            self.actions[self.mode][ROTARY_DOWN]
        );
        // hid_press_multiple(actions);
        // hid_release_multiple_later(actions, 10);
    }
    self.count_old = count;
}


void Rotary::config_mode(
    uint8_t mode,
    Actions actions_up,
    Actions actions_down
){
    memcpy(self.actions[mode][ROTARY_UP], actions_up, ACTIONS_LEN);
    memcpy(self.actions[mode][ROTARY_DOWN], actions_down, ACTIONS_LEN);
//     rotary.pending = false;
//    self.mode = mode;
//    rotary.increment = 0;
//     rotary.timestamp = 0;
}


esp_err_t Rotary::GetPulseCounterValue(int* count)
{
    return pcnt_unit_get_count(pcnt_unit, count);
}


void Rotary::InitializePulseCounter(int edge_a, int edge_b)
{
    // Initialization code for Touch can be added here if needed
    ESP_LOGI(TAG, "install pcnt unit");
    pcnt_unit_config_t unit_config = {
        .low_limit = PCNT_LOW_LIMIT,
        .high_limit = PCNT_HIGH_LIMIT,
    };
    ESP_ERROR_CHECK(pcnt_new_unit(&unit_config, &pcnt_unit));

    ESP_LOGI(TAG, "set glitch filter");
    pcnt_glitch_filter_config_t filter_config = {
        .max_glitch_ns = 1000,
    };
    ESP_ERROR_CHECK(pcnt_unit_set_glitch_filter(pcnt_unit, &filter_config));

    ESP_LOGI(TAG, "install pcnt channels");
    pcnt_chan_config_t chan_a_config = {
        .edge_gpio_num = edge_a,
        .level_gpio_num = edge_b,
    };
    pcnt_channel_handle_t pcnt_chan_a = NULL;
    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_a_config, &pcnt_chan_a));
    pcnt_chan_config_t chan_b_config = {
        .edge_gpio_num = edge_b,
        .level_gpio_num = edge_a,
    };
    pcnt_channel_handle_t pcnt_chan_b = NULL;
    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_b_config, &pcnt_chan_b));

    ESP_LOGI(TAG, "set edge and level actions for pcnt channels");
    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_chan_a, PCNT_CHANNEL_EDGE_ACTION_DECREASE, PCNT_CHANNEL_EDGE_ACTION_INCREASE));
    ESP_ERROR_CHECK(pcnt_channel_set_level_action(pcnt_chan_a, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE));
    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_chan_b, PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_EDGE_ACTION_DECREASE));
    ESP_ERROR_CHECK(pcnt_channel_set_level_action(pcnt_chan_b, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE));

    ESP_LOGI(TAG, "add watch points and register callbacks");
    int watch_points[] = {PCNT_LOW_LIMIT, -50, 0, 50, PCNT_HIGH_LIMIT};
    for (size_t i = 0; i < sizeof(watch_points) / sizeof(watch_points[0]); i++) {
        ESP_ERROR_CHECK(pcnt_unit_add_watch_point(pcnt_unit, watch_points[i]));
    }
    ESP_LOGI(TAG, "enable pcnt unit");
    ESP_ERROR_CHECK(pcnt_unit_enable(pcnt_unit));
    ESP_LOGI(TAG, "clear pcnt unit");
    ESP_ERROR_CHECK(pcnt_unit_clear_count(pcnt_unit));
    ESP_LOGI(TAG, "start pcnt unit");
    ESP_ERROR_CHECK(pcnt_unit_start(pcnt_unit));

}

Rotary::Rotary(int edge_a, int edge_b)
{
    InitializePulseCounter(edge_a, edge_b);
}


