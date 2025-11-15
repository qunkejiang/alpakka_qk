// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2022, Input Labs Oy.

#include <stdlib.h>
#include <string.h>
#include "ctrl.h"
#include "board.h"

Ctrl ctrl_empty() {
    // For some reason, the very first USB message goes to "waste" and ignored
    // by the other end, but it is needed to kickstart the communication.
    // So we will be sending this bogus message first.
    Ctrl ctrl;
    ctrl.protocol_flags = CTRL_FLAG_NONE;
    ctrl.device_id = ALPAKKA;
    ctrl.message_type = (Ctrl_msg_type)0;
    ctrl.len = 0;
    return ctrl;
}

Ctrl ctrl_log(uint8_t* offset_ptr, uint8_t len) {
    Ctrl ctrl;
    ctrl.protocol_flags = CTRL_FLAG_NONE;
    ctrl.device_id = ALPAKKA;
    ctrl.message_type = LOG;
    ctrl.len = len;
    for (uint8_t i=0; i<len; i++) {
        ctrl.payload[i] = offset_ptr[i];
    }
    return ctrl;
}

Ctrl ctrl_status_share() {
    Ctrl ctrl;
    ctrl.protocol_flags = CTRL_FLAG_NONE;
    ctrl.device_id = ALPAKKA;
    ctrl.message_type = STATUS_SHARE;
    ctrl.len = 3;
    char version[] = VERSION;
    char *version_temp = version;
    char *dash = (char *)"-";
    char *dot = (char *)".";
    char *semantic;
    semantic = strsep(&version_temp, dash);  // Split semantic from git tag.
    ctrl.payload[0] = atoi(strsep(&semantic, dot));  // Version major.
    ctrl.payload[1] = atoi(strsep(&semantic, dot));  // Version mid.
    ctrl.payload[2] = atoi(strsep(&semantic, dot));  // Version minor.
    return ctrl;
}

void ctrl_config_set(Ctrl_cfg_type key, uint8_t preset, uint8_t values[5]) {
    Config_t* config = Board::get_nvm_data();    
    switch (key) {
        case PROTOCOL:
            config->usb_protocol = preset;
            break;
        case SENS_TOUCH:
            // 注意：此功能已弃用
            logging::info("Touch sensitivity configuration is deprecated\n");
            break;
        case SENS_MOUSE:
            config->sens_mouse_values[0] = values[0] / 10.0f;
            config->sens_mouse_values[1] = values[1] / 10.0f;
            config->sens_mouse_values[2] = values[2] / 10.0f;
            config->sens_mouse = preset;
            break;
        case DEADZONE:
            config->deadzone_values[0] = values[0] / 100.0f;
            config->deadzone_values[1] = values[1] / 100.0f;
            config->deadzone_values[2] = values[2] / 100.0f;
            config->deadzone = preset;
            break;
        case LOG_MASK:
            config->log_mask.usb = (preset & 1) != 0;
            config->log_mask.cpu = (preset & 2) != 0;
            config->log_mask.bt = (preset & 4) != 0;
            Board::GetInstance().cpu_->toggleTaskMonitor(config->log_mask.cpu);
            break;
        case LONG_CALIBRATION:
            config->long_calibration = preset;
            break;
        case SWAP_GYROS:
            config->swap_gyros = preset;
            break;
        case TOUCH_INVERT_POLARITY:
            config->touch_invert_polarity = preset;
            break;
        case GYRO_USER_OFFSET:
            config->offset_gyro_user_x = values[0];
            config->offset_gyro_user_y = values[1];
            config->offset_gyro_user_z = values[2];
            break;
        case THUMBSTICK_SMOOTH_SAMPLES:
            config->thumbstick_smooth_samples = preset;
            break;
    }
}

Ctrl ctrl_config_share(uint8_t index) {
    Config_t* config = Board::get_nvm_data();
    Ctrl ctrl;
    ctrl.protocol_flags = CTRL_FLAG_NONE;
    ctrl.device_id = ALPAKKA;
    ctrl.message_type = CONFIG_SHARE;
    ctrl.len = 7;
    ctrl.payload[0] = index;
    if (index == PROTOCOL) {
       ctrl.payload[1] = Board::get_protocol();
    }
    else if (index == SENS_TOUCH) {
        ctrl.payload[1] = 0;//弃用
        ctrl.payload[2] = 0;
        ctrl.payload[3] = 0;
        ctrl.payload[4] = 0;
        ctrl.payload[5] = 0;
        ctrl.payload[6] = 0;
    }
    else if (index == SENS_MOUSE) {
        ctrl.payload[1] = config->sens_mouse;
        ctrl.payload[2] = config->sens_mouse_values[0] * 10;
        ctrl.payload[3] = config->sens_mouse_values[1] * 10;
        ctrl.payload[4] = config->sens_mouse_values[2] * 10;
    }
    else if (index == DEADZONE) {
        ctrl.payload[1] = config->deadzone;
        ctrl.payload[2] = config->deadzone_values[0] * 100;
        ctrl.payload[3] = config->deadzone_values[1] * 100;
        ctrl.payload[4] = config->deadzone_values[2] * 100;
    }
    else if (index == LOG_MASK) {
        ctrl.payload[1] = (config->log_mask.usb<<0) | (config->log_mask.cpu<<1) | (config->log_mask.bt<<2);
    }
    else if (index == LONG_CALIBRATION) {
        ctrl.payload[1] = config->long_calibration;
    }
    else if (index == SWAP_GYROS) {
        ctrl.payload[1] = config->swap_gyros;
    }
    else if (index == TOUCH_INVERT_POLARITY) {
        ctrl.payload[1] = config->touch_invert_polarity;
    }
    else if (index == GYRO_USER_OFFSET) {
        ctrl.payload[2] = config->offset_gyro_user_x;
        ctrl.payload[3] = config->offset_gyro_user_y;
        ctrl.payload[4] = config->offset_gyro_user_z;
    }
    else if (index == THUMBSTICK_SMOOTH_SAMPLES) {
        ctrl.payload[1] = config->thumbstick_smooth_samples;
    }
    return ctrl;
}

Ctrl ctrl_section_share(uint8_t profile_index, uint8_t section_index) {
    Ctrl ctrl;
    ctrl.protocol_flags = CTRL_FLAG_NONE;
    ctrl.device_id = ALPAKKA;
    ctrl.message_type = SECTION_SHARE;
    ctrl.len = 60;
    // Profile section struct cast into packed int array.
    // Note that section structs must be guaranteed to be packed.
    CtrlProfile *profile = Board::get_profile(profile_index);
    uint8_t *section = (uint8_t*)&(profile->sections[section_index]);
    // Write payload.
    ctrl.payload[0] = profile_index;
    ctrl.payload[1] = section_index;
    for(uint8_t i=2; i<60; i++) {
        ctrl.payload[i] = section[i-2];
    }
    return ctrl;
}
