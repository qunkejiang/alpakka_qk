#pragma once

#include "common.h"
#include "driver/pulse_cnt.h"


#define PCNT_HIGH_LIMIT 100
#define PCNT_LOW_LIMIT  -100

class Pcnt {
private:
    pcnt_unit_handle_t pcnt_unit;
    void InitializePulseCounter(int edge_a, int edge_b);
public:
    int8_t diff=0;
    int8_t count=0;
    void update();
    Pcnt(int edge_a, int edge_b);
};

