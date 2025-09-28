
#include "actions.h"
#include "screens.h"
#include "vars.h"
#include <esp_log.h>
#include "board.h"

static const char *TAG = "actions";
void action_load_options(lv_event_t * e)
{
    NVM * nvm = Board::GetInstance().nvm_;
    lv_roller_set_selected(objects.menu1_roller, nvm->nvm_data.mode,LV_ANIM_ON);
    set_var_mode_state(nvm->nvm_data.mode);
    lv_slider_set_value(objects.backlight, nvm->nvm_data.backlight,LV_ANIM_ON);
    lv_roller_set_selected(objects.language, nvm->nvm_data.language,LV_ANIM_ON);
    set_var_bt_state(nvm->nvm_data.bt_state);
    lv_roller_set_selected(objects.menu3_roller, nvm->nvm_data.Configuration,LV_ANIM_ON);
}
void action_save_options(lv_event_t * e)
{
    NVM * nvm = Board::GetInstance().nvm_;
    nvm->nvm_data.mode = lv_roller_get_selected(objects.menu1_roller);
    set_var_mode_state(nvm->nvm_data.mode);
    nvm->nvm_data.backlight = lv_slider_get_value(objects.backlight);
    nvm->nvm_data.language = lv_roller_get_selected(objects.language);
    nvm->nvm_data.bt_state = get_var_bt_state();
    nvm->nvm_data.Configuration = lv_roller_get_selected(objects.menu3_roller);
    nvm->save();
    ESP_LOGI(TAG, "action_save_options");
}    
void action_backlight_action(lv_event_t *e) {
    // TODO: Implement action backlight_action here
    int32_t backlight = lv_slider_get_value(objects.backlight);
    Board::GetInstance().backlight_->SetBrightnessImpl(50+backlight*40);
}
void action_joystick_calibration_setp1(lv_event_t * e)
{
    Board::GetInstance().joystick_->calibration_setp1();
    ESP_LOGI(TAG, "action_calibration_setp1");
}
void action_joystick_calibration_setp3(lv_event_t * e)
{
    NVM * nvm = Board::GetInstance().nvm_;
    Board::GetInstance().joystick_->calibration_setp3(nvm->nvm_data.Joystick_offset,nvm->nvm_data.Joystick_gain);
    ESP_LOGI(TAG, "action_calibration_setp3");
}
void action_touch_calibration1(lv_event_t * e)
{
    NVM * nvm = Board::GetInstance().nvm_;
    lv_spinbox_set_value(objects.menu7_sv, nvm->nvm_data.touch_trigger);
    ESP_LOGI(TAG, "action_touch_calibration1");
}
void action_touch_calibration2(lv_event_t * e)
{
    NVM * nvm = Board::GetInstance().nvm_;
    nvm->nvm_data.touch_trigger = lv_spinbox_get_value(objects.menu7_sv);
    ESP_LOGI(TAG, "action_touch_calibration2");
}