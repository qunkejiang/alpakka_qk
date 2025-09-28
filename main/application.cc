#include "application.h"
#include "board.h"
#include "groups.h"
#include "vars.h"
#include "screens.h"
#include "ui.h"

#define TAG "Application"

static void update_button_state(uint8_t key_edge, uint8_t key_value, uint8_t mask, 
                               lv_obj_t *button) {
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
        case SCREEN_ID_GENERIC_PAGE:
            break;
        case SCREEN_ID_GENERIC_KB_PAGE://通用键鼠
            ui_generic_kb_mouse_handle(sensor_data, board);
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
    board->display_->Unlock();
}
// void hid_generic_handle(sensor_data_t *sensor_data, Board* board) {
//     // 通用手柄处理//11652 3912
//     // GamepadReport gamepad_report=
//     // {
//     //     .lx=(int16_t)(sensor_data->adc_value[0]*32767),
//     //     .ly=(int16_t)(sensor_data->adc_value[1]*32767),
//     //     .rx=(int16_t)(sensor_data->adc_value[2]*32767),
//     //     .ry=(int16_t)(sensor_data->adc_value[3]*32767),
//     //     .lz=0,
//     //     .rz=0,
//     //     .buttons=0,
//     // };
//     // gamepad_report.buttons=
//     //     (((((board->keyboard_->key_value[0]<<5)|
//     //     board->keyboard_->key_value[1])<<5)|
//     //     board->keyboard_->key_value[2])<<5)|
//     //     board->keyboard_->key_value[3];
//     // board->usb_->gamepad_report(gamepad_report);
//     if(sensor_data->axp_data & 0x08)
//     {
//         eez_flow_set_screen(SCREEN_ID_MENU_PAGE, LV_SCR_LOAD_ANIM_FADE_IN, 500, 0);
//     }
// }
// void xbox_handle(sensor_data_t *sensor_data, Board* board)
// {   
//         XInputReport xinput_report=
//         {
//             .report_id   = 0,
//             .report_size = 20,
//             .buttons_0=0,
//             .buttons_1=0,
//             .lz=0,
//             .rz=0,
//             .lx=(int16_t)(sensor_data->adc_value[0]*32767),
//             .ly=(int16_t)(sensor_data->adc_value[1]*32767),
//             .rx=(int16_t)(sensor_data->adc_value[2]*32767),
//             .ry=(int16_t)(sensor_data->adc_value[3]*32767),
//             .reserved    = {0, 0, 0, 0, 0, 0}
//         };
//         board->usb_->xinput_report(xinput_report);
// }
// void user_kb_mouse_handle(sensor_data_t *sensor_data, Board* board)
// {
//     MouseReport mouse_report = {
//         .buttons = 0,
//         .x = (int16_t)(sensor_data->adc_value[0] - board->nvm_->nvm_data.Joystick_offset[0]),
//         .y = (int16_t)(sensor_data->adc_value[1] - board->nvm_->nvm_data.Joystick_offset[1]),
//         .scroll = 0,
//         .pan = 0,
//     };
//     board->usb_->mouse_report(mouse_report);
// }
// void hid_generic_kb_mouse_handle(sensor_data_t *sensor_data, Board* board) {
//     // 通用键鼠处理
// }
// void hid_periodic_hanld(sensor_data_t *sensor_data, Board* board) {
//     int16_t screenId = eez_flow_get_current_screen();
//     switch (screenId)
//     {
//         case SCREEN_ID_GENERIC_PAGE:
//             hid_generic_handle(sensor_data  , board);//通用手柄
//             break;
//         case SCREEN_ID_GENERIC_KB_PAGE://通用键鼠
//             hid_generic_kb_mouse_handle(sensor_data, board);
//             break;
//         default:
//             break;
//     }
// }

void Application::periodic_sensor_read_task(void *pvParameters) {
    Board* board = static_cast<Board*>(pvParameters);
    sensor_data_t sensor_data;
    sensor_data.key_matrix.key_edge = (uint8_t *)calloc(board->keyboard_->mkbd->nr_col_gpios, sizeof(uint8_t));
    sensor_data.key_matrix.key_value = (uint8_t *)calloc(board->keyboard_->mkbd->nr_col_gpios, sizeof(uint8_t));
    
    while (1) {
        board->axp2101_->Update(&sensor_data.axp_data);
        board->joystick_->GetADCValues(sensor_data.adc_value,board->nvm_->nvm_data.Joystick_offset,board->nvm_->nvm_data.Joystick_gain);
        board->touch_->GetTouchValue(&sensor_data.touch_data,board->nvm_->nvm_data.touch_trigger);
        board->rotary_->GetPulseCounterValue(&sensor_data.rotary_count);
        board->imu_->Icm_42688_update(&sensor_data.Data,board->nvm_->nvm_data.gyro_offset);
        board->keyboard_->get_matrix_kbd(&sensor_data.key_matrix);

        //hid_periodic_hanld(&sensor_data, board);
        ui_periodic_handle(&sensor_data, board);
        vTaskDelay(1); 
    }
}

void Application::Start() {
    auto& board = Board::GetInstance();
    xTaskCreatePinnedToCore(periodic_sensor_read_task, "sensor_read_task", 4096, &board, 4, NULL, 0);
}


// void joystick_log(sensor_data_t *sensor_data)
// {
//     static int i =  0;        
//     if(i>=999)
//     {
//         i=0;
//         // ESP_LOGI(TAG, "Joystick : %f", sensor_data->adc_value[0]);
//         // ESP_LOGI(TAG, "Joystick : %f", sensor_data->adc_value[1]);
//         // ESP_LOGI(TAG, "Joystick : %f", sensor_data->adc_value[2]);
//         // ESP_LOGI(TAG, "Joystick : %f", sensor_data->adc_value[3]);
// //             // ESP_LOGI(TAG, "Keyboard : %d", board->keyboard_->key_value[0]);
// //             // ESP_LOGI(TAG, "Keyboard : %d", board->keyboard_->key_value[1]);
// //             // ESP_LOGI(TAG, "Keyboard : %d", board->keyboard_->key_value[2]);
// //             // ESP_LOGI(TAG, "Keyboard : %d", board->keyboard_->key_value[3]);
// //             // ESP_LOGI(TAG, "Touch : %ld", touch_data);
// //             // ESP_LOGI(TAG, "Rotary : %d", rotary_count);
// // //     printf("%f,%f,%f,%f,%f,%f,%f,%d\n",
// // //             Data.acc.axis[0], Data.acc.axis[1], Data.acc.axis[2],
// // //             Data.gyro.axis[0], Data.gyro.axis[1], Data.gyro.axis[2],
// // //             Data.temp, fifo_count);
//     }else
//     {
//         i++;
//     }
// }
//         joystick_log(&sensor_data);
