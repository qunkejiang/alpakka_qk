// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2022, Input Labs Oy.

#pragma once
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define CTRL_MSG_SIZE 64
#define CTRL_NON_PAYLOAD_SIZE 4
#define CTRL_MAX_PAYLOAD_SIZE (CTRL_MSG_SIZE - CTRL_NON_PAYLOAD_SIZE)

enum HID_KEY {
    KEY_NONE = 0,
    KEY_A = 4,KEY_B,KEY_C,KEY_D,KEY_E,KEY_F,KEY_G,KEY_H,KEY_I,KEY_J,KEY_K,KEY_L,KEY_M,
    KEY_N,KEY_O,KEY_P,KEY_Q,KEY_R,KEY_S,KEY_T,KEY_U,KEY_V,KEY_W,KEY_X,KEY_Y,KEY_Z,
    KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_0,
    KEY_ENTER,KEY_ESCAPE,KEY_BACKSPACE,KEY_TAB,KEY_SPACE,KEY_MINUS,
    KEY_EQUALS,KEY_BRACKET_LEFT,KEY_BRACKET_RIGHT,KEY_BACKSLASH,
    KEY_ISO_1,
    KEY_SEMICOLON,KEY_QUOTE,KEY_BACKQUOTE,KEY_COMMA,KEY_PERIOD,KEY_SLASH,KEY_CAPS_LOCK,
    KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,KEY_F11,KEY_F12,
    KEY_PRINT_SCREEN,KEY_SCROLL_LOCK,KEY_PAUSE,KEY_INSERT,KEY_HOME,KEY_PAGE_UP,KEY_DELETE,KEY_END,KEY_PAGE_DOWN,
    KEY_RIGHT,KEY_LEFT,KEY_DOWN,KEY_UP,
    KEY_PAD_NUMLOCK,KEY_PAD_SLASH,KEY_PAD_ASTERISK,KEY_PAD_MINUS,KEY_PAD_PLUS,KEY_PAD_ENTER,
    KEY_PAD_1,KEY_PAD_2,KEY_PAD_3,KEY_PAD_4,KEY_PAD_5,KEY_PAD_6,KEY_PAD_7,KEY_PAD_8,KEY_PAD_9,KEY_PAD_0,
    KEY_PAD_PERIOD,
    KEY_ISO_2,
    KEY_POWER=102,
    KEY_F13=104,KEY_F14,KEY_F15,KEY_F16,KEY_F17,KEY_F18,KEY_F19,KEY_F20,KEY_F21,KEY_F22,KEY_F23,KEY_F24,
    KEY_MUTE=127,KEY_VOLUME_UP,KEY_VOLUME_DOWN,
    KEY_KANJI_1=135,KEY_KANJI_2,KEY_KANJI_3,KEY_KANJI_4,KEY_KANJI_5,KEY_KANJI_6,KEY_KANJI_7,KEY_KANJI_8,KEY_KANJI_9,
    KEY_LANG_1,KEY_LANG_2,KEY_LANG_3,KEY_LANG_4,KEY_LANG_5,KEY_LANG_6,KEY_LANG_7,KEY_LANG_8,KEY_LANG_9,
    KEY_CONTROL_LEFT=154,KEY_SHIFT_LEFT,KEY_ALT_LEFT,KEY_SUPER_LEFT,KEY_CONTROL_RIGHT,KEY_SHIFT_RIGHT,KEY_ALT_RIGHT,KEY_SUPER_RIGHT,
    MOUSE_1,MOUSE_2,MOUSE_3,MOUSE_4,MOUSE_5,MOUSE_SCROLL_UP,MOUSE_SCROLL_DOWN,MOUSE_X,MOUSE_Y,MOUSE_X_NEG,MOUSE_Y_NEG,
// Gamepad buttons sorted as in XInput.
    GAMEPAD_UP=174,GAMEPAD_DOWN,GAMEPAD_LEFT,GAMEPAD_RIGHT,GAMEPAD_START,GAMEPAD_SELECT,GAMEPAD_L3,GAMEPAD_R3,
    GAMEPAD_L1,GAMEPAD_R1,GAMEPAD_HOME,
    GAMEPAD_A=186,GAMEPAD_B,GAMEPAD_X,GAMEPAD_Y,
    GAMEPAD_AXIS_LX,GAMEPAD_AXIS_LY,GAMEPAD_AXIS_LZ,GAMEPAD_AXIS_RX,GAMEPAD_AXIS_RY,GAMEPAD_AXIS_RZ,
    GAMEPAD_AXIS_LX_NEG,GAMEPAD_AXIS_LY_NEG,GAMEPAD_AXIS_LZ_NEG,GAMEPAD_AXIS_RX_NEG,GAMEPAD_AXIS_RY_NEG,GAMEPAD_AXIS_RZ_NEG,
    PROC_HOME,PROC_PROFILE_1,PROC_PROFILE_2,PROC_PROFILE_3,PROC_PROFILE_4,PROC_PROFILE_5,PROC_PROFILE_6,
    PROC_PROFILE_7,PROC_PROFILE_8,PROC_PROFILE_9,PROC_PROFILE_10,PROC_PROFILE_11,PROC_PROFILE_12,
    PROC_TUNE_UP,PROC_TUNE_DOWN,PROC_TUNE_OS,PROC_TUNE_MOUSE_SENS,PROC_TUNE_TOUCH_SENS,PROC_TUNE_DEADZONE,
    PROC_CALIBRATE,PROC_RESTART,PROC_BOOTSEL,PROC_RESET_FACTORY,PROC_RESET_CONFIG,PROC_RESET_PROFILES,PROC_THANKS,PROC_HOME_GAMEPAD,
    PROC_MACRO_1,PROC_MACRO_2,PROC_MACRO_3,PROC_MACRO_4,PROC_MACRO_5,PROC_MACRO_6,PROC_MACRO_7,PROC_MACRO_8,
    PROC_ROTARY_MODE_0,PROC_ROTARY_MODE_1,PROC_ROTARY_MODE_2,PROC_ROTARY_MODE_3,PROC_ROTARY_MODE_4,PROC_ROTARY_MODE_5,
    PROC_IGNORE_LED_WARNINGS,PROC_SLEEP,PROC_PAIR,
    PROC_MAX,
};

typedef enum _Ctrl_protocol_flags {
    CTRL_FLAG_NONE = 1,
    CTRL_FLAG_WIRELESS,
} Ctrl_protocol_flags;

typedef enum _Ctrl_device {
    ALPAKKA = 1,
    KAPYBARA,
} Ctrl_device;

typedef enum Ctrl_msg_type_enum {
    LOG = 1,
    PROC,
    CONFIG_GET,
    CONFIG_SET,
    CONFIG_SHARE,
    SECTION_GET,
    SECTION_SET,
    SECTION_SHARE,
    STATUS_GET,
    STATUS_SET,
    STATUS_SHARE,
    PROFILE_OVERWRITE,
} Ctrl_msg_type;

typedef enum Ctrl_cfg_type_enum {
    PROTOCOL = 1,
    SENS_TOUCH,
    SENS_MOUSE,
    DEADZONE,
    LOG_MASK,
    LONG_CALIBRATION,
    SWAP_GYROS,
    TOUCH_INVERT_POLARITY,
    GYRO_USER_OFFSET,
    THUMBSTICK_SMOOTH_SAMPLES,
} Ctrl_cfg_type;

typedef enum CtrlSectionType_enum {
    // Unsorted indexes to keep backwards compatibility.
    SECTION_META = 1,
    SECTION_A,
    SECTION_B,
    SECTION_X,
    SECTION_Y,
    SECTION_DPAD_LEFT,
    SECTION_DPAD_RIGHT,
    SECTION_DPAD_UP,
    SECTION_DPAD_DOWN,
    SECTION_SELECT_1,
    SECTION_START_1,
    SECTION_SELECT_2,
    SECTION_START_2,
    SECTION_L1,
    SECTION_R1,
    SECTION_L2,
    SECTION_R2,
    SECTION_L4,
    SECTION_R4,
    SECTION_RSTICK_LEFT,
    SECTION_RSTICK_RIGHT,
    SECTION_RSTICK_UP,
    SECTION_RSTICK_DOWN,
    SECTION_RSTICK_UL,
    SECTION_RSTICK_UR,
    SECTION_RSTICK_DL,
    SECTION_RSTICK_DR,
    SECTION_RSTICK_PUSH,
    SECTION_ROTARY_UP,
    SECTION_ROTARY_DOWN,
    SECTION_LSTICK_SETTINGS,
    SECTION_LSTICK_LEFT,
    SECTION_LSTICK_RIGHT,
    SECTION_LSTICK_UP,
    SECTION_LSTICK_DOWN,
    SECTION_LSTICK_PUSH,
    SECTION_LSTICK_INNER,
    SECTION_LSTICK_OUTER,
    SECTION_GLYPHS_0,//弃用但保留索引
    SECTION_GLYPHS_1,//弃用
    SECTION_GLYPHS_2,//弃用
    SECTION_GLYPHS_3,//弃用
    SECTION_DAISY_0,//弃用
    SECTION_DAISY_1,//弃用
    SECTION_DAISY_2,//弃用
    SECTION_DAISY_3,//弃用
    SECTION_GYRO_SETTINGS,
    SECTION_GYRO_X,
    SECTION_GYRO_Y,
    SECTION_GYRO_Z,
    SECTION_MACRO_1,
    SECTION_MACRO_2,
    SECTION_MACRO_3,
    SECTION_MACRO_4,
    SECTION_LSTICK_UL,
    SECTION_LSTICK_UR,
    SECTION_LSTICK_DL,
    SECTION_LSTICK_DR,
    SECTION_RSTICK_SETTINGS,
    SECTION_RSTICK_INNER,
    SECTION_RSTICK_OUTER,
} CtrlSectionType;

typedef struct _Ctrl {
    Ctrl_protocol_flags protocol_flags:8;
    Ctrl_device device_id:8;
    Ctrl_msg_type message_type:8;
    uint8_t len;
    uint8_t payload[CTRL_MAX_PAYLOAD_SIZE];
} Ctrl;

typedef struct __packed _CtrlProfileMeta {
    // Must be packed (58 bytes).
    char name[24];
    uint8_t control_byte;
    uint8_t version_major;
    uint8_t version_minor;
    uint8_t version_patch;
    uint8_t _padding[30];
} CtrlProfileMeta;

typedef struct __packed _CtrlButton {
    // Must be packed (58 bytes).
    uint8_t mode;
    uint8_t actions[4];
    uint8_t actions_secondary[4];
    uint8_t actions_terciary[4];
    uint8_t hint[14];
    uint8_t hint_secondary[14];
    uint8_t hint_terciary[14];
    uint8_t _padding[3];
} CtrlButton;

typedef struct __packed _CtrlRotary {
    // Must be packed (58 bytes).
    uint8_t actions_0[4];
    uint8_t actions_1[4];
    uint8_t actions_2[4];
    uint8_t actions_3[4];
    uint8_t actions_4[4];
    uint8_t hint_0[14];
    uint8_t hint_1[6];
    uint8_t hint_2[6];
    uint8_t hint_3[6];
    uint8_t hint_4[6];
} CtrlRotary;

typedef struct __packed _CtrlThumbstick {
    // Must be packed (58 bytes).
    uint8_t mode;
    uint8_t distance_mode;
    uint8_t deadzone;
    int8_t overlap;
    uint8_t deadzone_override;
    uint8_t antideadzone;
    uint8_t saturation;
    uint8_t _padding[51];
} CtrlThumbstick;


typedef struct __packed _CtrlGyro {
    // Must be packed (58 bytes).
    uint8_t mode;
    uint8_t engage;
    uint8_t _padding[56];
} CtrlGyro;

typedef struct __packed _CtrlGyroAxis {
    // Must be packed (58 bytes).
    uint8_t actions_neg[4];
    uint8_t actions_pos[4];
    int8_t angle_min;
    int8_t angle_max;
    uint8_t hint_neg[14];
    uint8_t hint_pos[14];
    uint8_t _padding[20];
} CtrlGyroAxis;

typedef struct __packed _CtrlMacro {
    // Must be packed (58 bytes).
    uint8_t macro[2][28];
    uint8_t _padding[2];
} CtrlMacro;

typedef union _CtrlSection {
    CtrlButton button;
    CtrlRotary rotary;
    CtrlThumbstick thumbstick;
    CtrlGyro gyro;
    CtrlGyroAxis gyro_axis;
    CtrlProfileMeta meta;
    CtrlMacro macro;
} CtrlSection;

typedef struct _CtrlProfile {
    CtrlSection sections[64];// 64 sections
} CtrlProfile;

Ctrl ctrl_empty();
Ctrl ctrl_log(uint8_t* offset_ptr, uint8_t len);
Ctrl ctrl_status_share();
Ctrl ctrl_config_share(uint8_t index);
Ctrl ctrl_section_share(uint8_t profile_index, uint8_t section_index);

void ctrl_config_set(Ctrl_cfg_type key, uint8_t preset, uint8_t values[5]);
