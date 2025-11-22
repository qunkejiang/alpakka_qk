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

// bool synced_keyboard = false;
// bool synced_mouse = false;
// bool synced_gamepad = false;

bool synced_keyboard = true;
bool synced_mouse = true;
bool synced_gamepad = true;

uint8_t hid::state_matrix[PROC_MAX] = {0,};
float hid::mouse_x = 0;
float hid::mouse_y = 0;
float hid::gamepad_axis_data[6] = {0,};


void hid::procedure_press(uint8_t procedure){//qk
    profile *profile = Board::GetInstance().profile_;
    if (procedure == PROC_HOME) Board::set_profile(PROFILE_HOME); // Hold home.
    if (procedure == PROC_HOME_GAMEPAD) logging::info("HOME_GAMEPAD true\n");//profile_set_home_gamepad(true);  // Double-click-hold home.
    if (procedure == PROC_PROFILE_1) Board::set_profile(PROFILE_FPS_FUSION);
    if (procedure == PROC_PROFILE_2) Board::set_profile(PROFILE_RACING);
    if (procedure == PROC_PROFILE_3) Board::set_profile(PROFILE_CONSOLE);
    if (procedure == PROC_PROFILE_4) Board::set_profile(PROFILE_DESKTOP);
    if (procedure == PROC_PROFILE_5) Board::set_profile(PROFILE_FPS_WASD);
    if (procedure == PROC_PROFILE_6) Board::set_profile(PROFILE_FLIGHT);
    if (procedure == PROC_PROFILE_7) Board::set_profile(PROFILE_CONSOLE_LEGACY);
    if (procedure == PROC_PROFILE_8) Board::set_profile(PROFILE_RTS);
    if (procedure == PROC_PROFILE_9) Board::set_profile(PROFILE_CUSTOM_1);
    if (procedure == PROC_PROFILE_10) Board::set_profile(PROFILE_CUSTOM_2);
    if (procedure == PROC_PROFILE_11) Board::set_profile(PROFILE_CUSTOM_3);
    if (procedure == PROC_PROFILE_12) Board::set_profile(PROFILE_CUSTOM_4);
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
    if (procedure == PROC_THANKS) xTaskCreatePinnedToCore(thanks, "hid_thanks", 1024, NULL, 2, NULL, 1);
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
    if (procedure == PROC_MACRO_1) xTaskCreatePinnedToCore(macro, "hid_macro_1", 1024, (void *)1, 2, NULL, 1);
    if (procedure == PROC_MACRO_2) xTaskCreatePinnedToCore(macro, "hid_macro_2", 1024, (void *)2, 2, NULL, 1);
    if (procedure == PROC_MACRO_3) xTaskCreatePinnedToCore(macro, "hid_macro_3", 1024, (void *)3, 2, NULL, 1);
    if (procedure == PROC_MACRO_4) xTaskCreatePinnedToCore(macro, "hid_macro_4", 1024, (void *)4, 2, NULL, 1);
    if (procedure == PROC_MACRO_5) xTaskCreatePinnedToCore(macro, "hid_macro_5", 1024, (void *)5, 2, NULL, 1);
    if (procedure == PROC_MACRO_6) xTaskCreatePinnedToCore(macro, "hid_macro_6", 1024, (void *)6, 2, NULL, 1);
    if (procedure == PROC_MACRO_7) xTaskCreatePinnedToCore(macro, "hid_macro_7", 1024, (void *)7, 2, NULL, 1);
    if (procedure == PROC_MACRO_8) xTaskCreatePinnedToCore(macro, "hid_macro_8", 1024, (void *)8, 2, NULL, 1);
}

void hid::procedure_release(uint8_t procedure) {
    if (procedure == PROC_HOME) Board::set_profile(Board::get_nvm_data()->profile_index);
    if (procedure == PROC_HOME_GAMEPAD) logging::info("HOME_GAMEPAD false\n");//profile_set_home_gamepad(false);
}

void hid::press(uint8_t key) {
    if (key == KEY_NONE) return;
    else if (key >= PROC_HOME) procedure_press(key);
    else {
        state_matrix[key] += 1;
        if (key >= GAMEPAD_UP) synced_gamepad = false;
        else if (key >= MOUSE_1) synced_mouse = false;
        else synced_keyboard = false;
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
            if (key >= GAMEPAD_UP) synced_gamepad = false;
            else if (key >= MOUSE_1) synced_mouse = false;
            else synced_keyboard = false;
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

void hid::release_multiple_later(uint8_t *keys, uint16_t delay) {
    logging::debug_uart("release_multiple_later %d\n", delay);
    TimerHandle_t xTimer = xTimerCreate(
        "TimerName",                
        delay / portTICK_PERIOD_MS, 
        pdFALSE,                     // 自动重载（pdTRUE：周期性，pdFALSE：单次）
        keys,                       
        release_multiple_later_callback              // 回调函数
    );
    if (xTimer != NULL) {
        // 启动定时器
        xTimerStart(xTimer, 0);
    }
}


void hid::release_multiple_later_callback(TimerHandle_t xTimer) {
    uint8_t *keys = (uint8_t *)pvTimerGetTimerID(xTimer);
    release_multiple(keys);
}



void hid::macro(void *arg) {
    uint8_t index = static_cast<uint8_t>(reinterpret_cast<uintptr_t>(arg));
    logging::info("MACRO %d", index);
    uint8_t section = SECTION_MACRO_1 + ((index - 1) / 2);
    uint8_t subindex = (index - 1) % 2;
    CtrlProfile *ctrl_profile = Board::get_profile(Board::get_nvm_data()->profile_index);
    uint8_t *macro = ctrl_profile->sections[section].macro.macro[subindex];
    for(uint8_t i=0; i<28; i++) {
        if (macro[i] == 0) break;
        press(macro[i]);
        vTaskDelay(5);
        release(macro[i]);
        vTaskDelay(5);
    }
}

bool hid::is_axis(uint8_t key) {
    return is_between(key, GAMEPAD_AXIS_LX, GAMEPAD_AXIS_RZ_NEG);
}

void hid::mouse_move(float x, float y) {
    mouse_x += x;
    mouse_y += y;
    synced_mouse = false;
}

void hid::gamepad_axis(GamepadAxis axis, float value) {
    gamepad_axis_data[axis] += value;  // Multiple inputs can be combined.
}


void hid::report_mouse() {
    // Create button bitmask.
    uint8_t buttons = 0;
    for(int i=0; i<5; i++) {
        buttons |= (!!state_matrix[MOUSE_1 + i]) << i;
    }
    int8_t scroll = state_matrix[MOUSE_SCROLL_UP] - state_matrix[MOUSE_SCROLL_DOWN];
    state_matrix[MOUSE_SCROLL_UP] = 0;
    state_matrix[MOUSE_SCROLL_DOWN] = 0;
    // Create report.
    int16_t x = (int16_t)constrain(mouse_x, -32767.f, 32767.f);
    int16_t y = (int16_t)constrain(mouse_y, -32767.f, 32767.f);
    mouse_x -= (float)x;
    mouse_y -= (float)y;
    MouseReport report = {buttons, x, y, scroll, 0};
    tud_hid_report(REPORT_MOUSE, &report, sizeof(report));
    synced_mouse = true;
    static uint32_t last_report_mouse=0;
    if(++last_report_mouse < 100) return;
    last_report_mouse = 0;
    logging::debug_uart("report_mouse btn:%02x x:%d y:%d scroll:%d\n",
        report.buttons,
        report.x,
        report.y,
        report.scroll
    );
}

void hid::report_keyboard() {
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
    // Modifiers.
    uint8_t modifiers = 0;
    for(int i=0; i<8; i++) {
        // Any value bigger than 1 consolidates to 1 (with !!).
        modifiers |= (!!state_matrix[KEY_CONTROL_LEFT + i]) << i;
    }
    KeyboardReport report;
    report.modifier = modifiers;
    report.reserved = 0;
    memcpy(report.keycode, keys, 6);
    tud_hid_report(REPORT_KEYBOARD, &report, sizeof(report));
    synced_keyboard = true;
    logging::debug_uart("report_keyboard mod:%02x key:%02x %02x %02x %02x %02x %02x len:%d\n",
        report.modifier,
        report.keycode[0],
        report.keycode[1],
        report.keycode[2],
        report.keycode[3],
        report.keycode[4],
        report.keycode[5],
        sizeof(report)
    );
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


void hid::report_gamepad() {  // Sorted so the most common assigned buttons are lower and easier to
    // identify in-game.
    uint32_t buttons = (
        // Any value bigger than 1 consolidates to 1 (with !!).
        ((!!state_matrix[GAMEPAD_A])      <<  0) |
        ((!!state_matrix[GAMEPAD_B])      <<  1) |
        ((!!state_matrix[GAMEPAD_X])      <<  2) |
        ((!!state_matrix[GAMEPAD_Y])      <<  3) |
        ((!!state_matrix[GAMEPAD_L1])     <<  4) |
        ((!!state_matrix[GAMEPAD_R1])     <<  5) |
        ((!!state_matrix[GAMEPAD_L3])     <<  6) |
        ((!!state_matrix[GAMEPAD_R3])     <<  7) |
        ((!!state_matrix[GAMEPAD_LEFT])   <<  8) |
        ((!!state_matrix[GAMEPAD_RIGHT])  <<  9) |
        ((!!state_matrix[GAMEPAD_UP])     << 10) |
        ((!!state_matrix[GAMEPAD_DOWN])   << 11) |
        ((!!state_matrix[GAMEPAD_SELECT]) << 12) |
        ((!!state_matrix[GAMEPAD_START])  << 13) |
        ((!!state_matrix[GAMEPAD_HOME])   << 14)
    );
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
    
    for(uint8_t i=0; i<6; i++) gamepad_axis_data[i] = 0;
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
    synced_gamepad = true;
    logging::debug_uart("report_gamepad lx:%d ly:%d rx:%d ry:%d lz:%d rz:%d btn:%08x\n",
        report.lx,
        report.ly,
        report.rx,
        report.ry,
        report.lz,
        report.rz,
        report.buttons
    );
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
    for(uint8_t i=0; i<6; i++) gamepad_axis_data[i] = 0;
    
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
    synced_gamepad = true;

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

ReportType hid_get_priority() {

    if (!synced_keyboard) return REPORT_KEYBOARD;
    if (!synced_mouse) return REPORT_MOUSE;
    if (!synced_gamepad) {
        if (Board::get_protocol() == PROTOCOL_GENERIC) return REPORT_GAMEPAD;
        else return REPORT_XINPUT;
    }
    return REPORT_NONE;
}

void hid::report_wired() {
    ReportType device_to_report = hid_get_priority();
    tud_task();
    if (tud_ready()) {
        if (tud_hid_ready()) {
            webusb_read();
            webusb_flush();
            if (device_to_report == REPORT_KEYBOARD)report_keyboard();
            if (device_to_report == REPORT_MOUSE)report_mouse();
            if (device_to_report == REPORT_GAMEPAD)report_gamepad();
        }
        if (device_to_report == REPORT_XINPUT)
        {
            if (tud_suspended()) tud_remote_wakeup();
            report_xinput();
        }
    }
}


