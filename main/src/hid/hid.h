// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2022, Input Labs Oy.

#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "ctrl.h"



#define BIT_15 32767 
#define BIT_8 127

typedef enum _GamepadAxis {
    LX,
    LY,
    LZ,
    RX,
    RY,
    RZ
} GamepadAxis;


typedef struct __packed _KeyboardReport {
    uint8_t modifier;
    uint8_t reserved;
    uint8_t keycode[6];
} KeyboardReport;

typedef struct __packed _MouseReport {
    uint8_t buttons;
    int16_t x;
    int16_t y;
    int8_t scroll;
    int8_t pan;
} MouseReport;

typedef struct __packed _GamepadReport {
    int16_t lx;
    int16_t ly;
    int16_t rx;
    int16_t ry;
    int16_t lz;
    int16_t rz;
    uint32_t buttons;
} GamepadReport;




class hid
{
private:
    static void procedure_press(uint8_t procedure);
    static void procedure_release(uint8_t procedure);
    static float axis(float value,uint8_t matrix_index_pos, uint8_t matrix_index_neg);
    static void report_keyboard();    
    static void report_mouse();
    static void report_gamepad();
    static void report_xinput();
    
    static void macro(uint8_t index); 
    static void thanks(void *arg);
public:
    //hid() {};
    static uint8_t state_matrix[PROC_MAX];
    static float mouse_x;
    static float mouse_y;
    static float gamepad_axis_data[6];
    static float gamepad_axis_data_last[6];

    
// // Mouse axis.
    static void mouse_move(float x, float y);
// void hid_mouse_wheel(int8_t z);
// // Gamepad.
    static bool is_axis(uint8_t key);
    static void gamepad_axis(GamepadAxis axis, float value);

// bool hid_report_wireless();
    static void press(uint8_t key);
    static void release(uint8_t key);
    static void press_multiple(uint8_t *keys);
    static void release_multiple(uint8_t *keys);
    static void release_multiple_later(uint8_t *keys, uint16_t delay);
    static void release_multiple_later_callback(TimerHandle_t xTimer);
    void report_wired();
};

