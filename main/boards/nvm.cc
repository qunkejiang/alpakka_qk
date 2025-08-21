#include "nvm.h"
#include "esp_log.h"

#define TAG "nvm"

esp_err_t NVM::save()
{
    //nvs_open("nvs", NVS_READWRITE, &nvs_handle);
    nvs_set_blob(nvs_handle, "nvm_data", &nvm_data, sizeof(nvm_data_t));
    nvs_commit(nvs_handle);
    return ESP_OK;

}
void NVM::data_init()
{
    nvm_data.Joystick_offset[0] = 1955;
    nvm_data.Joystick_offset[1] = 2007;
    nvm_data.Joystick_offset[2] = 1931;
    nvm_data.Joystick_offset[3] = 1943;
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
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "NVS ");
        data_init();
        save();
    }
}
NVM::~NVM()
{
    nvs_close(nvs_handle);
}
