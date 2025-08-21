#include "application.h"
#include "board.h"

#define TAG "Application"
uint8_t mode=0;



void menu_handle(sensor_data_t *sensor_data, Board* board)
{

}
void generic_handle(sensor_data_t *sensor_data, Board* board)
{
    GamepadReport gamepad_report=
    {
        .lx=(int16_t)(sensor_data->adc_value[0]-board->nvm_->nvm_data.Joystick_offset[0]),
        .ly=(int16_t)(sensor_data->adc_value[1]-board->nvm_->nvm_data.Joystick_offset[1]),
        .rx=(int16_t)(sensor_data->adc_value[2]-board->nvm_->nvm_data.Joystick_offset[2]),
        .ry=(int16_t)(sensor_data->adc_value[3]-board->nvm_->nvm_data.Joystick_offset[3]),
        .lz=0,
        .rz=0,
        .buttons=0,
    };
    gamepad_report.buttons=
        (((((board->keyboard_->key_value[0]<<5)|
        board->keyboard_->key_value[1])<<5)|
        board->keyboard_->key_value[2])<<5)|
        board->keyboard_->key_value[3];
    board->usb_->gamepad_report(gamepad_report);

}
void xbox_handle(sensor_data_t *sensor_data, Board* board)
{   
        XInputReport xinput_report=
        {
            // .report_id   = 0,
            // .report_size = 20,
            .buttons_0=0,
            .buttons_1=0,
            .lz=0,
            .rz=0,
            .lx=(int16_t)(sensor_data->adc_value[0]-board->nvm_->nvm_data.Joystick_offset[0]),
            .ly=(int16_t)(sensor_data->adc_value[1]-board->nvm_->nvm_data.Joystick_offset[1]),
            .rx=(int16_t)(sensor_data->adc_value[2]-board->nvm_->nvm_data.Joystick_offset[2]),
            .ry=(int16_t)(sensor_data->adc_value[3]-board->nvm_->nvm_data.Joystick_offset[3]),
            .reserved    = {0, 0, 0, 0, 0, 0}
        };
        board->usb_->xinput_report(xinput_report);
}
void generic_kb_mouse_handle(sensor_data_t *sensor_data, Board* board)
{
    MouseReport mouse_report = {
        .buttons = 0,
        .x = (int16_t)(sensor_data->adc_value[0] - board->nvm_->nvm_data.Joystick_offset[0]),
        .y = (int16_t)(sensor_data->adc_value[1] - board->nvm_->nvm_data.Joystick_offset[1]),
    };
    board->usb_->mouse_report(mouse_report);
}
void user_kb_mouse_handle(sensor_data_t *sensor_data, Board* board)
{
    MouseReport mouse_report = {
        .buttons = 0,
        .x = (int16_t)(sensor_data->adc_value[0] - board->nvm_->nvm_data.Joystick_offset[0]),
        .y = (int16_t)(sensor_data->adc_value[1] - board->nvm_->nvm_data.Joystick_offset[1]),
    };
    board->usb_->mouse_report(mouse_report);
}
void periodic_hanld(sensor_data_t *sensor_data, Board* board) {
    switch (mode)
    {
        case 0://菜单
            menu_handle(sensor_data, board);
            break;
        case 1://通用手柄
            generic_handle(sensor_data  , board);
            break;
        case 2://xbox手柄
            xbox_handle(sensor_data, board);
            break;
        case 3://通用键鼠
            generic_kb_mouse_handle(sensor_data, board);
            break;
        case 4://用户键鼠
            user_kb_mouse_handle(sensor_data, board);
            break;
        default:
            break;
    }
}
void Application::periodic_sensor_read_task(void *pvParameters) {
    Board* board = static_cast<Board*>(pvParameters);
    sensor_data_t sensor_data;
    while (1) {
        board->axp2101_->Update(&sensor_data.axp_data);
        board->joystick_->ReadADCValues(sensor_data.adc_value);
        board->touch_->GetTouchValue(&sensor_data.touch_value);
        board->rotary_->GetPulseCounterValue(&sensor_data.rotary_count);
        board->imu_->Icm_42688_update(&sensor_data.Data);

        periodic_hanld(&sensor_data, board);
        vTaskDelay(1); 
    int i =  0;
//         if(i>=999)
//         {
//            //i=0;
//             // ESP_LOGI(TAG, "Joystick : %d", gamepad_report.x);
//             // ESP_LOGI(TAG, "Joystick : %d", gamepad_report.y);
//             // ESP_LOGI(TAG, "Joystick : %d", gamepad_report.rx);
//             // ESP_LOGI(TAG, "Joystick : %d", gamepad_report.ry);
//             // ESP_LOGI(TAG, "Keyboard : %d", board->keyboard_->key_value[0]);
//             // ESP_LOGI(TAG, "Keyboard : %d", board->keyboard_->key_value[1]);
//             // ESP_LOGI(TAG, "Keyboard : %d", board->keyboard_->key_value[2]);
//             // ESP_LOGI(TAG, "Keyboard : %d", board->keyboard_->key_value[3]);
//             // ESP_LOGI(TAG, "Touch : %ld", touch_data);
//             // ESP_LOGI(TAG, "Rotary : %d", rotary_count);
// //     printf("%f,%f,%f,%f,%f,%f,%f,%d\n",
// //             Data.acc.axis[0], Data.acc.axis[1], Data.acc.axis[2],
// //             Data.gyro.axis[0], Data.gyro.axis[1], Data.gyro.axis[2],
// //             Data.temp, fifo_count);
//         }else
//         {
//             i++;
//         }
    }
}


void Application::Start() {
    auto& board = Board::GetInstance();
    xTaskCreatePinnedToCore(periodic_sensor_read_task, "sensor_read_task", 4096, &board, 4, NULL, 0);
}
