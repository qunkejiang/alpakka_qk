// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2022, Input Labs Oy.
#include "profiles_default.h"




void config_profile_default_console_legacy(CtrlProfile *profile){
    // Metadata.
    profile->sections[SECTION_META].meta = {
        .name = "Console Legacy",
        .control_byte = NVM_CONTROL_BYTE,
        .version_major = NVM_PROFILE_VERSION / 1000000,
        .version_minor = (NVM_PROFILE_VERSION / 1000) % 1000,
        .version_patch = NVM_PROFILE_VERSION % 1000,
        ._padding = {0} // 初始化填充字段
    };

    // ABXY.
    profile->sections[SECTION_A].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_A, 0},
        .actions_secondary={0},
        .actions_terciary={0},
        .hint="",
        .hint_secondary={0},
        .hint_terciary={0},
        ._padding={0}
    };
    profile->sections[SECTION_B].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_B, 0},
        .actions_secondary={0},
        .actions_terciary={0},
        .hint="",
        .hint_secondary={0},
        .hint_terciary={0},
        ._padding={0}
    };
    profile->sections[SECTION_X].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_X, 0},
        .actions_secondary={0},
        .actions_terciary={0},
        .hint="",
        .hint_secondary={0},
        .hint_terciary={0},
        ._padding={0}
    };
    profile->sections[SECTION_Y].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_Y, 0},
        .actions_secondary={0},
        .actions_terciary={0},
        .hint="",
        .hint_secondary={0},
        .hint_terciary={0},
        ._padding={0}
    };

    // DPad.
    profile->sections[SECTION_DPAD_LEFT].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_LEFT, 0},
        .actions_secondary={0},
        .actions_terciary={0},
        .hint="",
        .hint_secondary={0},
        .hint_terciary={0},
        ._padding={0}
    };
    profile->sections[SECTION_DPAD_RIGHT].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_RIGHT, 0},
        .actions_secondary={0},
        .actions_terciary={0},
        .hint="",
        .hint_secondary={0},
        .hint_terciary={0},
        ._padding={0}
    };
    profile->sections[SECTION_DPAD_UP].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_UP, 0},
        .actions_secondary={0},
        .actions_terciary={0},
        .hint="",
        .hint_secondary={0},
        .hint_terciary={0},
        ._padding={0}
    };
    profile->sections[SECTION_DPAD_DOWN].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_DOWN, 0},
        .actions_secondary={0},
        .actions_terciary={0},
        .hint="",
        .hint_secondary={0},
        .hint_terciary={0},
        ._padding={0}
    };

    // Select/Start.
    profile->sections[SECTION_SELECT_1].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_SELECT, 0},
        .actions_secondary={0},
        .actions_terciary={0},
        .hint="",
        .hint_secondary={0},
        .hint_terciary={0},
        ._padding={0}
    };
    profile->sections[SECTION_START_1].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_START, 0},
        .actions_secondary={0},
        .actions_terciary={0},
        .hint="",
        .hint_secondary={0},
        .hint_terciary={0},
        ._padding={0}
    };
    profile->sections[SECTION_SELECT_2].button = (CtrlButton){
        .mode=NORMAL,
        .actions={0, 0, 0, 0},
        .actions_secondary={0, 0, 0, 0},
        .actions_terciary={0, 0, 0, 0},
        .hint="",
        .hint_secondary={0},
        .hint_terciary={0},
        ._padding={0}
    };
    profile->sections[SECTION_START_2].button = (CtrlButton){
        .mode=NORMAL,
        .actions={0, 0, 0, 0},
        .actions_secondary={0, 0, 0, 0},
        .actions_terciary={0, 0, 0, 0},
        .hint="",
        .hint_secondary={0},
        .hint_terciary={0},
        ._padding={0}
    };

    // Triggers.
    profile->sections[SECTION_L1].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_L1, 0, 0, 0},
        .actions_secondary={0, 0, 0, 0},
        .actions_terciary={0, 0, 0, 0},
        .hint="",
        .hint_secondary={0},
        .hint_terciary={0},
        ._padding={0}
    };
    profile->sections[SECTION_R1].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_R1, 0, 0, 0},
        .actions_secondary={0, 0, 0, 0},
        .actions_terciary={0, 0, 0, 0},
        .hint="",
        .hint_secondary={0},
        .hint_terciary={0},
        ._padding={0}
    };
    profile->sections[SECTION_L2].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_AXIS_LZ, 0, 0, 0},
        .actions_secondary={0, 0, 0, 0},
        .actions_terciary={0, 0, 0, 0},
        .hint="",
        .hint_secondary={0},
        .hint_terciary={0},
        ._padding={0}
    };
    profile->sections[SECTION_R2].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_AXIS_RZ, 0, 0, 0},
        .actions_secondary={0, 0, 0, 0},
        .actions_terciary={0, 0, 0, 0},
        .hint="",
        .hint_secondary={0},
        .hint_terciary={0},
        ._padding={0}
    };
    profile->sections[SECTION_L4].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_A, 0, 0, 0},
        .actions_secondary={0, 0, 0, 0},
        .actions_terciary={0, 0, 0, 0},
        .hint="",
        .hint_secondary={0},
        .hint_terciary={0},
        ._padding={0}
    };
    profile->sections[SECTION_R4].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_B, 0, 0, 0},
        .actions_secondary={0, 0, 0, 0},
        .actions_terciary={0, 0, 0, 0},
        .hint="",
        .hint_secondary={0},
        .hint_terciary={0},
        ._padding={0}
    };

    // Thumbstick (left).
    profile->sections[SECTION_LSTICK_SETTINGS].thumbstick = (CtrlThumbstick){
        .mode=(uint8_t)THUMBSTICK_MODE_4DIR,
        .distance_mode=THUMBSTICK_DISTANCE_RADIAL,
        .deadzone=0,
        .overlap=50,
        .deadzone_override=false,
        .antideadzone=0,
        .saturation=100,
        ._padding={0}
    };
    profile->sections[SECTION_LSTICK_LEFT].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_AXIS_LX_NEG, 0, 0, 0},
        .actions_secondary={0, 0, 0, 0},
        .actions_terciary={0, 0, 0, 0},
        .hint="",
        .hint_secondary={0},
        .hint_terciary={0},
        ._padding={0}
    };
    profile->sections[SECTION_LSTICK_RIGHT].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_AXIS_LX, 0, 0, 0},
        .actions_secondary={0, 0, 0, 0},
        .actions_terciary={0, 0, 0, 0},
        .hint="",
        .hint_secondary={0},
        .hint_terciary={0},
        ._padding={0}
    };
    profile->sections[SECTION_LSTICK_UP].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_AXIS_LY_NEG, 0, 0, 0},
        .actions_secondary={0, 0, 0, 0},
        .actions_terciary={0, 0, 0, 0},
        .hint="",
        .hint_secondary={0},
        .hint_terciary={0},
        ._padding={0}
    };
    profile->sections[SECTION_LSTICK_DOWN].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_AXIS_LY, 0, 0, 0},
        .actions_secondary={0, 0, 0, 0},
        .actions_terciary={0, 0, 0, 0},
        .hint="",
        .hint_secondary={0},
        .hint_terciary={0},
        ._padding={0}
    };
    profile->sections[SECTION_LSTICK_PUSH].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_L3, 0, 0, 0},
        .actions_secondary={0, 0, 0, 0},
        .actions_terciary={0, 0, 0, 0},
        .hint="",
        .hint_secondary={0},
        .hint_terciary={0},
        ._padding={0}
    };

    // Thumbstick (right) / DHat.
    profile->sections[SECTION_RSTICK_SETTINGS].thumbstick = (CtrlThumbstick){
        .mode=(uint8_t)THUMBSTICK_MODE_4DIR,
        .distance_mode=THUMBSTICK_DISTANCE_RADIAL,
        .deadzone=15,
        .overlap=50,
        .deadzone_override=true,
        .antideadzone=0,
        .saturation=70,
        ._padding={0}
    };
    profile->sections[SECTION_RSTICK_LEFT].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_AXIS_RX_NEG, 0, 0, 0},
        .actions_secondary={0, 0, 0, 0},
        .actions_terciary={0, 0, 0, 0},
        .hint="",
        .hint_secondary={0},
        .hint_terciary={0},
        ._padding={0}
    };
    profile->sections[SECTION_RSTICK_RIGHT].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_AXIS_RX, 0, 0, 0},
        .actions_secondary={0, 0, 0, 0},
        .actions_terciary={0, 0, 0, 0},
        .hint="",
        .hint_secondary={0},
        .hint_terciary={0},
        ._padding={0}
    };
    profile->sections[SECTION_RSTICK_UP].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_AXIS_RY_NEG, 0, 0, 0},
        .actions_secondary={0, 0, 0, 0},
        .actions_terciary={0, 0, 0, 0},
        .hint="",
        .hint_secondary={0},
        .hint_terciary={0},
        ._padding={0}
    };
    profile->sections[SECTION_RSTICK_DOWN].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_AXIS_RY, 0, 0, 0},
        .actions_secondary={0, 0, 0, 0},
        .actions_terciary={0, 0, 0, 0},
        .hint="",
        .hint_secondary={0},
        .hint_terciary={0},
        ._padding={0}
    };
    profile->sections[SECTION_RSTICK_PUSH].button = (CtrlButton){
        .mode=NORMAL,
        .actions={GAMEPAD_R3, 0, 0, 0},
        .actions_secondary={0, 0, 0, 0},
        .actions_terciary={0, 0, 0, 0},
        .hint="",
        .hint_secondary={0},
        .hint_terciary={0},
        ._padding={0}
    };

    // Rotary.
    profile->sections[SECTION_ROTARY_UP].rotary = (CtrlRotary){
        .actions_0={MOUSE_SCROLL_UP, 0, 0, 0},
        .actions_1={0, 0, 0, 0},
        .actions_2={0, 0, 0, 0},
        .actions_3={0, 0, 0, 0},
        .actions_4={0, 0, 0, 0},
        .hint_0="Prev / Zoom+",
        .hint_1="",
        .hint_2="",
        .hint_3="",
        .hint_4=""
    };
    profile->sections[SECTION_ROTARY_DOWN].rotary = (CtrlRotary){
        .actions_0={MOUSE_SCROLL_DOWN, 0, 0, 0},
        .actions_1={0, 0, 0, 0},
        .actions_2={0, 0, 0, 0},
        .actions_3={0, 0, 0, 0},
        .actions_4={0, 0, 0, 0},
        .hint_0="Next / Zoom-",
        .hint_1="",
        .hint_2="",
        .hint_3="",
        .hint_4=""
    };

    // Gyro.
    profile->sections[SECTION_GYRO_SETTINGS].gyro = (CtrlGyro){
        .mode=GYRO_MODE_OFF,
        .engage=0,
        ._padding={0}
    };
}