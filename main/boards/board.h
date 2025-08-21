#ifndef BOARD_H
#define BOARD_H

#include <string>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
// #include "backlight.h"

#include "nvm.h"
#include <driver/i2c_master.h>
#include "axp2101.h"
#include <esp_lcd_gc9a01.h>
#include "lcd_display.h"
#include "backlight.h"
#include "config.h"
#include "joystick.h"
#include "matrix_keyboard.h"
#include "touch.h"
#include "rotary.h"
#include "imu.h"
#include "Haptic_Motor.h"
#include "cpu.h"
#include "usb.h"



class Board {

private:
    Board();
    void Init_NVM();
    void Init_Axp2101();
    void Init_LCD_Display();
    void Init_Backlight();
    void Init_Joystick(); 
    void Init_Matrix_Keyboard();
    void Init_Touch();
    void Init_Rotary();
    void Init_IMU();
    void Init_Haptic();
    void Init_CPU();
    void Init_USB();
public:;
    NVM* nvm_ = nullptr;
    Axp2101* axp2101_ = nullptr;
    LcdDisplay* display_= nullptr;
    PwmBacklight* backlight_ = nullptr;
    joystick* joystick_ = nullptr;
    matrix_kbd* keyboard_ = nullptr;
    Touch* touch_ = nullptr;
    Rotary* rotary_ = nullptr;
    Imu* imu_ = nullptr;
    Haptic_Motor* haptic_ = nullptr;
    CPU* cpu_ = nullptr;
    USB* usb_ = nullptr;
    
    static Board& GetInstance() {
        static Board instance;
        return instance;
    }
};

#endif // BOARD_H
