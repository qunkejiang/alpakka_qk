// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2022, Input Labs Oy.
#pragma once

#include "button.h"
#include "ctrl.h"
#include "rotary.h"
#include "thumbstick.h"
#include "gyro.h"
// #include "webusb.h"
// #include "common.h"


// typedef enum ProfileIndex_enum {
//     PROFILE_HOME=0,
//     PROFILE_FPS_FUSION,
//     PROFILE_RACING,
//     PROFILE_CONSOLE,
//     PROFILE_DESKTOP,
//     PROFILE_FPS_WASD,
//     PROFILE_FLIGHT,
//     PROFILE_CONSOLE_LEGACY,
//     PROFILE_RTS,
//     PROFILE_CUSTOM_1,
//     PROFILE_CUSTOM_2,
//     PROFILE_CUSTOM_3,
//     PROFILE_CUSTOM_4,
//     PROFILE_HOME_GAMEPAD,
//     PROFILE_SLOTS
// } ProfileIndex;

typedef struct Profile_struct Profile_t;
struct Profile_struct {
    Button_t select_1;
    Button_t select_2;
    Button_t start_1;
    Button_t start_2;
    Button_t dpad_down;
    Button_t dpad_right;
    Button_t dpad_left;
    Button_t dpad_up;
    Button_t a;
    Button_t b;
    Button_t x;
    Button_t y;
    Button_t hat_down;
    Button_t hat_right;
    Button_t hat_left;
    Button_t hat_up;
    Button_t hat_in;
    Button_t l1;
    Button_t l2;
    Button_t l3;
    Button_t l4;
    Button_t r1;
    Button_t r2;
    Button_t r4;
    Thumbstick left_thumbstick;
    Thumbstick right_thumbstick;
    Rotary *rotary;
    Gyro gyro;
};

class  profile: public Button
{
private:
    Profile_t profiles;//[PROFILE_SLOTS]只存当前使用的，全存的话浪费空间 
    void load_from_config(CtrlProfile *profile);
public:
    profile(CtrlProfile *config_profiles,uint8_t index);
    void report();
    void reset();
};
