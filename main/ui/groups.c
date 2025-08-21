#include "ui.h"
#include "screens.h" // pick group declarations from here
#include "groups.h" // pick group declarations from here

#include <esp_log.h>
#include "esp_lvgl_port.h"
// you should initialize your input device
// before calling "ui_create_groups()"
lv_indev_t *indev_keypad;
// ...lv_indev_drv_t *indev_drv, lv_indev_data_t *data

void lvgl_port_read_kb(lv_indev_t * indev, lv_indev_data_t * data)
{
    //ESP_LOGI("groups","lvgl_port_read_kb");

}
// void lvgl_port_read_kb()
// {
//     // assert(indev_drv);
//     // lvgl_port_usb_hid_ctx_t *ctx = (lvgl_port_usb_hid_ctx_t *)indev_drv->user_data;
//     // assert(ctx);

//     // data->key = ctx->kb.last_key;
//     // if (ctx->kb.pressed) {
//     //     data->state = LV_INDEV_STATE_PRESSED;
//     //     ctx->kb.pressed = false;
//     // } else {
//     //     data->state = LV_INDEV_STATE_RELEASED;
//     //     ctx->kb.last_key = 0;
//     // }
//     //ESP_LOGI("lvgl_port_read_kb", "key: %d, state: %d", data->key, data->state);

// }

void groups_init()
{
    
    ESP_LOGI("groups","groups_init");
    // call this before "ui_init()"
    indev_keypad = lv_indev_create();
    lv_indev_set_type(indev_keypad, LV_INDEV_TYPE_KEYPAD);
    lv_indev_set_mode(indev_keypad, LV_INDEV_MODE_TIMER);
    lv_indev_set_read_cb(indev_keypad, lvgl_port_read_kb);
    lv_indev_set_driver_data(indev_keypad, NULL);

    ui_create_groups();
    // set group for your input device
    lv_indev_set_group(indev_keypad, groups.keyboard);

}
