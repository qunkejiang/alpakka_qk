// // SPDX-License-Identifier: GPL-2.0-only
// // Copyright (C) 2022, Input Labs Oy.

// /*
// HID layer is responsible for managing all the outputs that are sent to the
// operating system (via USB or wireless), by keeping a register of all the active
// actions (requested by the current profile) and syncing its state with the
// available interfaces (keyboard, mouse, gamepad...).

// Physical controller --> Profile --> [HID layer] --> HID keyboard
//                     --> Profile -->             --> HID mouse
//                     --> Profile -->             --> XInput gamepad
//                     --> Profile -->             --> Internal procedures

// At the end of each cycle (determined by the polling rate) the HID layer checks
// if the potential new report is different from the last report sent to the
// interfaces (USB keyboard, USB mouse, gamepad...), and sends the report if
// required.

// The state matrix is a representation of all the actions that could be sent
// (output) and internal operations (procedures) requested by the user. It keep
// track of how many references to these actions are active.
// As a simplification: Button presses increase the counter by one, and button
// releases decrease the counter by one.

// To avoid orphan references, the state matrix is usually re-initialized (reset)
// to zeros when the user changes the active profile, otherwise the disabled
// profile won't ever trigger the corresponding counter decrease of held buttons
// during the profile change.

// The replay feature was introduced as a simple mechanism to prevent stuck inputs
// if the last wireless report is lost (since the protocol does not have
// packet-received confirmation nor any resend logic yet).
// It works by re-sending (replaying) the last report of an specific report type
// several times, and therefore reducing the chances that all these packets are
// lost. To determine what is considered "last" it keeps counters of how many
// polling cycles passed since the last report (per report type), then after
// HID_REPLAY_THRESHOLD is excedeed the last report is replayed a fixed amount of
// times determined by HID_REPLAY_N_TIMES. When HID_REPLAY_N_TIMES is excedeed
// nothing will happen anymore until new inputs are sent, which will reset the
// replay counters.
// Flow diagram: docs/replay.md
// */

#include <tusb.h>
#include "hid.h"
#include "board.h"
#include "xinput.h"
#include "webusb.h"

// Toggle to prevent any further communication. Main use case being turning it
// off while the protocol is being changed to avoid incoherent outputs.

uint8_t hid::state_matrix[PROC_MAX] = {0,};
float hid::mouse_x = 0;
float hid::mouse_y = 0;
float hid::gamepad_axis_data[6] = {0,};


void hid::procedure_press(uint8_t procedure){//qk
    profile *profile = Board::GetInstance().profile_;

    if (procedure == PROC_HOME) Board::set_profile(PROFILE_HOME); // Hold home.
    if (procedure == PROC_HOME_GAMEPAD) logging::info("HOME_GAMEPAD true\n");//profile_set_home_gamepad(true);  // Double-click-hold home.
    if (procedure == PROC_PROFILE_1) Board::set_profile(1);
    if (procedure == PROC_PROFILE_2) Board::set_profile(2);
    if (procedure == PROC_PROFILE_3) Board::set_profile(3);
    if (procedure == PROC_PROFILE_4) Board::set_profile(4);
    if (procedure == PROC_PROFILE_5) Board::set_profile(5);
    if (procedure == PROC_PROFILE_6) Board::set_profile(6);
    if (procedure == PROC_PROFILE_7) Board::set_profile(7);
    if (procedure == PROC_PROFILE_8) Board::set_profile(8);
    if (procedure == PROC_PROFILE_9) Board::set_profile(9);
    if (procedure == PROC_PROFILE_10) Board::set_profile(10);
    if (procedure == PROC_PROFILE_11) Board::set_profile(11);
    if (procedure == PROC_PROFILE_12) Board::set_profile(12);
    if (procedure == PROC_TUNE_UP) logging::info("TUNE_UP\n");//config_tune(1);
    if (procedure == PROC_TUNE_DOWN) logging::info("TUNE_DOWN\n");//config_tune(0);
    if (procedure == PROC_TUNE_OS) logging::info("TUNE_OS\n");//config_tune_set_mode(procedure);
    if (procedure == PROC_TUNE_MOUSE_SENS) logging::info("TUNE_MOUSE_SENS\n");//config_tune_set_mode(procedure);
    if (procedure == PROC_TUNE_TOUCH_SENS) logging::info("TUNE_TOUCH_SENS\n");//config_tune_set_mode(procedure);
    if (procedure == PROC_TUNE_DEADZONE) logging::info("TUNE_DEADZONE\n");//config_tune_set_mode(procedure);
    if (procedure == PROC_CALIBRATE)
    {
        logging::info("CALIBRATE\n");
        Board::GetInstance().joystick_->calibration_step = 1;
        Board::GetInstance().imu_->calibration_step = 1;
    }
    if (procedure == PROC_RESTART) esp_restart();
    if (procedure == PROC_BOOTSEL) logging::info("BOOTSEL\n");//power_bootsel();  // TODO: BOOTSEL_OR_PAIR
    if (procedure == PROC_THANKS) xTaskCreatePinnedToCore(thanks, "hid_thanks", 1024, NULL, 5, NULL, 1);
    if (procedure == PROC_IGNORE_LED_WARNINGS) logging::info("IGNORE_LED_WARNINGS\n");//config_ignore_problems();
    if (procedure == PROC_SLEEP) logging::info("SLEEP\n");//power_dormant();
    // Scrollwheel alternative modes. (Used for example in Racing profile).
    if (procedure == PROC_ROTARY_MODE_0) profile->profiles.rotary.set_mode(0);
    if (procedure == PROC_ROTARY_MODE_1) profile->profiles.rotary.set_mode(1);
    if (procedure == PROC_ROTARY_MODE_2) profile->profiles.rotary.set_mode(2);
    if (procedure == PROC_ROTARY_MODE_3) profile->profiles.rotary.set_mode(3);
    if (procedure == PROC_ROTARY_MODE_4) profile->profiles.rotary.set_mode(4);
    if (procedure == PROC_ROTARY_MODE_5) profile->profiles.rotary.set_mode(5);
    // Macros.
    if (procedure == PROC_MACRO_1) macro(1);
    if (procedure == PROC_MACRO_2) macro(2);
    if (procedure == PROC_MACRO_3) macro(3);
    if (procedure == PROC_MACRO_4) macro(4);
    if (procedure == PROC_MACRO_5) macro(5);
    if (procedure == PROC_MACRO_6) macro(6);
    if (procedure == PROC_MACRO_7) macro(7);
    if (procedure == PROC_MACRO_8) macro(8);
}

void hid::procedure_release(uint8_t procedure) {
    if (procedure == PROC_HOME) 
    {
        Board::set_profile(Board::get_nvm_data()->profile_index); // Release home.
    }
    if (procedure == PROC_HOME_GAMEPAD) logging::info("HOME_GAMEPAD false\n");//profile_set_home_gamepad(false);
}

void hid::press(uint8_t key) {
    if (key == KEY_NONE) return;
    else if (key >= PROC_HOME) procedure_press(key);
    else {
        state_matrix[key] += 1;
    }
    logging::debug_uart("press %d\n", key);
}

void hid::release(uint8_t key) {
    if (key == KEY_NONE) return;
    else if (key == MOUSE_SCROLL_UP) return;
    else if (key == MOUSE_SCROLL_DOWN) return;
    else if (key >= PROC_HOME) procedure_release(key);
    else {
        if (state_matrix[key] > 0) {  // Do not allow to wrap / go negative.
            state_matrix[key] -= 1;
        }
    }
    logging::debug_uart("release %d\n", key);
}

void hid::press_multiple(uint8_t *keys) {
    for(uint8_t i=0; i<ACTIONS_LEN; i++) {
        if (keys[i] == 0) break;
        press(keys[i]);
    }
}

void hid::release_multiple(uint8_t *keys) {
    for(uint8_t i=0; i<ACTIONS_LEN; i++) {
        if (keys[i] == 0) return;
        release(keys[i]);
    }
}

void hid::release_multiple_later_callback(TimerHandle_t xTimer) {
    uint8_t *keys = (uint8_t *)pvTimerGetTimerID(xTimer);
    release_multiple(keys);
}
void hid::release_multiple_later(uint8_t *keys, uint16_t delay) {
    // TimerHandle_t xTimer = xTimerCreate(
    //     "TimerName",                
    //     delay / portTICK_PERIOD_MS, 
    //     pdFALSE,                     // 自动重载（pdTRUE：周期性，pdFALSE：单次）
    //     keys,                       
    //     release_multiple_later_callback              // 回调函数
    // );
    // if (xTimer != NULL) {
    //     // 启动定时器
    //     xTimerStart(xTimer, 0);
    // }
}





void hid::macro(uint8_t index) {//qk
     logging::info("MACRO %d", index);
//     uint8_t section = SECTION_MACRO_1 + ((index - 1) / 2);
//     uint8_t subindex = (index - 1) % 2;
//   CtrlProfile *profile = config_profile_read(profile_get_active_index(false));
//     uint8_t *macro = profile->sections[section].macro.macro[subindex];
//     if (alarms > 0) return;  // Disallows parallel macros. TODO fix.
//     uint16_t time = 10;
//     for(uint8_t i=0; i<28; i++) {
//         if (macro[i] == 0) break;
//         hid_press_later(macro[i], time);
//         time += 10;
//         hid_release_later(macro[i], time);
//         time += 10;
//     }
}

bool hid::is_axis(uint8_t key) {
    return is_between(key, GAMEPAD_AXIS_LX, GAMEPAD_AXIS_RZ_NEG);
}

void hid::mouse_move(float x, float y) {
    mouse_x += x;
    mouse_y += y;
}

void hid::gamepad_axis(GamepadAxis axis, float value) {
    gamepad_axis_data[axis] += value;  // Multiple inputs can be combined.
}


float hid::axis(
    float value,
    uint8_t matrix_index_pos,
    uint8_t matrix_index_neg
) {
    if (matrix_index_neg) {
        if (state_matrix[matrix_index_neg]) return -1;
        else if (state_matrix[matrix_index_pos]) return 1;
        else return constrain(value, -1, 1);
    } else {
        if (state_matrix[matrix_index_pos]) return 1;
        else return constrain(fabs(value), 0, 1);
    }
}




void hid::report_keyboard() {
    // Keys.
    uint8_t keys[6] = {0};
    uint8_t keys_available = 6;
    for(int i=0; i<=KEY_F24; i++) {
        if (state_matrix[i] >= 1) {
            keys[keys_available - 1] = (uint8_t)i;
            keys_available--;
            if (keys_available == 0) {
                break;
            }
        }
    }
    uint8_t modifiers = 0;
    for(int i=0; i<8; i++) {
        // Any value bigger than 1 consolidates to 1 (with !!).
        modifiers += !!state_matrix[KEY_CONTROL_LEFT + i] << i;
    }
    if((keys_available==6)&&(modifiers==0))
        return;
    // Modifiers.
    KeyboardReport report;
    report.modifier = modifiers;
    memcpy(report.keycode, keys, 6);
    //tud_hid_report(REPORT_KEYBOARD, &report, sizeof(report));
    logging::debug_uart("report_keyboard mod:%02x key:%02x %02x %02x %02x %02x %02x\n",
        report.modifier,
        report.keycode[0],
        report.keycode[1],
        report.keycode[2],
        report.keycode[3],
        report.keycode[4],
        report.keycode[5]
    );
}

void hid::report_mouse() {
    // Create button bitmask.
    uint8_t buttons = 0;
    for(int i=0; i<5; i++) {
        buttons |= (!!state_matrix[MOUSE_1 + i]) << i;
    }
    int8_t scroll = state_matrix[MOUSE_SCROLL_UP] - state_matrix[MOUSE_SCROLL_DOWN];
    // Create report.
    int16_t x = (int16_t)constrain(mouse_x, -32767.f, 32767.f);
    int16_t y = (int16_t)constrain(mouse_y, -32767.f, 32767.f);
    mouse_x -= (float)x;
    mouse_y -= (float)y;
    MouseReport report = {buttons, x, y, scroll, 0};
    tud_hid_report(REPORT_MOUSE, &report, sizeof(report));
    // logging::info("report_mouse btn:%02x x:%04x y:%04x scroll:%02x\n",
    //     report.buttons,
    //     report.x,
    //     report.y,
    //     report.scroll
    // );
}

void hid::report_gamepad() {  // Sorted so the most common assigned buttons are lower and easier to
    // identify in-game.
    uint32_t buttons = (
        // Any value bigger than 1 consolidates to 1 (with !!).
        ((!!state_matrix[GAMEPAD_A])      <<  0) +
        ((!!state_matrix[GAMEPAD_B])      <<  1) +
        ((!!state_matrix[GAMEPAD_X])      <<  2) +
        ((!!state_matrix[GAMEPAD_Y])      <<  3) +
        ((!!state_matrix[GAMEPAD_L1])     <<  4) +
        ((!!state_matrix[GAMEPAD_R1])     <<  5) +
        ((!!state_matrix[GAMEPAD_L3])     <<  6) +
        ((!!state_matrix[GAMEPAD_R3])     <<  7) +
        ((!!state_matrix[GAMEPAD_LEFT])   <<  8) +
        ((!!state_matrix[GAMEPAD_RIGHT])  <<  9) +
        ((!!state_matrix[GAMEPAD_UP])     << 10) +
        ((!!state_matrix[GAMEPAD_DOWN])   << 11) +
        ((!!state_matrix[GAMEPAD_SELECT]) << 12) +
        ((!!state_matrix[GAMEPAD_START])  << 13) +
        ((!!state_matrix[GAMEPAD_HOME])   << 14)
    );
    //qk gamepad_axis=0
    // Adjust range from [-1,1] to [-32767,32767].
    int16_t lx_report = axis(gamepad_axis_data[LX], GAMEPAD_AXIS_LX, GAMEPAD_AXIS_LX_NEG) * BIT_15;
    int16_t ly_report = axis(gamepad_axis_data[LY], GAMEPAD_AXIS_LY, GAMEPAD_AXIS_LY_NEG) * BIT_15;
    int16_t rx_report = axis(gamepad_axis_data[RX], GAMEPAD_AXIS_RX, GAMEPAD_AXIS_RX_NEG) * BIT_15;
    int16_t ry_report = axis(gamepad_axis_data[RY], GAMEPAD_AXIS_RY, GAMEPAD_AXIS_RY_NEG) * BIT_15;
    // HID triggers must be also defined as unsigned in the USB descriptor, and has to be manually
    // value-shifted from signed to unsigned here, otherwise Windows is having erratic behavior and
    // inconsistencies between games (not sure if a bug in Windows' DirectInput or TinyUSB).
    int16_t lz_report = ((axis(gamepad_axis_data[LZ], GAMEPAD_AXIS_LZ, 0) * 2) - 1) * BIT_15;
    int16_t rz_report = ((axis(gamepad_axis_data[RZ], GAMEPAD_AXIS_RZ, 0) * 2) - 1) * BIT_15;
    GamepadReport report = {
        lx_report,
        ly_report,
        rx_report,
        ry_report,
        lz_report,
        rz_report,
        buttons,
    };
    tud_hid_report(REPORT_GAMEPAD, &report, sizeof(report));
    // logging::info("report_gamepad lx:%04x ly:%04x rx:%04x ry:%04x lz:%04x rz:%04x btn:%08x\n",
    //     report.lx,
    //     report.ly,
    //     report.rx,
    //     report.ry,
    //     report.lz,
    //     report.rz,
    //     report.buttons
    // );
}

void hid::report_xinput() {
    uint8_t buttons_0 = 0;
    uint8_t buttons_1 = 0;
    // Button bitmask.
    // Any value bigger than 1 consolidates to 1 (with !!).
    for(int i=0; i<8; i++) {
        buttons_0 |= (!!state_matrix[GAMEPAD_UP + i]) << i;
    }
    for(int i=0; i<8; i++) {
        buttons_1 |= (!!state_matrix[GAMEPAD_UP + i + 8]) << i;
    }
    //qk gamepad_axis=0
    // Adjust range from [-1,1] to [-32767,32767].
    int16_t lx_report = axis(gamepad_axis_data[LX], GAMEPAD_AXIS_LX, GAMEPAD_AXIS_LX_NEG) * BIT_15;
    int16_t ly_report = axis(gamepad_axis_data[LY], GAMEPAD_AXIS_LY, GAMEPAD_AXIS_LY_NEG) * BIT_15;
    int16_t rx_report = axis(gamepad_axis_data[RX], GAMEPAD_AXIS_RX, GAMEPAD_AXIS_RX_NEG) * BIT_15;
    int16_t ry_report = axis(gamepad_axis_data[RY], GAMEPAD_AXIS_RY, GAMEPAD_AXIS_RY_NEG) * BIT_15;
    // Adjust range from [0,1] to [0,255].
    int16_t lz_report = axis(gamepad_axis_data[LZ], GAMEPAD_AXIS_LZ, 0) * BIT_8;
    int16_t rz_report = axis(gamepad_axis_data[RZ], GAMEPAD_AXIS_RZ, 0) * BIT_8;
    XInputReport report = {
        .report_id   = 0,
        .report_size = XINPUT_REPORT_SIZE,
        .buttons_0   = buttons_0,
        .buttons_1   = buttons_1,
        .lz          = (uint8_t)lz_report,
        .rz          = (uint8_t)rz_report,
        .lx          = lx_report,
        .ly          = ly_report,
        .rx          = rx_report,
        .ry          = ry_report,
        .reserved    = {0, 0, 0, 0, 0, 0}
    };
    xinput_send_report(&report);
}




void hid::thanks(void *arg)
{
    uint8_t thanks_len = 11;
    const uint8_t thanks_list[11][24] = {
        {19, 28, 14, 8, 8, 0},
        {18, 6, 11, 12, 8, 9, 0},
        {4, 21, 23, 8, 17, 10, 12, 17, 8, 8, 21, 0},
        {26, 8, 12, 6, 11, 8, 12, 0},
        {14, 8, 21, 16, 12, 23, 0},
        {19, 18, 22, 12, 23, 12, 18, 17, 27, 29, 0},
        {7, 8, 17, 17, 12, 22, 44, 23, 11, 8, 44, 16, 8, 17, 4, 6, 8, 0},
        {7, 8, 17, 10, 29, 18, 0},
        {13, 4, 5, 8, 10, 15, 18, 26, 0},
        {5, 8, 23, 23, 4, 6, 18, 21, 8, 0},
        {7, 24, 9, 23, 8, 0}
    };
    uint8_t r = (uint8_t) rand() % thanks_len;
    uint8_t x = 0;
    while (thanks_list[r][x])
    {
        hid::press(thanks_list[r][x]);
        vTaskDelay(5);
        hid::release(thanks_list[r][x]);
        vTaskDelay(5);
        x++;
    }
}

void hid::report_wired() {
    tud_task();
    if (tud_ready()) {
        if (tud_hid_ready()) {
            webusb_read();
            webusb_flush();
            report_keyboard();
            // report_mouse();
            // if (Board::get_protocol() == PROTOCOL_GENERIC)
            //     report_gamepad();
        }
        if (tud_suspended()) tud_remote_wakeup();
        // if (Board::get_protocol() != PROTOCOL_GENERIC)
        //     report_xinput();
    } 
}


