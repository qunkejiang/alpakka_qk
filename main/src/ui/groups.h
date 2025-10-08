#ifndef EEZ_GROUPS_H
#define EEZ_GROUPS_H

#include <esp_lvgl_port.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct  {
    uint8_t key;
    uint8_t state;
}group_msg_t;

// Style: s1
lv_style_t *get_style_s1_MAIN_DEFAULT();
void add_style_s1(lv_obj_t *obj);
void remove_style_s1(lv_obj_t *obj);


void groups_init(void);
void groups_send_key(group_msg_t report);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_STYLES_H*/