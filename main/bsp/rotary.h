#ifndef ROTARY_C_H
#define ROTARY_C_H

#include "driver/pulse_cnt.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
class Rotary {
private:
    pcnt_unit_handle_t pcnt_unit;
    void InitializePulseCounter(int edge_a, int edge_b);
public:
    esp_err_t GetPulseCounterValue(int* count);
    Rotary(int edge_a, int edge_b);
};

#endif
