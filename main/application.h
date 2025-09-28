#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <freertos/task.h>
#include <esp_log.h>
#include "board.h"

typedef struct 
{
    key_matrix_t key_matrix;
    float adc_value[4];
    touch_data_t touch_data;
    int rotary_count = 0;
    Imu_Data_t Data;
    axp2101_data_t axp_data;
}sensor_data_t;


class Application {
public:
    static Application& GetInstance() {
        static Application instance;
        return instance;
    }
    void Start();
    // QueueHandle_t GetHidQueue()  { return hid_queue; }

private:
    static void periodic_sensor_read_task(void *pvParameters);
    //static void periodic_hid_task(void *pvParameters);
    // void periodic_hanld(sensor_data_t *sensor_data);
    Application(){};
    ~Application(){};
};
// QueueHandle_t Application::hid_queue = nullptr;

#endif // _APPLICATION_H_
