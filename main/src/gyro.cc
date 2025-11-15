// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2022, Input Labs Oy.

#include "board.h"
#include "gyro.h"
#include "hid.h"




void gyro_absolute_output(float value, uint8_t *actions, bool *pressed) {
    for(uint8_t i=0; i<4; i++) {
        uint8_t action = actions[i];
        if (hid::is_axis(action)) {
            value = fabs(value);
            if      (action == GAMEPAD_AXIS_LX)     hid::gamepad_axis(LX,  value);
            else if (action == GAMEPAD_AXIS_LY)     hid::gamepad_axis(LY,  value);
            else if (action == GAMEPAD_AXIS_LZ)     hid::gamepad_axis(LZ,  value);
            else if (action == GAMEPAD_AXIS_RX)     hid::gamepad_axis(RX,  value);
            else if (action == GAMEPAD_AXIS_RY)     hid::gamepad_axis(RY,  value);
            else if (action == GAMEPAD_AXIS_RZ)     hid::gamepad_axis(RZ,  value);
            else if (action == GAMEPAD_AXIS_LX_NEG) hid::gamepad_axis(LX, -value);
            else if (action == GAMEPAD_AXIS_LY_NEG) hid::gamepad_axis(LY, -value);
            else if (action == GAMEPAD_AXIS_LZ_NEG) hid::gamepad_axis(LZ, -value);
            else if (action == GAMEPAD_AXIS_RX_NEG) hid::gamepad_axis(RX, -value);
            else if (action == GAMEPAD_AXIS_RY_NEG) hid::gamepad_axis(RY, -value);
            else if (action == GAMEPAD_AXIS_RZ_NEG) hid::gamepad_axis(RZ, -value);
        } else {
            if (!(*pressed) && value >= 0.5) {
                hid::press(action);
                if (i==3) *pressed = true;
            }
            else if (*pressed && value < 0.5) {
                hid::release(action);
                if (i==3) *pressed = false;
            }
        }
    }
}

void Gyro::gyro_incremental_output(float value, uint8_t *actions) {
    for(uint8_t i=0; i<4; i++) {
        uint8_t action = actions[i];
        if      (action == MOUSE_X)     hid::mouse_move(value, 0L);
        else if (action == MOUSE_Y)     hid::mouse_move(0L, value);
        else if (action == MOUSE_X_NEG) hid::mouse_move(-value, 0L);
        else if (action == MOUSE_Y_NEG) hid::mouse_move(0L, -value);
    }
}


void Gyro::report_absolute() {
    // Get data from gyros.
    float x = Board::get_imu_data()->pose.rMat[0];
    float y = Board::get_imu_data()->pose.rMat[1];
    //float z = rMat[2][2];
    // Output mapping.
    x = ramp(x, gyro.absolute_x_min/90, gyro.absolute_x_max/90); // Adjust range.
    y = ramp(y, gyro.absolute_y_min/90, gyro.absolute_y_max/90); // Adjust range.
    if (x >= 0) gyro_absolute_output( x, gyro.actions_x_pos, &(gyro.pressed_x_pos));
    else        gyro_absolute_output(-x, gyro.actions_x_neg, &(gyro.pressed_x_neg));
    if (y >= 0) gyro_absolute_output( y, gyro.actions_y_pos, &(gyro.pressed_y_pos));
    else        gyro_absolute_output(-y, gyro.actions_y_neg, &(gyro.pressed_y_neg));
    // printf("\r%6.1f %6.1f %6.1f", x*100, y*100, z*100);
}

void Gyro::report_incremental() {
     // Read gyro values.
     
    Axis3f imu_gyro = Board::get_imu_data()->gyro;
    float multiplier = Board::get_sens_mouse_values();
    float x = imu_gyro.x  * multiplier ;
    float y = imu_gyro.y  * multiplier ;
    float z = imu_gyro.z  * multiplier ;
    // x = x/(2-x);//取消非线性
    // y = y/(2-y);
    // z = z/(2-z);
    // Report.
    if (x >= 0) gyro_incremental_output( x, gyro.actions_x_pos);
    else        gyro_incremental_output(-x, gyro.actions_x_neg);
    if (y >= 0) gyro_incremental_output( y, gyro.actions_y_pos);
    else        gyro_incremental_output(-y, gyro.actions_y_neg);
    if (z >= 0) gyro_incremental_output( z, gyro.actions_z_pos);
    else        gyro_incremental_output(-z, gyro.actions_z_neg);
}

bool Gyro::is_engaged() {
    if (gyro.engage == BUTTON_NONE) return false;
    if (gyro.engage == BUTTON_TOUCH_IN) return Board::GetInstance().touch_->data.touch_status;
    return Button::is_pressed(&gyro.engage_button);
}

void Gyro::report() {
    switch (gyro.mode)
    {
    case GYRO_MODE_TOUCH_ON:
        if (is_engaged()) report_incremental();
        break;
    case GYRO_MODE_TOUCH_OFF:
        if (!is_engaged()) report_incremental();
        break;
    case GYRO_MODE_ALWAYS_ON:
        report_incremental();
        break;
    case GYRO_MODE_AXIS_ABSOLUTE:
        report_absolute();
        break;
    case GYRO_MODE_OFF:
    default:
        break;
    }
}

void Gyro::reset() {
    gyro.pressed_x_pos = false;
    gyro.pressed_y_pos = false;
    gyro.pressed_z_pos = false;
    gyro.pressed_x_neg = false;
    gyro.pressed_y_neg = false;
    gyro.pressed_z_neg = false;
}

void Gyro::config_x(float min, float max, Actions neg, Actions pos) {
    gyro.absolute_x_min = min;
    gyro.absolute_x_max = max;
    memcpy(gyro.actions_x_neg, neg, ACTIONS_LEN);
    memcpy(gyro.actions_x_pos, pos, ACTIONS_LEN);
}

void Gyro::config_y( float min, float max, Actions neg, Actions pos) {
    gyro.absolute_y_min = min;
    gyro.absolute_y_max = max;
    memcpy(gyro.actions_y_neg, neg, ACTIONS_LEN);
    memcpy(gyro.actions_y_pos, pos, ACTIONS_LEN);
}

void Gyro::config_z( float min, float max, Actions neg, Actions pos) {
    gyro.absolute_z_min = min;
    gyro.absolute_z_max = max;
    memcpy(gyro.actions_z_neg, neg, ACTIONS_LEN);
    memcpy(gyro.actions_z_pos, pos, ACTIONS_LEN);
}

void Gyro::from_ctrl (
    GyroMode mode,
    uint8_t engage
) {
    gyro.mode = mode;
    gyro.engage = engage;
    if (engage != BUTTON_NONE && engage != BUTTON_TOUCH_IN) {
        Actions none = {0,};
        gyro.engage_button = Button_from_params(engage, NORMAL, none, none, none);
    }
    memset(gyro.actions_x_pos, 0, ACTIONS_LEN);
    memset(gyro.actions_y_pos, 0, ACTIONS_LEN);
    memset(gyro.actions_z_pos, 0, ACTIONS_LEN);
    memset(gyro.actions_x_neg, 0, ACTIONS_LEN);
    memset(gyro.actions_y_neg, 0, ACTIONS_LEN);
    memset(gyro.actions_z_neg, 0, ACTIONS_LEN);
    reset();
}
