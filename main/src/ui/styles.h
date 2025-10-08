#ifndef EEZ_LVGL_UI_STYLES_H
#define EEZ_LVGL_UI_STYLES_H

#include <esp_lvgl_port.h>

#ifdef __cplusplus
extern "C" {
#endif

// Style: s1
lv_style_t *get_style_s1_MAIN_DEFAULT();
void add_style_s1(lv_obj_t *obj);
void remove_style_s1(lv_obj_t *obj);



#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_STYLES_H*/