#include "application.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include "ui.h"
#include "screens.h"



void Application::sensor_task() {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = 1;

    while (1) {
        xSemaphoreTake(semaphore_hid, portMAX_DELAY);
        board->touch_->update(board->nvm_->nvm_data.touch_trigger);
        board->imu_->update(board->nvm_->nvm_data.gyro_offset);
        board->joystick_->update(board->nvm_->nvm_data.Joystick_offset);
        board->axp2101_->update();
        board->pcnt_->update();
        xSemaphoreGive(semaphore_hid);

        xSemaphoreGive(semaphore_sensor);
        
        UI_Handle::report();
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}
int i=0;
void Application::hid_task() {
    while (1) {
        xSemaphoreTake(semaphore_sensor, portMAX_DELAY);
        int16_t screenId = eez_flow_get_current_screen();
        if( (screenId==SCREEN_ID_GENERIC_KB_PAGE) || (screenId==SCREEN_ID_GENERIC_PAGE) )
        {
            xSemaphoreTake(semaphore_hid, portMAX_DELAY);
            board->profile_->report();
            xSemaphoreGive(semaphore_hid);
        }
        board->hid_->report_wired();
    }
}

void Application::Start()
{
    board = &Board::GetInstance();
    board->Init();
    semaphore_sensor = xSemaphoreCreateBinary();
    semaphore_hid = xSemaphoreCreateBinary();
    xSemaphoreGive(semaphore_hid);
    
    xTaskCreatePinnedToCore([](void* arg) {
        ((Application*)arg)->sensor_task();
        vTaskDelete(NULL);
    }, "sensor_task", 4096, this, 6, NULL, 0);
    
    xTaskCreatePinnedToCore([](void* arg) {
        ((Application*)arg)->hid_task();
        vTaskDelete(NULL);
    }, "hid_task", 4096, this, 5, NULL, 1);
}

