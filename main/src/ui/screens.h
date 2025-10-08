#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <esp_lvgl_port.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _groups_t {
    lv_group_t *keyboard_groups;
} groups_t;

extern groups_t groups;

void ui_create_groups();

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *generic_page;
    lv_obj_t *generic_kb_page;
    lv_obj_t *menu_page;
    lv_obj_t *menu1_page;
    lv_obj_t *menu2_page;
    lv_obj_t *menu3_page;
    lv_obj_t *menu4_page;
    lv_obj_t *menu5_page;
    lv_obj_t *menu6_page;
    lv_obj_t *menu7_page;
    lv_obj_t *obj0;
    lv_obj_t *obj0__obj0;
    lv_obj_t *obj0__obj1;
    lv_obj_t *obj0__obj2;
    lv_obj_t *obj0__obj3;
    lv_obj_t *obj0__obj4;
    lv_obj_t *obj1;
    lv_obj_t *obj1__obj0;
    lv_obj_t *obj1__obj1;
    lv_obj_t *obj1__obj2;
    lv_obj_t *obj1__obj3;
    lv_obj_t *obj1__obj4;
    lv_obj_t *obj2;
    lv_obj_t *obj3;
    lv_obj_t *obj4;
    lv_obj_t *obj5;
    lv_obj_t *backlight;
    lv_obj_t *obj6;
    lv_obj_t *obj7;
    lv_obj_t *obj8;
    lv_obj_t *obj9;
    lv_obj_t *menu5_label;
    lv_obj_t *menu6_label;
    lv_obj_t *menu7_sv;
    lv_obj_t *keymouse_button1;
    lv_obj_t *keymouse_button2;
    lv_obj_t *keymouse_button3;
    lv_obj_t *keymouse_button4;
    lv_obj_t *keymouse_button5;
    lv_obj_t *keymouse_button6;
    lv_obj_t *obj10;
    lv_obj_t *obj11;
    lv_obj_t *obj12;
    lv_obj_t *obj13;
    lv_obj_t *obj14;
    lv_obj_t *obj15;
    lv_obj_t *menu1_roller;
    lv_obj_t *language;
    lv_obj_t *menu3_roller;
    lv_obj_t *obj16;
    lv_obj_t *obj17;
    lv_obj_t *obj18;
    lv_obj_t *obj19;
    lv_obj_t *obj20;
    lv_obj_t *obj21;
    lv_obj_t *obj22;
    lv_obj_t *obj23;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_GENERIC_PAGE = 2,
    SCREEN_ID_GENERIC_KB_PAGE = 3,
    SCREEN_ID_MENU_PAGE = 4,
    SCREEN_ID_MENU1_PAGE = 5,
    SCREEN_ID_MENU2_PAGE = 6,
    SCREEN_ID_MENU3_PAGE = 7,
    SCREEN_ID_MENU4_PAGE = 8,
    SCREEN_ID_MENU5_PAGE = 9,
    SCREEN_ID_MENU6_PAGE = 10,
    SCREEN_ID_MENU7_PAGE = 11,
};

void create_screen_main();
void tick_screen_main();

void create_screen_generic_page();
void tick_screen_generic_page();

void create_screen_generic_kb_page();
void tick_screen_generic_kb_page();

void create_screen_menu_page();
void tick_screen_menu_page();

void create_screen_menu1_page();
void tick_screen_menu1_page();

void create_screen_menu2_page();
void tick_screen_menu2_page();

void create_screen_menu3_page();
void tick_screen_menu3_page();

void create_screen_menu4_page();
void tick_screen_menu4_page();

void create_screen_menu5_page();
void tick_screen_menu5_page();

void create_screen_menu6_page();
void tick_screen_menu6_page();

void create_screen_menu7_page();
void tick_screen_menu7_page();

void create_user_widget_status_bar(lv_obj_t *parent_obj, void *flowState, int startWidgetIndex);
void tick_user_widget_status_bar(void *flowState, int startWidgetIndex);

enum Themes {
    THEME_ID_LIGHT,
    THEME_ID_DARK,
};
enum Colors {
    COLOR_ID_BUTTON_COLOR,
    COLOR_ID_BACK_COLOR,
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