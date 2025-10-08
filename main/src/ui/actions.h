#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <esp_lvgl_port.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void action_load_options(lv_event_t * e);
extern void action_save_options(lv_event_t * e);
extern void action_backlight_action(lv_event_t * e);
extern void action_joystick_calibration_setp1(lv_event_t * e);
extern void action_joystick_calibration_setp3(lv_event_t * e);
extern void action_touch_calibration1(lv_event_t * e);
extern void action_touch_calibration2(lv_event_t * e);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/