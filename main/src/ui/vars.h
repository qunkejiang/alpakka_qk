#ifndef EEZ_LVGL_UI_VARS_H
#define EEZ_LVGL_UI_VARS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// enum declarations



// Flow global variables

enum FlowGlobalVariables {
    FLOW_GLOBAL_VARIABLE_NONE
};

// Native global variables

extern int32_t imu_calibration;
extern bool get_var_bt_state();
extern void set_var_bt_state(bool value);
extern bool get_var_usb_state();
extern void set_var_usb_state(bool value);
extern int32_t get_var_battery_state();
extern void set_var_battery_state(int32_t value);
extern int32_t get_var_mode_state();
extern void set_var_mode_state(int32_t value);
extern bool get_var_keymouse_aa();
extern void set_var_keymouse_aa(bool value);
extern int32_t get_var_keymouse_zu();
extern void set_var_keymouse_zu(int32_t value);
extern int32_t get_var_joystick_calibration1();
extern void set_var_joystick_calibration1(int32_t value);
extern int32_t get_var_joystick_calibration2();
extern void set_var_joystick_calibration2(int32_t value);
extern int32_t get_var_joystick_calibration3();
extern void set_var_joystick_calibration3(int32_t value);
extern int32_t get_var_joystick_calibration4();
extern void set_var_joystick_calibration4(int32_t value);
extern int32_t get_var_imu_calibration();
extern void set_var_imu_calibration(int32_t value);
extern int32_t get_var_touch_calibration_pv();
extern void set_var_touch_calibration_pv(int32_t value);
extern bool get_var_touch_calibration_led();
extern void set_var_touch_calibration_led(bool value);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/