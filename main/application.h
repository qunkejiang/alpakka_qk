#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <freertos/task.h>
#include <esp_log.h>
#include "board.h"

typedef struct sensor_data_t
{
    int16_t adc_value[4];
    uint32_t touch_value = 0;
    int rotary_count = 0;
    sensorData_t Data;
    uint8_t axp_data;
};


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
