// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2022, Input Labs Oy.

#include "profile.h"
#include "board.h"
#include "ctrl.h"

// bool profile_led_lock = false;  // Extern.
// bool profile_pending_reboot = false;  // Extern.
// bool profile_reported_inputs = false;
// bool pending_reset = false;
// uint8_t pending_reset_keep;  // Action that must be kept between resets.
// bool home_is_active = false;
// bool home_gamepad_is_active = false;
// bool enabled_all = true;
// bool enabled_abxy = true;
// Button home;
// uint64_t hold_home_to_sleep_ts = 0;

void profile::report() {//Profile_t *self
//     if (!enabled_all) return;
//     bus_i2c_io_cache_update();
//     home.report(&home);
//     if (enabled_abxy) {
    Button::report(&profiles.a);
    Button::report(&profiles.b);
    Button::report(&profiles.x);
    Button::report(&profiles.y);
//     }
    Button::report(&profiles.dpad_left);
    Button::report(&profiles.dpad_right);
    Button::report(&profiles.dpad_up);
    Button::report(&profiles.dpad_down);
    Button::report(&profiles.select_1);
    Button::report(&profiles.select_2);
    Button::report(&profiles.start_2);
    Button::report(&profiles.start_1);
    Button::report(&profiles.l1);
    Button::report(&profiles.r1);
    Button::report(&profiles.l2);
    Button::report(&profiles.r2);
    Button::report(&profiles.l4);
    Button::report(&profiles.r4);
    profiles.rotary->report();
    profiles.left_thumbstick.report();
    profiles.right_thumbstick.report();
    profiles.gyro.report();
}

void profile::reset() {
//     self->a.reset(&self->a);
//     self->b.reset(&self->b);
//     self->x.reset(&self->x);
//     self->y.reset(&self->y);
//     self->dpad_left.reset(&self->dpad_left);
//     self->dpad_right.reset(&self->dpad_right);
//     self->dpad_up.reset(&self->dpad_up);
//     self->dpad_down.reset(&self->dpad_down);
//     self->select_1.reset(&self->select_1);
//     self->select_2.reset(&self->select_2);
//     self->start_2.reset(&self->start_1);
//     self->start_1.reset(&self->start_2);
//     self->l1.reset(&self->l1);
//     self->l2.reset(&self->l2);
//     self->r1.reset(&self->r1);
//     self->r2.reset(&self->r2);
//     self->l4.reset(&self->l4);
//     self->r4.reset(&self->r4);
//     self->dhat.reset(&self->dhat);
//     self->rotary.reset(&self->rotary);
//     self->left_thumbstick.reset(&self->left_thumbstick);
//     self->right_thumbstick.reset(&self->right_thumbstick);
//     self->gyro.reset(&self->gyro);
}

// Alternative init.
void profile::load_from_config(CtrlProfile *profile) {
    // Buttons.
    profiles.a =          Button_from_ctrl(PIN_A,          profile->sections[SECTION_A]);
    profiles.b =          Button_from_ctrl(PIN_B,          profile->sections[SECTION_B]);
    profiles.x =          Button_from_ctrl(PIN_X,          profile->sections[SECTION_X]);
    profiles.y =          Button_from_ctrl(PIN_Y,          profile->sections[SECTION_Y]);
    profiles.dpad_left =  Button_from_ctrl(PIN_DPAD_LEFT,  profile->sections[SECTION_DPAD_LEFT]);
    profiles.dpad_right = Button_from_ctrl(PIN_DPAD_RIGHT, profile->sections[SECTION_DPAD_RIGHT]);
    profiles.dpad_up =    Button_from_ctrl(PIN_DPAD_UP,    profile->sections[SECTION_DPAD_UP]);
    profiles.dpad_down =  Button_from_ctrl(PIN_DPAD_DOWN,  profile->sections[SECTION_DPAD_DOWN]);
    profiles.select_1 =   Button_from_ctrl(PIN_SELECT_1,   profile->sections[SECTION_SELECT_1]);
    profiles.select_2 =   Button_from_ctrl(PIN_SELECT_2,   profile->sections[SECTION_SELECT_2]);
    profiles.start_2 =    Button_from_ctrl(PIN_START_2,    profile->sections[SECTION_START_2]);
    profiles.start_1 =    Button_from_ctrl(PIN_START_1,    profile->sections[SECTION_START_1]);
    profiles.l1 =         Button_from_ctrl(PIN_L1,         profile->sections[SECTION_L1]);
    profiles.l2 =         Button_from_ctrl(PIN_L2,         profile->sections[SECTION_L2]);
    profiles.r1 =         Button_from_ctrl(PIN_R1,         profile->sections[SECTION_R1]);
    profiles.r2 =         Button_from_ctrl(PIN_R2,         profile->sections[SECTION_R2]);
    profiles.l4 =         Button_from_ctrl(PIN_L4,         profile->sections[SECTION_L4]);
    profiles.r4 =         Button_from_ctrl(PIN_R4,         profile->sections[SECTION_R4]);
    // Rotary.
    CtrlRotary up = profile->sections[SECTION_ROTARY_UP].rotary;
    CtrlRotary down = profile->sections[SECTION_ROTARY_DOWN].rotary;
    profiles.rotary = Board::GetInstance().rotary_;
    profiles.rotary->config_mode(0, up.actions_0, down.actions_0);
    profiles.rotary->config_mode(1, up.actions_1, down.actions_1);
    profiles.rotary->config_mode(2, up.actions_2, down.actions_2);
    profiles.rotary->config_mode(3, up.actions_3, down.actions_3);
    profiles.rotary->config_mode(4, up.actions_4, down.actions_4);
    profiles.rotary->set_mode(0);
    // Thumbsticks.
    profiles.left_thumbstick.from_ctrl(profile, 0);
    profiles.right_thumbstick.from_ctrl(profile, 1);
    // Gyro.
    CtrlGyro ctrl_gyro = profile->sections[SECTION_GYRO_SETTINGS].gyro;
    CtrlGyroAxis ctrl_gyro_x = profile->sections[SECTION_GYRO_X].gyro_axis;
    CtrlGyroAxis ctrl_gyro_y = profile->sections[SECTION_GYRO_Y].gyro_axis;
    CtrlGyroAxis ctrl_gyro_z = profile->sections[SECTION_GYRO_Z].gyro_axis;
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




// void profile_reset_all() {
//     // Reset HID state matrix. Optionally keep certain actions.
//     hid_matrix_reset(pending_reset_keep);
//     // Reset all profiles runtimes.
//     profile_reset_all_profiles();
//     // Reset flags.
//     pending_reset = false;
//     pending_reset_keep = 0;
// }

// void profile_check_home_sleep() {
//     // Check if home button is held super long, and go to sleep.
//     if (home_is_active && hold_home_to_sleep_ts) {
//         uint64_t threshold = hold_home_to_sleep_ts + (CFG_HOME_SLEEP_TIME*1000);
//         if (time_us_64() > threshold) {
//             info("Dormant mode requested by long press home\n");
//             power_dormant();
//         }
//         // Disable hold-home-sleep if there were reports (home profile was used).
//         if (profile_reported_inputs) {
//             profile_reset_home_sleep(false);
//         }
//     }
// }

// void profile_report_active() {
//     // Reboot if needed.
//     if (profile_pending_reboot && !home_is_active) power_restart();
//     // Reset all profiles (state) if needed.
//     if (pending_reset) profile_reset_all();
//     // Report active profile.
//     Profile* profile = profile_get_active(false);
//     profile_reported_inputs = false;
//     profile->report(profile);
//     profile_check_home_sleep();
// }

// void profile_reset_home_sleep(bool now) {
//     if (now) hold_home_to_sleep_ts = time_us_64();
//     else hold_home_to_sleep_ts = 0;
// }

// void profile_set_home(bool state) {
//     info("Profile: Home %s\n", state ? "on" : "off");
//     home_is_active = state;
//     if (state) {
//         led_static_mask(LED_ALL);
//         led_set_mode(LED_MODE_ENGAGE);
//         profile_reset_home_sleep(true);  // Init home-to-sleep timer.
//     } else {
//         profile_update_leds();
//     }
//     pending_reset = true;
// }

// void profile_set_home_gamepad(bool state) {
//     home_gamepad_is_active = state;
//     if (state) {
//         led_static_mask(LED_NONE);
//         led_set_mode(LED_MODE_ENGAGE);
//     } else {
//         profile_update_leds();
//     }
//     pending_reset = true;
//     pending_reset_keep = GAMEPAD_HOME;  // Do not reset held gamepad home.
// }

// void profile::set_active(uint8_t index) {
// //     // Reset hold-to-sleep countdown if user changes profile.
// //     profile_reset_home_sleep(true);
// //     // Change profile.
// //     if (index != profile_active_index) {
// //         info("Profile: Profile %i\n", index);
// //         profile_active_index = index;
// //         config_set_profile(index);
// //     }
// }

// Profile* profile_get_active(bool strict) {
//     if (strict) {
//         return &profiles[profile_active_index];
//     } else {
//         if (home_is_active) return &profiles[PROFILE_HOME];
//         else if (home_gamepad_is_active) return &profiles[PROFILE_HOME_GAMEPAD];
//         else return &profiles[profile_active_index];
//     }
// }

// Profile* profile_get(uint8_t index) {
//     return &profiles[index];
// }

// uint8_t profile_get_active_index(bool strict) {
//     if (strict) {
//         return profile_active_index;
//     } else {
//         if (home_is_active) return 0;
//         else return profile_active_index;
//     }
// }

// void profile_set_reported_inputs(bool value) {
//     profile_reported_inputs = value;
// }


// void profile_enable_abxy(bool value) {
//     enabled_abxy = value;
// }


profile::profile(CtrlProfile *config_profiles,uint8_t index) : Button() 
{
//     info("INIT: Profiles\n");
//     // Home button setup.
//     Actions actions = {PROC_HOME};
//     Actions actions_secondary = {0, 0, 0, 0};
//     Actions actions_terciary = {GAMEPAD_HOME, PROC_HOME_GAMEPAD, PROC_IGNORE_LED_WARNINGS};
//     home = Button_(PIN_HOME, DOUBLE|IMMEDIATE, actions, actions_secondary, actions_terciary);
//     // Profiles setup.
    // for(uint8_t i=0; i<PROFILE_SLOTS; i++) {
        load_from_config(&config_profiles[index]);
    // }
    //set_active(config_get_profile());
}
