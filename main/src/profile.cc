// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2022, Input Labs Oy.

#include "profile.h"
#include "board.h"
#include "ctrl.h"


void profile::report() {
    Button::report(&home);
 
    CtrlProfile *ctrl_profile = Board::get_profile(Board::get_nvm_data()->profile_index);
    bool mode_state = (ctrl_profile->sections[SECTION_RSTICK_SETTINGS].thumbstick.mode == THUMBSTICK_MODE_ALPHANUMERIC) ||
                      (ctrl_profile->sections[SECTION_LSTICK_SETTINGS].thumbstick.mode == THUMBSTICK_MODE_ALPHANUMERIC);
    if (!mode_state) {
        Button::report(&profiles.a);
        Button::report(&profiles.b);
        Button::report(&profiles.x);
        Button::report(&profiles.y);
        Button::report(&profiles.start_1);
        Button::report(&profiles.start_2);
    }
    Button::report(&profiles.dpad_left);
    Button::report(&profiles.dpad_right);
    Button::report(&profiles.dpad_up);
    Button::report(&profiles.dpad_down);
    Button::report(&profiles.select_1);
    Button::report(&profiles.select_2);
    Button::report(&profiles.l1);
    Button::report(&profiles.r1);
    Button::report(&profiles.l2);
    Button::report(&profiles.r2);
    Button::report(&profiles.l4);
    Button::report(&profiles.r4);
    profiles.rotary.report();
    profiles.left_thumbstick.report();
    profiles.right_thumbstick.report();
    profiles.gyro.report();
}

void profile::reset() {
    Button::reset(&profiles.a);
    Button::reset(&profiles.b);
    Button::reset(&profiles.x);
    Button::reset(&profiles.y);
    Button::reset(&profiles.dpad_left);
    Button::reset(&profiles.dpad_right);
    Button::reset(&profiles.dpad_up);
    Button::reset(&profiles.dpad_down);
    Button::reset(&profiles.select_1);
    Button::reset(&profiles.select_2);
    Button::reset(&profiles.start_2);
    Button::reset(&profiles.start_1);
    Button::reset(&profiles.l1);
    Button::reset(&profiles.r1);
    Button::reset(&profiles.l2);
    Button::reset(&profiles.r2);
    Button::reset(&profiles.l4);
    Button::reset(&profiles.r4);
    Button::reset(&profiles.hat_down);
    Button::reset(&profiles.hat_right);
    Button::reset(&profiles.hat_left);
    Button::reset(&profiles.hat_up);
    Button::reset(&profiles.hat_in);
    profiles.left_thumbstick.reset();
    profiles.right_thumbstick.reset();
    profiles.gyro.reset();
}

// Alternative init.
void profile::load_from_config(CtrlProfile *ctrl_profile) {
    // Buttons.
    profiles.a =          Button_from_ctrl(BUTTON_A,          ctrl_profile->sections[SECTION_A]);
    profiles.b =          Button_from_ctrl(BUTTON_B,          ctrl_profile->sections[SECTION_B]);
    profiles.x =          Button_from_ctrl(BUTTON_X,          ctrl_profile->sections[SECTION_X]);
    profiles.y =          Button_from_ctrl(BUTTON_Y,          ctrl_profile->sections[SECTION_Y]);
    profiles.dpad_left =  Button_from_ctrl(BUTTON_DPAD_LEFT,  ctrl_profile->sections[SECTION_DPAD_LEFT]);
    profiles.dpad_right = Button_from_ctrl(BUTTON_DPAD_RIGHT, ctrl_profile->sections[SECTION_DPAD_RIGHT]);
    profiles.dpad_up =    Button_from_ctrl(BUTTON_DPAD_UP,    ctrl_profile->sections[SECTION_DPAD_UP]);
    profiles.dpad_down =  Button_from_ctrl(BUTTON_DPAD_DOWN,  ctrl_profile->sections[SECTION_DPAD_DOWN]);
    profiles.select_1 =   Button_from_ctrl(BUTTON_SELECT_1,   ctrl_profile->sections[SECTION_SELECT_1]);
    profiles.select_2 =   Button_from_ctrl(BUTTON_SELECT_2,   ctrl_profile->sections[SECTION_SELECT_2]);
    profiles.start_2 =    Button_from_ctrl(BUTTON_START_2,    ctrl_profile->sections[SECTION_START_2]);
    profiles.start_1 =    Button_from_ctrl(BUTTON_START_1,    ctrl_profile->sections[SECTION_START_1]);
    profiles.l1 =         Button_from_ctrl(BUTTON_L1,         ctrl_profile->sections[SECTION_L1]);
    profiles.l2 =         Button_from_ctrl(BUTTON_L2,         ctrl_profile->sections[SECTION_L2]);
    profiles.r1 =         Button_from_ctrl(BUTTON_R1,         ctrl_profile->sections[SECTION_R1]);
    profiles.r2 =         Button_from_ctrl(BUTTON_R2,         ctrl_profile->sections[SECTION_R2]);
    profiles.l4 =         Button_from_ctrl(BUTTON_L4,         ctrl_profile->sections[SECTION_L4]);
    profiles.r4 =         Button_from_ctrl(BUTTON_R4,         ctrl_profile->sections[SECTION_R4]);
    // Rotary.
    CtrlRotary up = ctrl_profile->sections[SECTION_ROTARY_UP].rotary;
    CtrlRotary down = ctrl_profile->sections[SECTION_ROTARY_DOWN].rotary;
    profiles.rotary.config_mode(0, up.actions_0, down.actions_0);
    profiles.rotary.config_mode(1, up.actions_1, down.actions_1);
    profiles.rotary.config_mode(2, up.actions_2, down.actions_2);
    profiles.rotary.config_mode(3, up.actions_3, down.actions_3);
    profiles.rotary.config_mode(4, up.actions_4, down.actions_4);
    profiles.rotary.set_mode(0);
    // Thumbsticks.
    profiles.left_thumbstick.from_ctrl(ctrl_profile, 0);
    profiles.right_thumbstick.from_ctrl(ctrl_profile, 1);
    // Gyro.
    CtrlGyro ctrl_gyro = ctrl_profile->sections[SECTION_GYRO_SETTINGS].gyro;
    CtrlGyroAxis ctrl_gyro_x = ctrl_profile->sections[SECTION_GYRO_X].gyro_axis;
    CtrlGyroAxis ctrl_gyro_y = ctrl_profile->sections[SECTION_GYRO_Y].gyro_axis;
    CtrlGyroAxis ctrl_gyro_z = ctrl_profile->sections[SECTION_GYRO_Z].gyro_axis;
    profiles.gyro.from_ctrl((GyroMode)ctrl_gyro.mode,ctrl_gyro.engage);
    profiles.gyro.config_x(
        (int8_t)ctrl_gyro_x.angle_min,
        (int8_t)ctrl_gyro_x.angle_max,
        ctrl_gyro_x.actions_neg,
        ctrl_gyro_x.actions_pos
    );
    profiles.gyro.config_y(
        (int8_t)ctrl_gyro_y.angle_min,
        (int8_t)ctrl_gyro_y.angle_max,
        ctrl_gyro_y.actions_neg,
        ctrl_gyro_y.actions_pos
    );
    profiles.gyro.config_z(
        (int8_t)ctrl_gyro_z.angle_min,
        (int8_t)ctrl_gyro_z.angle_max,
        ctrl_gyro_z.actions_neg,
        ctrl_gyro_z.actions_pos
    );
}




profile::profile(CtrlProfile *ctrl_profile) : Button()
{
    // Home button setup.
    Actions actions = {PROC_HOME, 0};
    Actions actions_secondary = {0, 0, 0, 0};
    Actions actions_terciary = {GAMEPAD_HOME, PROC_HOME_GAMEPAD, PROC_IGNORE_LED_WARNINGS, 0};
    home = Button_from_params(BUTTON_HOME, (ButtonMode)(DOUBLE|IMMEDIATE), actions, actions_secondary, actions_terciary);
    //Profiles setup.
    load_from_config(ctrl_profile);
}
