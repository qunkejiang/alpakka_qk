// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2022, Input Labs Oy.
#pragma once    

#include <cstdint>
#include "board_config.h"
#include "ctrl.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define ACTIONS_LEN 4
typedef uint8_t Actions[ACTIONS_LEN];

#define BUTTON_SELECT_1 0x02
#define BUTTON_SELECT_2 0x22
#define BUTTON_START_1 0x32
#define BUTTON_START_2 0x12
#define BUTTON_DPAD_LEFT 0x20
#define BUTTON_DPAD_RIGHT 0x10
#define BUTTON_DPAD_UP 0x30
#define BUTTON_DPAD_DOWN 0x00
#define BUTTON_L1 0x03
#define BUTTON_L2 0x33
#define BUTTON_L3 0x13
#define BUTTON_L4 0x23
#define BUTTON_A 0x01
#define BUTTON_B 0x21
#define BUTTON_X 0x11
#define BUTTON_Y 0x31
#define BUTTON_R1 0x34
#define BUTTON_R2 0x24
#define BUTTON_R3 0x14
#define BUTTON_R4 0x04

#define BUTTON_HOME 252
#define BUTTON_TOUCH_IN 253    
#define BUTTON_VIRTUAL 254 
#define BUTTON_NONE 255 // Buttons without any hardware associated to them.


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
    bool virtual_press;
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

