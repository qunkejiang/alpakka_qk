// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2022, Input Labs Oy.
#pragma once

#include "button.h"
#include "ctrl.h"
#include "rotary.h"
#include "thumbstick.h"
#include "gyro.h"


typedef enum ProfileIndex_enum {
    PROFILE_HOME=0,
    PROFILE_FPS_FUSION,
    PROFILE_RACING,
    PROFILE_CONSOLE,
    PROFILE_DESKTOP,
    PROFILE_FPS_WASD,
    PROFILE_FLIGHT,
    PROFILE_CONSOLE_LEGACY,
    PROFILE_RTS,
    PROFILE_CUSTOM_1,
    PROFILE_CUSTOM_2,
    PROFILE_CUSTOM_3,
    PROFILE_CUSTOM_4,
    PROFILE_HOME_GAMEPAD,
    PROFILE_SLOTS
} ProfileIndex;

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
    Rotary rotary;
    Gyro gyro;
};

class  profile: public Button
{
private:
    Button_t home;
public:
    Profile_t profiles;
    //Profile_t profiles_home;

    //void set_home(bool state);
    void load_from_config(CtrlProfile *ctrl_profile);
    void report();
    void reset();
    profile(CtrlProfile *ctrl_profile);
};
