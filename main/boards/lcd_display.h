#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H


#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_ops.h>

#include <lvgl.h>
#include <esp_timer.h>
#include <esp_log.h>
#include <esp_pm.h>

class LcdDisplay  {
protected:
    esp_lcd_panel_io_handle_t panel_io_ = nullptr;
    esp_lcd_panel_handle_t panel_ = nullptr;
    
    static void Lcd_ui_tick(void *pvParameters);
    void SetupUI();
    bool Lock(int timeout_ms = 0) ;// virtual override
    void Unlock() ;// virtual override

protected:
    int width_ = 0;
    int height_ = 0;

    lv_display_t *display_ = nullptr;
    uint8_t *emotion_buf;
    lv_obj_t *emotion_label_ = nullptr;

    // 添加protected构造函数
    LcdDisplay(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_handle_t panel)//, DisplayFonts fonts
        : panel_io_(panel_io), panel_(panel) {}//, fonts_(fonts)
};


// // SPI LCD显示器
class SpiLcdDisplay : public LcdDisplay {
public:
    SpiLcdDisplay(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_handle_t panel,
                  int width, int height, int offset_x, int offset_y,
                  bool mirror_x, bool mirror_y, bool swap_xy
//                   ,DisplayFonts fonts
    );
};
#endif // LCD_DISPLAY_H
