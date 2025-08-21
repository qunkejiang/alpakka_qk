#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <esp_lvgl_port.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _groups_t {
    lv_group_t *keyboard;
} groups_t;

extern groups_t groups;

void ui_create_groups();

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *gamepad_page;
    lv_obj_t *qh_1;
    lv_obj_t *qh_2;
    lv_obj_t *set_page;
    lv_obj_t *aa;
    lv_obj_t *obj0;
    lv_obj_t *obj0__obj9;
    lv_obj_t *obj0__obj10;
    lv_obj_t *obj0__obj11;
    lv_obj_t *obj0__obj12;
    lv_obj_t *obj0__obj13;
    lv_obj_t *obj0__obj14;
    lv_obj_t *obj0__obj15;
    lv_obj_t *obj0__obj16;
    lv_obj_t *obj0__obj17;
    lv_obj_t *obj0__obj18;
    lv_obj_t *obj0__obj19;
    lv_obj_t *obj0__obj20;
    lv_obj_t *obj1;
    lv_obj_t *obj1__obj0;
    lv_obj_t *obj1__obj1;
    lv_obj_t *obj1__obj2;
    lv_obj_t *obj1__obj3;
    lv_obj_t *obj1__obj4;
    lv_obj_t *obj1__obj5;
    lv_obj_t *obj1__obj6;
    lv_obj_t *obj1__obj7;
    lv_obj_t *obj1__obj8;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_GAMEPAD_PAGE = 2,
    SCREEN_ID_QH_1 = 3,
    SCREEN_ID_QH_2 = 4,
    SCREEN_ID_SET_PAGE = 5,
    SCREEN_ID_AA = 6,
};

void create_screen_main();
void tick_screen_main();

void create_screen_gamepad_page();
void tick_screen_gamepad_page();

void create_screen_qh_1();
void tick_screen_qh_1();

void create_screen_qh_2();
void tick_screen_qh_2();

void create_screen_set_page();
void tick_screen_set_page();

void create_screen_aa();
void tick_screen_aa();

void create_user_widget_keyboard1(lv_obj_t *parent_obj, void *flowState, int startWidgetIndex);
void tick_user_widget_keyboard1(void *flowState, int startWidgetIndex);

void create_user_widget_keyboard2(lv_obj_t *parent_obj, void *flowState, int startWidgetIndex);
void tick_user_widget_keyboard2(void *flowState, int startWidgetIndex);

enum Themes {
    THEME_ID_LIGHT,
    THEME_ID_DARK,
};
enum Colors {
    COLOR_ID_BUTTON_COLOR,
    COLOR_ID_PAGE,
};
void change_color_theme(uint32_t themeIndex);
extern uint32_t theme_colors[2][2];

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/