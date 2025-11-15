// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2022, Input Labs Oy.
#pragma once

#include "ctrl.h"
#include "button.h"
#include "imu.h"


typedef enum GyroMode_enum {
    GYRO_MODE_OFF,
    GYRO_MODE_ALWAYS_ON,
    GYRO_MODE_TOUCH_OFF,
    GYRO_MODE_TOUCH_ON,
    GYRO_MODE_AXIS_ABSOLUTE,
} GyroMode;

typedef struct Gyro_struct Gyro_t;
struct Gyro_struct {
    GyroMode mode;
    uint8_t engage;
    Button_t engage_button;
    float absolute_x_min;
    float absolute_y_min;
    float absolute_z_min;
    float absolute_x_max;
    float absolute_y_max;
    float absolute_z_max;
    bool pressed_x_pos;
    bool pressed_y_pos;
    bool pressed_z_pos;
    bool pressed_x_neg;
    bool pressed_y_neg;
    bool pressed_z_neg;
    Actions actions_x_pos;
    Actions actions_y_pos;
    Actions actions_z_pos;
    Actions actions_x_neg;
    Actions actions_y_neg;
    Actions actions_z_neg;
};

// void gyro_update_sensitivity();
class Gyro : public Button {
private:

    Gyro_t gyro;
    bool is_engaged();
    void report_incremental();
    void report_absolute();
    void gyro_incremental_output(float value, uint8_t *actions);
    void gyro_update_sensitivity();

public:
    void reset();
    void config_x(float min, float max, Actions neg, Actions pos);
    void config_y(float min, float max, Actions neg, Actions pos);
    void config_z(float min, float max, Actions neg, Actions pos);
    void from_ctrl(GyroMode mode, uint8_t engage);
    void report();
};