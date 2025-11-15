#pragma once

#include "logging.h"
#include "board.h"



class Application {
private:
    SemaphoreHandle_t semaphore_hid;   
    SemaphoreHandle_t semaphore_sensor;
    Board* board;
public:
    void hid_task();
    void sensor_task();
    void Start();
};

