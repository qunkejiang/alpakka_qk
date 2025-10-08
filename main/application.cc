#include "application.h"
#include "board.h"
#include "ui_handle.h"

#define TAG "Application"


void Application::periodic_sensor_read_task(void *pvParameters) {
    Board* board = static_cast<Board*>(pvParameters);
    // sensor_data_t sensor_data;
    // sensor_data.key_matrix.key_edge = (uint8_t *)calloc(board->keyboard_->mkbd->nr_col_gpios, sizeof(uint8_t));
    // sensor_data.key_matrix.key_value = (uint8_t *)calloc(board->keyboard_->mkbd->nr_col_gpios, sizeof(uint8_t));
    
    while (1) {
        board->touch_->update(board->nvm_->nvm_data.touch_trigger);
        board->imu_->update(board->nvm_->nvm_data.gyro_offset);
        // board->axp2101_->Update(&sensor_data.axp_data);
        // board->joystick_->GetADCValues(sensor_data.adc_value,board->nvm_->nvm_data.Joystick_offset,board->nvm_->nvm_data.Joystick_gain);
        // board->rotary_->GetPulseCounterValue(&sensor_data.rotary_count);
        // board->keyboard_->get_matrix_kbd(&sensor_data.key_matrix);

        board->profile_->report();
        //hid_periodic_hanld(&sensor_data, board);
        //ui_periodic_handle(&sensor_data, board);
        vTaskDelay(1); 
    }
}

void Application::Start()
{
    auto& board = Board::GetInstance();
    xTaskCreatePinnedToCore(periodic_sensor_read_task, "sensor_read_task", 4096, &board, 4, NULL, 0);
}

