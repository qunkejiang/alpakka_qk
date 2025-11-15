#include "ui.h"
#include "screens.h" // pick group declarations from here
#include "groups.h" // pick group declarations from here

#include "esp_lvgl_port.h"
// you should initialize your input device
// before calling "ui_create_groups()"

lv_indev_t *indev_keypad;
QueueHandle_t groups_queue;

void groups_send_key(group_msg_t report)
{
    bool result = xQueueSend(groups_queue, &report, 0);
    if(result != pdPASS)
    {
        //logging::warn(TAG, "Key send failed: %d %d\n", report.key, report.state);
    }
}

void lvgl_port_read_kb(lv_indev_t * indev, lv_indev_data_t * data)
{
    group_msg_t report;
    if(xQueueReceive(groups_queue, &report, 0) == pdPASS)
    {
        data->key = report.key;
        data->state = report.state;
        //logging::info(TAG, "Key : %d %d\n", report.key, report.state);
    }
}

void groups_init()
{
    
    //logging::info(TAG, "groups_init\n");
    groups_queue = xQueueCreate(8, sizeof(group_msg_t));
    // call this before "ui_init()"
    indev_keypad = lv_indev_create();
    lv_indev_set_type(indev_keypad, LV_INDEV_TYPE_KEYPAD);
    lv_indev_set_mode(indev_keypad, LV_INDEV_MODE_TIMER);
    lv_indev_set_read_cb(indev_keypad, lvgl_port_read_kb);
    lv_indev_set_driver_data(indev_keypad, NULL);

    ui_create_groups();
    // set group for your input device
    lv_indev_set_group(indev_keypad, groups.keyboard_groups);

}
