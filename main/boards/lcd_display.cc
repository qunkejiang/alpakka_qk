#include "lcd_display.h"

#include <vector>
#include <esp_lvgl_port.h>

#include "board.h"
#include "ui.h"

#define TAG "LcdDisplay"

SpiLcdDisplay::SpiLcdDisplay(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_handle_t panel,
                           int width, int height, int offset_x, int offset_y, bool mirror_x, bool mirror_y, bool swap_xy
                           //,
                           //DisplayFonts fonts
                           )
    : LcdDisplay(panel_io, panel) //, fonts
{
    width_ = width;
    height_ = height;

    // Set the display to on
    ESP_LOGI(TAG, "Turning display on");
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_, true));

    ESP_LOGI(TAG, "Initialize LVGL library");
    lv_init();

    ESP_LOGI(TAG, "Initialize LVGL port");
    lvgl_port_cfg_t port_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    port_cfg.task_priority = 1;
    port_cfg.timer_period_ms = 50;
    // port_cfg.task_stack = 40*1024;
    lvgl_port_init(&port_cfg);
    
    ESP_LOGI(TAG, "Adding LCD screen");
    const lvgl_port_display_cfg_t display_cfg = {
        .io_handle = panel_io_,
        .panel_handle = panel_,
        .control_handle = nullptr,
        .buffer_size = static_cast<uint32_t>(width_ * 10),
        .double_buffer = true,
        .trans_size = 0,
        .hres = static_cast<uint32_t>(width_),
        .vres = static_cast<uint32_t>(height_),
        .monochrome = false,
        .rotation = {
            .swap_xy = swap_xy,
            .mirror_x = mirror_x,
            .mirror_y = mirror_y,
        },
        .color_format = LV_COLOR_FORMAT_RGB565,
        .flags = {
            .buff_dma = 1,
            .buff_spiram = 1,
            .sw_rotate = 0,
            .swap_bytes = 1,
            .full_refresh = 0,
            .direct_mode = 0,
        },
    };

    display_ = lvgl_port_add_disp(&display_cfg);
    if (display_ == nullptr) {
        ESP_LOGE(TAG, "Failed to add display");
        return;
    }

    if (offset_x != 0 || offset_y != 0) {
        lv_display_set_offset(display_, offset_x, offset_y);
    }

    SetupUI();
}

void LcdDisplay::SetupUI() {
    if (!Lock(30000)) {
        ESP_LOGE("Display", "Failed to lock display");
    }
    ui_init();
    
    Unlock();
}

bool LcdDisplay::Lock(int timeout_ms) {
    return lvgl_port_lock(timeout_ms);
}

void LcdDisplay::Unlock() {
    lvgl_port_unlock();
}

void lv_mem_init(void)
{
    return; /*Nothing to init*/
}
void lv_mem_deinit(void)
{
    return; /*Nothing to deinit*/
}


void lv_mem_monitor_core(lv_mem_monitor_t * mon_p)
{
    /*Not supported*/
    LV_UNUSED(mon_p);
    return;
}
void * lv_malloc_core(size_t size)
{
    return heap_caps_malloc(size , MALLOC_CAP_SPIRAM);
}
void * lv_realloc_core(void * p, size_t new_size)
{
    return heap_caps_realloc(p, new_size,MALLOC_CAP_SPIRAM);
}
void lv_free_core(void * p)
{
    heap_caps_free(p);
}




