// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2022, Input Labs Oy.
#include "profiles_default.h"

void config_profile_default_console(CtrlProfile *profile){
    // Metadata.
    profile->sections[SECTION_META].meta = (CtrlProfileMeta){
        .name = "Console",
        .control_byte = NVM_CONTROL_BYTE,
        .version_major = NVM_PROFILE_VERSION / 1000000,
        .version_minor = (NVM_PROFILE_VERSION / 1000) % 1000,
        .version_patch = NVM_PROFILE_VERSION % 1000,
        ._padding = {0}  // 添加填充字段初始化
    };

    // ABXY.
    profile->sections[SECTION_A].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_A},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}  // 添加填充字段初始化
    };
    profile->sections[SECTION_B].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_B},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_X].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_X},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_Y].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_Y},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };

    // DPad.
    profile->sections[SECTION_DPAD_LEFT].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_LEFT},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_DPAD_RIGHT].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_RIGHT},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_DPAD_UP].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_UP},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_DPAD_DOWN].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_DOWN},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };

    // Select/Start.
    profile->sections[SECTION_SELECT_1].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_SELECT},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_START_1].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_START},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_SELECT_2].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_M},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_START_2].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_N},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };

    // Triggers.
    profile->sections[SECTION_L1].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_L1},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_R1].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_R1},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_L2].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_AXIS_LZ},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_R2].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_AXIS_RZ},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_L4].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_SPACE},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_R4].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_CONTROL_LEFT},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };

    // Thumbstick (left).
    profile->sections[SECTION_LSTICK_SETTINGS].thumbstick = (CtrlThumbstick){
        .mode=(uint8_t)THUMBSTICK_MODE_4DIR,
        .distance_mode=0,
        .deadzone=0,
        .overlap=50,
        .deadzone_override=false,
        .antideadzone=0,
        .saturation=100,
        ._padding = {0}  // 添加填充字段初始化
    };
    profile->sections[SECTION_LSTICK_LEFT].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_AXIS_LX_NEG},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_LSTICK_RIGHT].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_AXIS_LX},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_LSTICK_UP].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_AXIS_LY_NEG},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_LSTICK_DOWN].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_AXIS_LY},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_LSTICK_PUSH].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_L3},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };

    // Thumbstick (right) / DHat.
    profile->sections[SECTION_RSTICK_SETTINGS].thumbstick = (CtrlThumbstick){
        .mode=(uint8_t)THUMBSTICK_MODE_8DIR,
        .distance_mode=0,
        .deadzone=60,
        .overlap=50,
        .deadzone_override=true,
        .antideadzone=0,
        .saturation=70,
        ._padding = {0}
    };
    profile->sections[SECTION_RSTICK_LEFT].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_1},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_RSTICK_DOWN].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_2},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_RSTICK_RIGHT].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_3},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_RSTICK_UP].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_4},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_RSTICK_DL].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_5},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_RSTICK_DR].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_6},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_RSTICK_UR].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_7},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_RSTICK_UL].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_8},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_RSTICK_PUSH].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_R3},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };

    // Rotary.
    profile->sections[SECTION_ROTARY_UP].rotary = (CtrlRotary){
        .actions_0={MOUSE_SCROLL_UP},
        .actions_1={KEY_NONE},
        .actions_2={KEY_NONE},
        .actions_3={KEY_NONE},
        .actions_4={KEY_NONE},
        .hint_0="Prev / Zoom+",
        .hint_1="",
        .hint_2="",
        .hint_3="",
        .hint_4=""
    };
    profile->sections[SECTION_ROTARY_DOWN].rotary = (CtrlRotary){
        .actions_0={MOUSE_SCROLL_DOWN},
        .actions_1={KEY_NONE},
        .actions_2={KEY_NONE},
        .actions_3={KEY_NONE},
        .actions_4={KEY_NONE},
        .hint_0="Next / Zoom-",
        .hint_1="",
        .hint_2="",
        .hint_3="",
        .hint_4=""
    };

    // Gyro.
    profile->sections[SECTION_GYRO_SETTINGS].gyro = (CtrlGyro){
        .mode=GYRO_MODE_TOUCH_ON,
        .engage=BUTTON_TOUCH_IN,
        ._padding = {0}  // 添加填充字段初始化
    };
    profile->sections[SECTION_GYRO_X].gyro_axis = (CtrlGyroAxis){
        .actions_neg={MOUSE_X_NEG},
        .actions_pos={MOUSE_X},
        .angle_min=-90,
        .angle_max=90,
        .hint_neg="",
        .hint_pos="Mouse",
        ._padding = {0}  // 添加填充字段初始化
    };
    profile->sections[SECTION_GYRO_Y].gyro_axis = (CtrlGyroAxis){
        .actions_neg={MOUSE_Y_NEG},
        .actions_pos={MOUSE_Y},
        .angle_min=-90,
        .angle_max=90,
        .hint_neg="",
        .hint_pos="Mouse",
        ._padding = {0}
    };
}