#ifndef ROTARY_C_H
#define ROTARY_C_H

#include "ctrl.h"
#include "button.h"

typedef enum RotaryDir_enum {
    ROTARY_UP,
    ROTARY_DOWN,
} RotaryDir;

typedef struct Rotary_struct Rotary_t;
struct Rotary_struct {
    int8_t count;
    int8_t mode;
    uint8_t actions[5][2][4];
};


class Rotary {
private:
    Rotary_t self={0,0,{0}};
public:
    void config_mode(uint8_t mode,Actions actions_up,Actions actions_down);
    void set_mode(uint8_t value);
    void report();
};

#endif
