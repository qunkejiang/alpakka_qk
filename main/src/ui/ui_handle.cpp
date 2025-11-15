#include "ui_handle.h"
#include "groups.h"
#include "vars.h"
#include "screens.h"
#include "ui.h"
#include "board.h"

static uint8_t key_old[MATRIX_KEYBOARD_COLS_NUM];
static uint8_t key_edge[MATRIX_KEYBOARD_COLS_NUM];

bool is_key_pressed(uint8_t *data , uint8_t pin) {
    return (data[pin>>4]&(1<<(pin&0xf)))!=0;
}
static void update_button_state(uint8_t pin, lv_obj_t *button)
{
    if (is_key_pressed(key_edge, pin)) {
        lv_obj_set_state(button, LV_STATE_PRESSED,is_key_pressed(key_old , pin));
    }
}

void ui_generic_kb_mouse_handle()
{
    update_button_state(BUTTON_START_1, objects.keymouse_button1);
    update_button_state(BUTTON_START_2, objects.keymouse_button2);
    update_button_state(BUTTON_Y, objects.keymouse_button3);
    update_button_state(BUTTON_X, objects.keymouse_button4);
    update_button_state(BUTTON_B, objects.keymouse_button5);
    update_button_state(BUTTON_A, objects.keymouse_button6);
}

void send_key_if_triggered(uint8_t pin, lv_key_t key) {
    if (is_key_pressed(key_edge, pin)) {
        group_msg_t report;
        report.key = key;
        report.state = is_key_pressed(key_old, pin);
        groups_send_key(report);
    }
}
void ui_menu_group()
{
    axp2101_data_t axp_data = Board::get_axp2101();
    send_key_if_triggered( BUTTON_DPAD_UP, LV_KEY_PREV);
    send_key_if_triggered( BUTTON_DPAD_DOWN, LV_KEY_NEXT);
    send_key_if_triggered( BUTTON_DPAD_RIGHT, LV_KEY_UP);
    send_key_if_triggered( BUTTON_DPAD_LEFT, LV_KEY_DOWN);
    send_key_if_triggered( BUTTON_A, LV_KEY_ENTER);
    if(is_key_pressed(key_edge,BUTTON_B)&&is_key_pressed(key_old,BUTTON_B))
    {
        lv_obj_send_event(lv_scr_act(), LV_EVENT_CLICKED, NULL);
    }
    if(axp_data.event & SHORT_PRESS)
        lv_obj_send_event(lv_scr_act(), LV_EVENT_CLICKED, NULL);
}
void ui_menu1_group()
{
    axp2101_data_t axp_data = Board::get_axp2101();
    send_key_if_triggered( BUTTON_DPAD_UP, LV_KEY_UP);
    send_key_if_triggered( BUTTON_DPAD_DOWN, LV_KEY_DOWN);
    send_key_if_triggered( BUTTON_DPAD_RIGHT, LV_KEY_RIGHT);
    send_key_if_triggered( BUTTON_DPAD_LEFT, LV_KEY_LEFT);
    send_key_if_triggered( BUTTON_A, LV_KEY_ENTER);
    if(is_key_pressed(key_edge,BUTTON_B)&&is_key_pressed(key_old,BUTTON_B))
    {
        lv_obj_send_event(lv_scr_act(), LV_EVENT_CLICKED, NULL);
    }
    if(axp_data.event & SHORT_PRESS)
        lv_obj_send_event(lv_scr_act(), LV_EVENT_CLICKED, NULL);
}

void UI_Handle::report()
{
    axp2101_data_t axp_data = Board::get_axp2101();
    uint8_t *key_value = Board::GetInstance().keyboard_->key_value;
    touch_data_t touch_data = Board::GetInstance().touch_->data;
    int16_t *adc_raw = Board::GetInstance().joystick_->adc_raw;
    
    static uint16_t axp_count=0;
    for(int i=0;i<MATRIX_KEYBOARD_COLS_NUM;i++)
    {
        key_edge[i] = key_value[i] ^ key_old[i];
        key_old[i] = key_value[i];
    }
    int16_t screenId = eez_flow_get_current_screen();
    lvgl_port_lock(0);
    switch (screenId)
    {
        case SCREEN_ID_GENERIC_KB_PAGE://通用键鼠
        case SCREEN_ID_GENERIC_PAGE:
            if(screenId==SCREEN_ID_GENERIC_KB_PAGE)
                ui_generic_kb_mouse_handle();
            if(get_var_usb_state())
            {
                if(axp_data.battery_level>=75)
                    set_var_battery_state(0);
                else if(axp_data.battery_level>50)
                    set_var_battery_state(1);
                else if(axp_data.battery_level>25)
                    set_var_battery_state(2);
                else 
                    set_var_battery_state(4);
            }
            if(axp_data.event & CHARGING_DONE)
                set_var_battery_state(0);
            if(axp_data.event & CHARGING_START)
                set_var_battery_state(5);
            if(axp_data.event & VBUS_INSERT)
                set_var_usb_state(false);
            if(axp_data.event & VBUS_REMOVE)
                set_var_usb_state(true);
            if(axp_count>0)
                axp_count++;
            if(axp_data.event & NEGATIVE_EDGE)
                axp_count=1;
            if(axp_data.event & POSITIVE_EDGE)
            {
                if(axp_count>10&&axp_count<500)
                    lv_obj_send_event(lv_scr_act(), LV_EVENT_CLICKED, NULL);
                axp_count=0;
            }
            break;
        case SCREEN_ID_MENU_PAGE://菜单
        case SCREEN_ID_MENU2_PAGE://菜单2
        case SCREEN_ID_MENU4_PAGE://菜单4
            ui_menu_group();
            break;
        case SCREEN_ID_MENU1_PAGE://菜单1
        case SCREEN_ID_MENU3_PAGE://菜单3
        case SCREEN_ID_MENU5_PAGE://菜单5
            ui_menu1_group();
            break;
        case SCREEN_ID_MENU6_PAGE://菜单6
            set_var_joystick_calibration1(adc_raw[0]);
            set_var_joystick_calibration2(adc_raw[1]);
            set_var_joystick_calibration3(adc_raw[2]);
            set_var_joystick_calibration4(adc_raw[3]);
            ui_menu1_group();
            break;
        case SCREEN_ID_MENU7_PAGE://菜单7
            set_var_touch_calibration_pv(touch_data.touch_value);
            set_var_touch_calibration_led(!touch_data.touch_status);
            ui_menu1_group();
            break;
        default:
            break;
    }
    lvgl_port_unlock();
}
