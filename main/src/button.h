// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2022, Input Labs Oy.
#pragma once    

#include <string>
#include "config.h"
#include "ctrl.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

    #define PIN_THUMBSTICK_RY 26
    #define PIN_THUMBSTICK_RX 27
    #define PIN_THUMBSTICK_LX 29
    #define PIN_THUMBSTICK_LY 28
    #define PIN_SELECT_1 114
    #define PIN_SELECT_2 110
    #define PIN_DPAD_LEFT 104
    #define PIN_DPAD_RIGHT 101
    #define PIN_DPAD_UP 103
    #define PIN_DPAD_DOWN 100
    #define PIN_L1 102
    #define PIN_L2 115
    #define PIN_L3 109
    #define PIN_L4 108
    #define PIN_PCBGEN_0 111
    #define PIN_PCBGEN_1 113
    #define PIN_START_1 200
    #define PIN_START_2 201
    #define PIN_A 215
    #define PIN_B 210
    #define PIN_X 213
    #define PIN_Y 211
    #define PIN_DHAT_LEFT 203
    #define PIN_DHAT_RIGHT 205
    #define PIN_DHAT_UP 206
    #define PIN_DHAT_DOWN 204
    #define PIN_R1 212
    #define PIN_R2 214
    #define PIN_R3 202
    #define PIN_R4 207

    #define PIN_VIRTUAL 254 
    #define PIN_TOUCH_IN 12     // 13 (rc1)
    #define PIN_NONE 255 // Buttons without any hardware associated to them.


typedef enum _ButtonMode {
    NORMAL = 0x1,
    HOLD = 0x2,
    DOUBLE = 0x4,
    IMMEDIATE = 0x8,
    LONG = 0x10,
    STICKY = 0x20,
} ButtonMode;

typedef struct Button_struct Button_t;
struct Button_struct {
    uint8_t pin;
    ButtonMode mode;
    Actions actions;
    Actions actions_secondary;
    Actions actions_terciary;
    bool state_primary;
    bool state_secondary;
    bool state_terciary;
    bool emitted_primary;
//     bool virtual_press;
    bool timestamps_updated;
    uint64_t press_timestamp;
    uint64_t press_timestamp_prev;
};

class Button {
private:
    void handle_normal(Button_t *self);
    void handle_hold(Button_t *self);
    void handle_double(Button_t *self);
    void handle_hold_double(Button_t *self);
    void handle_sticky(Button_t *self);
    
public:
    bool is_pressed(Button_t *self);
    void reset(Button_t *self);
    void report(Button_t *self);
    Button_t Button_from_params (uint8_t pin,ButtonMode mode,Actions actions,Actions actions_secondary,Actions actions_terciary);
    Button_t Button_from_ctrl(uint8_t pin, CtrlSection section);
};

