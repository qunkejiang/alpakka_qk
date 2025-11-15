// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2022, Input Labs Oy.
#include "profiles_default.h"



void config_profile_default_desktop(CtrlProfile *profile){
    // Metadata.
    profile->sections[SECTION_META].meta = (CtrlProfileMeta){
        .name = "Desktop",
        .control_byte = NVM_CONTROL_BYTE,
        .version_major = NVM_PROFILE_VERSION / 1000000,
        .version_minor = (NVM_PROFILE_VERSION / 1000) % 1000,
        .version_patch = NVM_PROFILE_VERSION % 1000,
        ._padding = {0}  // 添加填充字段初始化
    };

    // ABXY.
    profile->sections[SECTION_A].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_ENTER},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}  // 添加填充字段初始化
    };
    profile->sections[SECTION_B].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_ESCAPE},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_X].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_BACKSPACE},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_Y].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_SPACE},
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
        .actions={KEY_LEFT},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_DPAD_RIGHT].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_RIGHT},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_DPAD_UP].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_UP},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_DPAD_DOWN].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_DOWN},
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
        .actions={KEY_CONTROL_LEFT, KEY_Z},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="Undo",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_START_1].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_CONTROL_LEFT, KEY_SHIFT_LEFT, KEY_Z},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="Redo",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_SELECT_2].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_CONTROL_LEFT, KEY_C},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="Copy",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_START_2].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_CONTROL_LEFT, KEY_V},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="Paste",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };

    // Triggers.
    profile->sections[SECTION_L1].button = (CtrlButton){
        .mode=NORMAL,
        .actions={MOUSE_3},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_R1].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_ALT_LEFT},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_L2].button = (CtrlButton){
        .mode=NORMAL,
        .actions={MOUSE_2},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_R2].button = (CtrlButton){
        .mode=NORMAL,
        .actions={MOUSE_1},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_L4].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_CONTROL_LEFT},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_R4].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_SHIFT_LEFT},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };

    // Thumbstick (left).
    profile->sections[SECTION_LSTICK_SETTINGS].thumbstick = (CtrlThumbstick){
        .mode=(uint8_t)THUMBSTICK_MODE_ALPHANUMERIC,
        .distance_mode=0,
        .deadzone=0,
        .overlap=50,
        .deadzone_override=false,
        .antideadzone=0,
        .saturation=100,
        ._padding = {0}  // 添加填充字段初始化
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
    profile->sections[SECTION_RSTICK_UL].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_1},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_RSTICK_UP].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_2},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_RSTICK_UR].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_3},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_RSTICK_LEFT].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_4},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_RSTICK_PUSH].button = (CtrlButton){
        .mode=HOLD,
        .actions={KEY_5},
        .actions_secondary={KEY_0},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_RSTICK_RIGHT].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_6},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_RSTICK_DL].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_7},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_RSTICK_DOWN].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_8},
        .actions_secondary={KEY_NONE},
        .actions_terciary={KEY_NONE},
        .hint="",
        .hint_secondary="",
        .hint_terciary="",
        ._padding = {0}
    };
    profile->sections[SECTION_RSTICK_DR].button = (CtrlButton){
        .mode=NORMAL,
        .actions={KEY_9},
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
        .hint_0="",
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
        .hint_0="",
        .hint_1="",
        .hint_2="",
        .hint_3="",
        .hint_4=""
    };

    // // Daisywheel.
    // CtrlDaisyGroup up =         {.actions_a={KEY_A}, .actions_b={KEY_B}, .actions_x={KEY_C}, .actions_y={KEY_D}};
    // CtrlDaisyGroup up_right =   {.actions_a={KEY_E}, .actions_b={KEY_F}, .actions_x={KEY_G}, .actions_y={KEY_H}};
    // CtrlDaisyGroup left =       {.actions_a={KEY_I}, .actions_b={KEY_J}, .actions_x={KEY_K}, .actions_y={KEY_L}};
    // CtrlDaisyGroup right =      {.actions_a={KEY_O}, .actions_b={KEY_M}, .actions_x={KEY_N}, .actions_y={KEY_NONE}};
    // CtrlDaisyGroup down_left =  {.actions_a={KEY_P}, .actions_b={KEY_Q}, .actions_x={KEY_R}, .actions_y={KEY_S}};
    // CtrlDaisyGroup down =       {.actions_a={KEY_U}, .actions_b={KEY_T}, .actions_x={KEY_V}, .actions_y={KEY_NONE}};
    // CtrlDaisyGroup down_right = {.actions_a={KEY_W}, .actions_b={KEY_Z}, .actions_x={KEY_X}, .actions_y={KEY_Y}};
    // CtrlDaisyGroup up_left =    {
    //     .actions_a={KEY_COMMA},
    //     .actions_b={KEY_PERIOD},
    //     .actions_x={KEY_SHIFT_LEFT, KEY_2},      // @
    //     .actions_y={KEY_SHIFT_LEFT, KEY_SLASH},  // ?
    // };
    // profile->sections[SECTION_DAISY_0].daisy = (CtrlDaisy){.groups={left, right}};
    // profile->sections[SECTION_DAISY_1].daisy = (CtrlDaisy){.groups={up, down}};
    // profile->sections[SECTION_DAISY_2].daisy = (CtrlDaisy){.groups={up_left, up_right}};
    // profile->sections[SECTION_DAISY_3].daisy = (CtrlDaisy){.groups={down_left, down_right}};

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