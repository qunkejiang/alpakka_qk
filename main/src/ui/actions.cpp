
#include "actions.h"
#include "screens.h"
#include "vars.h"
#include "logging.h"
#include "board.h"


void action_load_profile_names(lv_event_t * e)//预留
{

}
void action_load_options(lv_event_t * e)
{
    CtrlProfile *config_profiles = Board::GetInstance().nvm_->config_profiles;
    char profile_names[336];
    uint16_t profile_names_len = 0;
    for(int i=PROFILE_FPS_FUSION;i<=PROFILE_CUSTOM_4;i++){
        profile_names_len += snprintf(&profile_names[profile_names_len], 24, "%s\n", config_profiles[i].sections[SECTION_META].meta.name);
    }
    profile_names[profile_names_len-1] = '\0';
    lv_roller_set_options(objects.menu3_roller, profile_names, LV_ROLLER_MODE_INFINITE);
    Config_t * config = Board::get_nvm_data();
    lv_roller_set_selected(objects.menu1_roller, config->usb_protocol,LV_ANIM_ON);
    lv_slider_set_value(objects.backlight, config->backlight,LV_ANIM_ON);
    lv_roller_set_selected(objects.language, config->language,LV_ANIM_ON);
    set_var_bt_state(config->log_mask.bt);
    lv_roller_set_selected(objects.menu3_roller, config->profile_index-1,LV_ANIM_ON);
    logging::info("action_load_options\n");
}
void action_save_options(lv_event_t * e)
{
    NVM * nvm = Board::GetInstance().nvm_;
    Config_t * nvm_data = &nvm->nvm_data;
    nvm_data->backlight = lv_slider_get_value(objects.backlight);
    nvm_data->language = lv_roller_get_selected(objects.language);
    nvm_data->log_mask.bt = get_var_bt_state();
    uint32_t mode =lv_roller_get_selected(objects.menu1_roller);
    uint32_t profile_index = lv_roller_get_selected(objects.menu3_roller)+1;
    if(profile_index != nvm_data->profile_index)
    {
        Board::set_profile(profile_index);
    }
    if(mode != nvm_data->usb_protocol)
    {
        nvm_data->usb_protocol = mode;
    }
    nvm->save_nvm_data();
    logging::info("action_save_options\n");
}    
void action_backlight_action(lv_event_t *e) {
    // TODO: Implement action backlight_action here
    int32_t backlight = lv_slider_get_value(objects.backlight);
    Board::GetInstance().backlight_->SetBrightnessImpl(50+backlight*40);
}
void action_touch_calibration1(lv_event_t * e)
{
    Config_t * config = Board::get_nvm_data();
    lv_spinbox_set_value(objects.menu7_sv, config->touch_trigger);
    logging::info("action_touch_calibration1\n");
}
void action_touch_calibration2(lv_event_t * e)
{
    Config_t * config = Board::get_nvm_data();
    config->touch_trigger = lv_spinbox_get_value(objects.menu7_sv);
    logging::info("action_touch_calibration2\n");
}