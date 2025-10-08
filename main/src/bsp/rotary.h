#ifndef ROTARY_C_H
#define ROTARY_C_H

#include "config.h"
#include "driver/pulse_cnt.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

typedef enum RotaryDir_enum {
    ROTARY_UP,
    ROTARY_DOWN,
} RotaryDir;

typedef struct Rotary_struct Rotary_t;
struct Rotary_struct {
    int8_t count_old;
    int8_t mode;
    uint8_t actions[5][2][4];
};


class Rotary {
private:
    Rotary_t self;
    pcnt_unit_handle_t pcnt_unit;
    void InitializePulseCounter(int edge_a, int edge_b);
public:
    esp_err_t GetPulseCounterValue(int* count);
    void config_mode(uint8_t mode,Actions actions_up,Actions actions_down);
    void set_mode(uint8_t value);
    void report();
    Rotary(int edge_a, int edge_b);
};

#endif
