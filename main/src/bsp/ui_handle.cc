#include "ui_handle.h"
#include "groups.h"
#include "vars.h"
#include "screens.h"
#include "ui.h"


static void update_button_state(uint8_t key_edge, uint8_t key_value, uint8_t mask, lv_obj_t *button)
{
    if (key_edge & mask) {
        lv_obj_set_state(button, LV_STATE_PRESSED, (key_value & mask) != 0);
    }
}

void ui_generic_kb_mouse_handle(sensor_data_t *sensor_data, Board* board)
{
    uint8_t *key_edge = sensor_data->key_matrix.key_edge;
    uint8_t *key_value = sensor_data->key_matrix.key_value;
    update_button_state(key_edge[3], key_value[3], 0x04, objects.keymouse_button1);
    update_button_state(key_edge[1], key_value[1], 0x04, objects.keymouse_button2);
    update_button_state(key_edge[3], key_value[3], 0x02, objects.keymouse_button3);
    update_button_state(key_edge[1], key_value[1], 0x02, objects.keymouse_button4);
    update_button_state(key_edge[2], key_value[2], 0x02, objects.keymouse_button5);
    update_button_state(key_edge[0], key_value[0], 0x02, objects.keymouse_button6);
    joystick_dir_t dir=board->joystick_->GetJoystickDir(sensor_data->adc_value[0],sensor_data->adc_value[1]);
    if(dir<JOYSTICK_DIR_NONE)
    {
        set_var_keymouse_zu((int32_t)dir);   
    }
    if((key_edge[0] & 0x08)||((key_edge[3] & 0x04)&&(key_value[3] & 0x04)&&(get_var_keymouse_zu()==0)))
    {
        if(get_var_keymouse_aa())set_var_keymouse_aa(false);
        else set_var_keymouse_aa(true);
    }
}
static void send_key_if_triggered(sensor_data_t *sensor_data, uint8_t row, uint8_t mask, lv_key_t key) {
    if ((sensor_data->key_matrix.key_edge[row] & mask)) {
        group_msg_t report;
        report.key = key;
        report.state = ((sensor_data->key_matrix.key_value[row] & mask) != 0x0);
        groups_send_key(report);
    }
}
void ui_menu_group(sensor_data_t *sensor_data)
{
    send_key_if_triggered(sensor_data, 3, 0x01, LV_KEY_PREV);
    send_key_if_triggered(sensor_data, 0, 0x01, LV_KEY_NEXT);
    send_key_if_triggered(sensor_data, 1, 0x01, LV_KEY_UP);
    send_key_if_triggered(sensor_data, 2, 0x01, LV_KEY_DOWN);
    send_key_if_triggered(sensor_data, 0, 0x02, LV_KEY_ENTER);
    if(sensor_data->key_matrix.key_edge[2] & 0x02)
    {
        if((sensor_data->key_matrix.key_value[2] & 0x02)!=0x0)
            lv_obj_send_event(lv_scr_act(), LV_EVENT_CLICKED, NULL);
    }
}
void ui_menu1_group(sensor_data_t *sensor_data)
{
    send_key_if_triggered(sensor_data, 3, 0x01, LV_KEY_UP);
    send_key_if_triggered(sensor_data, 0, 0x01, LV_KEY_DOWN);
    send_key_if_triggered(sensor_data, 1, 0x01, LV_KEY_RIGHT);
    send_key_if_triggered(sensor_data, 2, 0x01, LV_KEY_LEFT);
    send_key_if_triggered(sensor_data, 0, 0x02, LV_KEY_ENTER);
    if(sensor_data->key_matrix.key_edge[2] & 0x02)
    {
        if((sensor_data->key_matrix.key_value[2] & 0x02)!=0x0)
            lv_obj_send_event(lv_scr_act(), LV_EVENT_CLICKED, NULL);
    }
}


void ui_periodic_handle(sensor_data_t *sensor_data, Board* board) {
    int16_t screenId = eez_flow_get_current_screen();
    board->display_->Lock(0);
    switch (screenId)
    {
        case SCREEN_ID_GENERIC_KB_PAGE://通用键鼠
            ui_generic_kb_mouse_handle(sensor_data, board);
        case SCREEN_ID_GENERIC_PAGE:
            if(sensor_data->axp_data.event & VBUS_INSERT)
                set_var_usb_state(false);
            if(sensor_data->axp_data.event & VBUS_REMOVE)
                set_var_usb_state(true);
            if(get_var_usb_state())
            {
                if(sensor_data->axp_data.battery_level>=75)
                    set_var_battery_state(0);
                else if(sensor_data->axp_data.battery_level>50)
                    set_var_battery_state(1);
                else if(sensor_data->axp_data.battery_level>25)
                    set_var_battery_state(2);
                else 
                    set_var_battery_state(4);
            }
            if(sensor_data->axp_data.event & CHARGING_DONE)
                set_var_battery_state(0);
            if(sensor_data->axp_data.event & CHARGING_START)
                set_var_battery_state(5);
            break;
        case SCREEN_ID_MENU_PAGE://菜单
        case SCREEN_ID_MENU2_PAGE://菜单2
        case SCREEN_ID_MENU4_PAGE://菜单4
            ui_menu_group(sensor_data);
            break;
        case SCREEN_ID_MENU1_PAGE://菜单1
        case SCREEN_ID_MENU3_PAGE://菜单3
            ui_menu1_group(sensor_data);
            break;
        case SCREEN_ID_MENU5_PAGE://菜单5
            board->imu_->calibration(board->nvm_->nvm_data.gyro_offset,&imu_calibration);
            ui_menu1_group(sensor_data);
            break;
        case SCREEN_ID_MENU6_PAGE://菜单6
            board->joystick_->calibration_setp2();
            set_var_joystick_calibration1(board->joystick_->adc_raw[0]);
            set_var_joystick_calibration2(board->joystick_->adc_raw[1]);
            set_var_joystick_calibration3(board->joystick_->adc_raw[2]);
            set_var_joystick_calibration4(board->joystick_->adc_raw[3]);
            ui_menu1_group(sensor_data);
            break;
        case SCREEN_ID_MENU7_PAGE://菜单7
            set_var_touch_calibration_pv(sensor_data->touch_data.touch_value);
            set_var_touch_calibration_led(!sensor_data->touch_data.touch_status);
            ui_menu1_group(sensor_data);
            break;
        default:
            break;
    }
    if(sensor_data->axp_data.event & SHORT_PRESS)
        lv_obj_send_event(lv_scr_act(), LV_EVENT_CLICKED, NULL);
    board->display_->Unlock();
}