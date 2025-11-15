#pragma once 

#include <string.h>



#include "board_config.h"
#include "nvm.h"
#include "axp2101.h"
#include <esp_lcd_gc9a01.h>
#include "lcd_display.h"
#include "backlight.h"
#include "joystick.h"
#include "matrix_keyboard.h"
#include "touch.h"
#include "pcnt.h"
#include "imu.h"
#include "Haptic_Motor.h"
#include "cpu.h"
#include "usb.h"
#include "profile.h"
#include "hid.h"
#include "common.h"
#include "ui_handle.h"
#include "logging.h"





class Board {

private:
    void Init_NVM();
    void Init_Axp2101();
    void Init_LCD_Display();
    void Init_Backlight();
    void Init_Joystick(); 
    void Init_Matrix_Keyboard();
    void Init_Touch();
    void Init_Pcnt();
    void Init_IMU();
    void Init_Haptic();
    void Init_CPU();
    void Init_USB();
    void Init_profile();
    void Init_hid();
    
public:
    NVM* nvm_ = nullptr;
    Axp2101* axp2101_ = nullptr;
    LcdDisplay* display_= nullptr;
    PwmBacklight* backlight_ = nullptr;
    joystick* joystick_ = nullptr;
    matrix_kbd* keyboard_ = nullptr;
    Touch* touch_ = nullptr;
    Pcnt* pcnt_ = nullptr;
    Imu* imu_ = nullptr;
    Haptic_Motor* haptic_ = nullptr;
    CPU* cpu_ = nullptr;
    profile * profile_ =nullptr;
    USB* usb_ = nullptr;
    hid* hid_ = nullptr;
    
    static bool get_key_value(uint8_t key){
        return (Board::GetInstance().keyboard_->key_value[key>>4]&(1<<(key&0xf)))!=0;
    }
    static Imu_Data_t* get_imu_data(){
        return &Board::GetInstance().imu_->data;
    }
    static axp2101_data_t get_axp2101(){
        return Board::GetInstance().axp2101_->data;
    }
    static Config_t* get_nvm_data(){
        return &Board::GetInstance().nvm_->nvm_data;
    }
    static uint8_t get_protocol() {
        return Board::GetInstance().nvm_->nvm_data.usb_protocol;
    }
    static float get_sens_mouse_values() {
        Config_t* nvm = &Board::GetInstance().nvm_->nvm_data;
        return nvm->sens_mouse_values[nvm->sens_mouse];
    }
    static CtrlProfile* get_profile(uint8_t index) {
        return &Board::GetInstance().nvm_->config_profiles[index];
    }
    static void set_profile(uint8_t profile_index) {
        logging::info("Switching to profile %d\n", profile_index);
        Board::GetInstance().profile_->load_from_config(Board::get_profile(profile_index));
        if(profile_index!=PROFILE_HOME)
        {
            Board::GetInstance().nvm_->nvm_data.profile_index = profile_index;
            
        }
    }
    void Init();
    Board() {}
    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;
    static Board& GetInstance() {
        static Board instance;
        return instance;
    }
};
