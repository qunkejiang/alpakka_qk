#include "nvm.h"
#include "esp_log.h"
#include "profiles_default.h"
#include <cstring>
#include "Board.h"



void (*config_profile_default_func[])(CtrlProfile *profile) = {
    config_profile_default_home,
    config_profile_default_fps_fusion,
    config_profile_default_racing,
    config_profile_default_console,
    config_profile_default_desktop,
    config_profile_default_fps_wasd,
    config_profile_default_flight,
    config_profile_default_console_legacy,
    config_profile_default_rts,
    config_profile_default_custom,
    config_profile_default_custom,
    config_profile_default_custom,
    config_profile_default_custom,
    config_profile_default_console_legacy,
};
void NVM::config_profile_default(uint8_t indexTo, int8_t indexFrom)
{
    memset(&config_profiles[indexTo], 0, sizeof(CtrlProfile));
    config_profile_default_func[indexFrom](&(config_profiles[indexTo]));
    if (indexTo >= 9 && indexTo<=12) 
    {
        char *name = config_profiles[indexTo].sections[SECTION_META].meta.name;
        char custom_name[9];  // Custom=6 +space +digit +nullterm.
        snprintf(custom_name, 9, "Custom %i", indexTo-8);
        memcpy(name, custom_name, sizeof(custom_name));
    }
}
void NVM::save_profile(uint8_t index)
{
    char profile_name[12];
    sprintf(profile_name, "profile_%d", index);
    nvs_set_blob(nvs_handle, profile_name, &config_profiles[index], sizeof(CtrlProfile));//缓存数据
    nvs_commit(nvs_handle);//提交缓存数据
}
void NVM::config_profile_default_all()
{
    for (int i = 0; i < NVM_PROFILE_SLOTS; i++) {
        config_profile_default(i, i);
    }
    for (int i = 0; i < NVM_PROFILE_SLOTS; i++)
    {
        save_profile(i);
    }
}


esp_err_t NVM::save_nvm_data()
{
    //nvs_open("nvs", NVS_READWRITE, &nvs_handle);
    nvs_set_blob(nvs_handle, "nvm_data", &nvm_data, sizeof(Config_t));//缓存数据
    nvs_commit(nvs_handle);//提交缓存数据
    return ESP_OK;
}
void NVM::data_default()
{
    nvm_data.Joystick_offset[0] = 2000;
    nvm_data.Joystick_offset[1] = 2000;
    nvm_data.Joystick_offset[2] = 2000;
    nvm_data.Joystick_offset[3] = 2000;
    nvm_data.gyro_offset[0] = 0.0f;
    nvm_data.gyro_offset[1] = 0.0f;
    nvm_data.gyro_offset[2] = 0.0f;
    nvm_data.touch_trigger = 25000;
    nvm_data.language = 0;
    nvm_data.backlight = 10;
    nvm_data.profile_index = 1;
    nvm_data.usb_protocol = 0;
    nvm_data.sens_mouse = 0;
    nvm_data.sens_mouse_values[0] = 1.0f;
    nvm_data.sens_mouse_values[1] = 1.5f;
    nvm_data.sens_mouse_values[2] = 2.0f;
    nvm_data.deadzone = 0;
    nvm_data.deadzone_values[0] = 0.08f;
    nvm_data.deadzone_values[1] = 0.12f;
    nvm_data.deadzone_values[2] = 0.20f;
    nvm_data.log_mask = {.usb=0,.cpu=0,.bt=0};
    nvm_data.long_calibration = false;
    nvm_data.swap_gyros = false;
    nvm_data.touch_invert_polarity = false;
    nvm_data.offset_gyro_user_x = 0;
    nvm_data.offset_gyro_user_y = 0;
    nvm_data.offset_gyro_user_z = 0;
    nvm_data.thumbstick_smooth_samples = 0;
    nvm_data.log_level = 3;
    nvm_data.CRC = 0x1234;
}


NVM::NVM()
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    config_profiles = (CtrlProfile *)heap_caps_malloc(NVM_PROFILE_SLOTS * sizeof(CtrlProfile), MALLOC_CAP_SPIRAM);
    // Initialization code for Touch can be added here if needed
    err = nvs_open("storage",  NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        logging::error("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }
    size_t required_size = 0;
    err = nvs_get_blob(nvs_handle, "nvm_data", NULL, &required_size);//blob需要先读长度,避免缓冲区溢出
    err = nvs_get_blob(nvs_handle, "nvm_data", &nvm_data, &required_size);


    logging::info("nvm_data size : %d\n", required_size);
    if ((err != ESP_OK)||(nvm_data.CRC!=0x1234)) {
        data_default();
        config_profile_default_all();
        save_nvm_data();
        logging::warn("NVS_data_init\n");
    }
    else
    {
        char profile_name[12];
        for (int i = 0; i < NVM_PROFILE_SLOTS; i++)
        {
            sprintf(profile_name, "profile_%d", i);
            required_size = sizeof(CtrlProfile);
            err = nvs_get_blob(nvs_handle, profile_name, NULL, &required_size);//blob需要先读长度,避免缓冲区溢出
            err = nvs_get_blob(nvs_handle, profile_name, &config_profiles[i], &required_size);
        }
    }

    logging::info("log_level : %d\n", nvm_data.log_level);
    logging::info("profile_index : %d\n", nvm_data.profile_index);
    logging::info("usb_protocol : %d\n", nvm_data.usb_protocol);
    logging::info("Joystick1 : %d\n", nvm_data.Joystick_offset[0]);
    logging::info("Joystick1 : %d\n", nvm_data.Joystick_offset[1]);
    logging::info("Joystick1 : %d\n", nvm_data.Joystick_offset[2]);
    logging::info("Joystick1 : %d\n", nvm_data.Joystick_offset[3]);
    logging::info("gyro_offset x : %f\n", nvm_data.gyro_offset[0]);
    logging::info("gyro_offset y : %f\n", nvm_data.gyro_offset[1]);
    logging::info("gyro_offset z : %f\n", nvm_data.gyro_offset[2]);
    logging::info("profile_size : %d\n", sizeof(CtrlProfile));
}

NVM::~NVM()
{
    nvs_close(nvs_handle);
}



void NVM::profile_overwrite(uint8_t indexTo, int8_t indexFrom) {
    //debug("Config: Profile overwrite %i -> %i\n", indexFrom, indexTo);
    // Remember name.
    char name[24];
    CtrlProfile* profile = &config_profiles[indexTo];
    CtrlProfileMeta *meta = &profile->sections[SECTION_META].meta;
    memcpy(name, meta->name, 24);
    // From default.
    if (indexFrom < 0) {
        config_profile_default(indexTo, -indexFrom);
    }
    // From other profile slot.
    if (indexFrom > 0) {
        memcpy(
            &profile[indexTo],
            &profile[indexFrom],
            sizeof(CtrlProfile)
        );
    }
    // Restore name.
    memcpy(meta->name, name, 24);
}
