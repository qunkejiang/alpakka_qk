#include "nvm.h"
#include "esp_log.h"

#define TAG "nvm"
//crc16运算
uint16_t crc16(uint8_t *data, uint8_t len)
{
    uint16_t crc = 0;
    while (len--) {
        crc ^= *data++;
        for (uint8_t i = 0; i < 8; i++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

uint16_t NVM::get_crc()
{
    return crc16((uint8_t *)&nvm_data, sizeof(nvm_data_t) - sizeof(uint16_t));
}
esp_err_t NVM::save()
{
    //nvs_open("nvs", NVS_READWRITE, &nvs_handle);
    uint16_t crc = get_crc();
    if (crc != nvm_data.CRC) {
        nvm_data.CRC = crc;
        nvs_set_blob(nvs_handle, "nvm_data", &nvm_data, sizeof(nvm_data_t));
        nvs_commit(nvs_handle);
    }
    return ESP_OK;
}
void NVM::data_init()
{
    nvm_data.Joystick_offset[0] = 1955;
    nvm_data.Joystick_offset[1] = 2007;
    nvm_data.Joystick_offset[2] = 1931;
    nvm_data.Joystick_offset[3] = 1943;
    nvm_data.Joystick_gain[0] = 1300;
    nvm_data.Joystick_gain[1] = 1300;
    nvm_data.Joystick_gain[2] = -860;
    nvm_data.Joystick_gain[3] = 860;
    nvm_data.touch_trigger = 25000;
    nvm_data.gyro_offset[0] = 0.0f;
    nvm_data.gyro_offset[1] = 0.0f;
    nvm_data.gyro_offset[2] = 0.0f;
    nvm_data.language = 0;
    nvm_data.backlight = 10;
    nvm_data.mode = 0;
    nvm_data.Configuration = 0;
    nvm_data.bt_state = 0;

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

    // Initialization code for Touch can be added here if needed
    err = nvs_open("storage",  NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }
    size_t required_size = 0;
    err = nvs_get_blob(nvs_handle, "nvm_data", NULL, &required_size);
    err = nvs_get_blob(nvs_handle, "nvm_data", &nvm_data, &required_size);
    ESP_LOGI(TAG, "NVS size : %d", required_size);
    ESP_LOGI(TAG, "Joystick1 : %d", nvm_data.Joystick_offset[0]);
    ESP_LOGI(TAG, "Joystick1 : %d", nvm_data.Joystick_offset[1]);
    ESP_LOGI(TAG, "Joystick1 : %d", nvm_data.Joystick_offset[2]);
    ESP_LOGI(TAG, "Joystick1 : %d", nvm_data.Joystick_offset[3]);

    //nvs_close(my_handle);
    uint16_t crc = get_crc();
    if ((err != ESP_OK)||(nvm_data.CRC!=crc)) {
        ESP_LOGE(TAG, "NVS_data_init");
        data_init();
        save();
    }

    config_profiles = (CtrlProfile *)heap_caps_malloc(NVM_PROFILE_SLOTS * sizeof(CtrlProfile), MALLOC_CAP_SPIRAM);
}

NVM::~NVM()
{
    nvs_close(nvs_handle);
}
