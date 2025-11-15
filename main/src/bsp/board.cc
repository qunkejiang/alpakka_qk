#include "board.h"
#include "logging.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/i2c_master.h>



const gc9a01_lcd_init_cmd_t gc9d01n_lcd_init_cmds[] = {
    //  {cmd, { data }, data_size, delay_ms}
    // Enable Inter Register
    {0xFE, (uint8_t[]){0x00}, 0, 0},
    {0xEF, (uint8_t[]){0x00}, 0, 0},
    {0x80, (uint8_t[]){0xFF}, 1, 0},
    {0x81, (uint8_t[]){0xFF}, 1, 0},
    {0x82, (uint8_t[]){0xFF}, 1, 0},
    {0x84, (uint8_t[]){0xFF}, 1, 0},
    {0x85, (uint8_t[]){0xFF}, 1, 0},
    {0x86, (uint8_t[]){0xFF}, 1, 0},
    {0x87, (uint8_t[]){0xFF}, 1, 0},
    {0x88, (uint8_t[]){0xFF}, 1, 0},
    {0x89, (uint8_t[]){0xFF}, 1, 0},
    {0x8A, (uint8_t[]){0xFF}, 1, 0},
    {0x8B, (uint8_t[]){0xFF}, 1, 0},
    {0x8C, (uint8_t[]){0xFF}, 1, 0},
    {0x8D, (uint8_t[]){0xFF}, 1, 0},
    {0x8E, (uint8_t[]){0xFF}, 1, 0},
    {0x8F, (uint8_t[]){0xFF}, 1, 0},
    {0x3A, (uint8_t[]){0x05}, 1, 0},
    {0xEC, (uint8_t[]){0x01}, 1, 0},
    {0x74, (uint8_t[]){0x02, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00}, 7, 0},
    {0x98, (uint8_t[]){0x3E}, 1, 0},
    {0x99, (uint8_t[]){0x3E}, 1, 0},
    {0xB5, (uint8_t[]){0x0D, 0x0D}, 2, 0},
    {0x60, (uint8_t[]){0x38, 0x0F, 0x79, 0x67}, 4, 0},
    {0x61, (uint8_t[]){0x38, 0x11, 0x79, 0x67}, 4, 0},
    {0x64, (uint8_t[]){0x38, 0x17, 0x71, 0x5F, 0x79, 0x67}, 6, 0},
    {0x65, (uint8_t[]){0x38, 0x13, 0x71, 0x5B, 0x79, 0x67}, 6, 0},
    {0x6A, (uint8_t[]){0x00, 0x00}, 2, 0},
    {0x6C, (uint8_t[]){0x22, 0x02, 0x22, 0x02, 0x22, 0x22, 0x50}, 7, 0},
    {0x6E, (uint8_t[]){0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x0F, 0x0F, 0x0D, 0x0D, 0x0B, 0x0B, 0x09, 0x09, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x0A, 0x0C, 0x0C, 0x0E, 0x0E, 0x10, 0x10, 0x00, 0x00, 0x02, 0x02, 0x04, 0x04}, 32, 0},
    {0xBF, (uint8_t[]){0x01}, 1, 0},
    {0xF9, (uint8_t[]){0x40}, 1, 0},
    {0x9B, (uint8_t[]){0x3B, 0x93, 0x33, 0x7F, 0x00}, 5, 0},
    {0x7E, (uint8_t[]){0x30}, 1, 0},
    {0x70, (uint8_t[]){0x0D, 0x02, 0x08, 0x0D, 0x02, 0x08}, 6, 0},
    {0x71, (uint8_t[]){0x0D, 0x02, 0x08}, 3, 0},
    {0x91, (uint8_t[]){0x0E, 0x09}, 2, 0},
    {0xC3, (uint8_t[]){0x19, 0xC4, 0x19, 0xC9, 0x3C}, 5, 0},
    {0xF0, (uint8_t[]){0x53, 0x15, 0x0A, 0x04, 0x00, 0x3E}, 6, 0},
    {0xF1, (uint8_t[]){0x56, 0xA8, 0x7F, 0x33, 0x34, 0x5F}, 6, 0},
    {0xF2, (uint8_t[]){0x53, 0x15, 0x0A, 0x04, 0x00, 0x3A}, 6, 0},
    {0xF3, (uint8_t[]){0x52, 0xA4, 0x7F, 0x33, 0x34, 0xDF}, 6, 0},

    // {0x20, (uint8_t[]){0x00}, 0, 0},
    {0x36, (uint8_t[]){0x00}, 1, 0},
    {0x11, (uint8_t[]){0x00}, 0, 200},
    {0x29, (uint8_t[]){0x00}, 0, 0},
    {0x2C, (uint8_t[]){0x00}, 0, 20},
};
void Board::Init_LCD_Display() {

    logging::info( "Init GC9D01N\n");
    spi_bus_config_t buscfg;
    memset(&buscfg, 0, sizeof(buscfg));
    buscfg.mosi_io_num = DISPLAY_MOSI;
    buscfg.miso_io_num = GPIO_NUM_NC;
    buscfg.sclk_io_num = DISPLAY_SCLK;
    buscfg.quadwp_io_num = GPIO_NUM_NC;
    buscfg.quadhd_io_num = GPIO_NUM_NC;
    buscfg.max_transfer_sz = DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(uint32_t);
    ESP_ERROR_CHECK(spi_bus_initialize(DISPLAY_SPI, &buscfg, SPI_DMA_CH_AUTO));//SPI_DMA_DISABLED 

    esp_lcd_panel_io_handle_t panel_io = nullptr;
    esp_lcd_panel_handle_t panel = nullptr;

    esp_lcd_panel_io_spi_config_t io_config;
    memset(&io_config, 0, sizeof(io_config));
    io_config.cs_gpio_num = DISPLAY_CS;
    io_config.dc_gpio_num = DISPLAY_DC;
    io_config.spi_mode = 0;
    io_config.pclk_hz = 40 * 1000 * 1000;
    io_config.trans_queue_depth = 10;
    io_config.lcd_cmd_bits = 8;
    io_config.lcd_param_bits = 8;
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi(DISPLAY_SPI, &io_config, &panel_io));

    gc9a01_vendor_config_t vendor_config = {
        .init_cmds = gc9d01n_lcd_init_cmds,
        .init_cmds_size = sizeof(gc9d01n_lcd_init_cmds) / sizeof(gc9a01_lcd_init_cmd_t),
    };
    esp_lcd_panel_dev_config_t panel_config = {};
    panel_config.reset_gpio_num = DISPLAY_RST;
    panel_config.rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB;
    panel_config.bits_per_pixel = 16;
    panel_config.vendor_config = &vendor_config;
    ESP_ERROR_CHECK(esp_lcd_new_panel_gc9a01(panel_io, &panel_config, &panel));
    esp_lcd_panel_reset(panel);
    esp_lcd_panel_init(panel);
    esp_lcd_panel_invert_color(panel, false);
    esp_lcd_panel_swap_xy(panel, DISPLAY_SWAP_XY);
    esp_lcd_panel_mirror(panel, DISPLAY_MIRROR_X, DISPLAY_MIRROR_Y);
 
    display_ = new SpiLcdDisplay(panel_io, panel,
                                DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_OFFSET_X, DISPLAY_OFFSET_Y, DISPLAY_MIRROR_X,
                                DISPLAY_MIRROR_Y, DISPLAY_SWAP_XY
                            );
}

void Board::Init_Backlight() 
{
    backlight_ = new PwmBacklight(DISPLAY_BACKLIGHT_PIN, DISPLAY_BACKLIGHT_OUTPUT_INVERT,DISPLAY_BACKLIGHT_TIMER,DISPLAY_BACKLIGHT_CHANNEL);
    backlight_->SetBrightnessImpl(50+nvm_->nvm_data.backlight*40);
}
void Board::Init_NVM()
{
    nvm_ = new NVM();
}


void Board::Init_Axp2101() 
{
    i2c_master_bus_config_t i2c_bus_cfg = {
        .i2c_port = (i2c_port_t)AXP_I2C,
        .sda_io_num = AXP_I2C_SDA_PIN,
        .scl_io_num = AXP_I2C_SCL_PIN,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .intr_priority = 0,
        .trans_queue_depth = 0,
        .flags = {
            .enable_internal_pullup = 1,
            .allow_pd = 0, // Allow power down
        },
    };
    axp2101_ = new Axp2101(i2c_bus_cfg, AXP2101_I2C_ADDR, AXP_I2C_INT_PIN);
}
void Board::Init_Joystick()
{
    adc_channel_t channel[4] = { JOYSTICK_CHANNEL_0,JOYSTICK_CHANNEL_1, JOYSTICK_CHANNEL_2, JOYSTICK_CHANNEL_3};
    joystick_ = new joystick(channel, sizeof(channel) / sizeof(adc_channel_t));
}
void Board::Init_Matrix_Keyboard() 
{
    matrix_kbd_config_t config = {
        .row_gpios = MATRIX_KEYBOARD_ROWS, // GPIOs for rows
        .col_gpios = MATRIX_KEYBOARD_COLS, // GPIOs for columns
        .nr_row_gpios = MATRIX_KEYBOARD_ROWS_NUM, // Number of row GPIOs
        .nr_col_gpios = MATRIX_KEYBOARD_COLS_NUM, // Number of column GPIOs
        .debounce_us = 250, // Debounce time in microseconds
    };
    keyboard_ = new matrix_kbd(&config);
}
void Board::Init_Touch() {
    touch_lowlevel_config_t config = {
        .channel_num = 1,
        .channel_list = (uint32_t[]){TOUCH},
        .channel_type = NULL,  // Use default TOUCH type
        .sample_period_ms = 10, // Use platform defaults
        .proximity_count = 0, // No proximity measurement
    };
    touch_ = new Touch(&config);
}
void Board::Init_Pcnt() 
{
    pcnt_ = new Pcnt(EC_P, EC_N);
}
void Board::Init_IMU() 
{
    imu_config_t config ={ 
        .timer_num = ICM_TIMER, // LEDC timer number
        .channel = ICM_TIMER_CHANNEL0, // LEDC channel number
        .host = ICM_SPI, // SPI host
        .mosi_io_num = ICM_SDI, // MOSI pin
        .miso_io_num = ICM_SDO, // MISO pin
        .sclk_io_num = ICM_SCK, // SCLK pin
        .spics_io_num = ICM_CS0, // CS0 pin
        .sync_io_num = ICM_INT0, // SYNC pin 0
    };
    imu_ = new Imu(&config);
}
void Board::Init_Haptic() {
    haptic_config_t config = {
        .gpio_num_left = HAPTIC_MOTOR_L, // GPIO pin for motor 1
        .gpio_num_right = HAPTIC_MOTOR_R, // GPIO pin for motor 2
        .ledc_channel_left = HAPTIC_TIMER_CHANNEL_L, // LEDC channel number for motor 1
        .ledc_channel_right = HAPTIC_TIMER_CHANNEL_R, // LEDC channel number for motor 2
        .timer_num = HAPTIC_TIMER, // LEDC timer number
    };

    haptic_ = new Haptic_Motor(&config);
}
void Board::Init_USB()
{
    usb_ = new USB();
}
void Board::Init_CPU()
{
    cpu_ = new CPU();
}
void Board::Init_hid()
{
    hid_ = new hid();
}
void Board::Init_profile()
{
    profile_ = new profile(Board::get_profile(nvm_->nvm_data.profile_index));
}


void Board::Init() {
    Init_Matrix_Keyboard();//矩阵键盘
    do
    {
        vTaskDelay(pdMS_TO_TICKS(100));
    } while ( keyboard_->key_value[BUTTON_START_2>>4]&(1<<(BUTTON_START_2&0xf)));
    Init_NVM();//数据保存
    Init_USB();//USB
    Init_hid();//HID
    Init_Axp2101();//电源控制
    Init_LCD_Display();//屏幕
    Init_Backlight();//背光
    Init_Joystick();//摇杆
    Init_Touch();//触摸按键
    Init_Pcnt();//旋转编码器
    Init_IMU();//IMU
    Init_Haptic();//震动
    Init_CPU();//CPU任务
    // // Init_Bluetooth();//蓝牙
    Init_profile();//配置文件
    
    logging::info("Board Init Finish\n");
}




// W (30707) board: INTERNAL RAM left 387636B/403536B 15900B,min:382936B
// W (30713) board: SPI      RAM left 2093928B/2097152B 3224B,min:2093928B
// W (20846) CPU: INTERNAL RAM left 298479B/428087B 129608B,min:293895B
// W (20847) CPU: SPI      RAM left 1995432B/2097152B 101720B,min:1993952B
// W (20839) CPU: INTERNAL RAM left 302779B/428087B 125308B,min:302375B
// W (20840) CPU: SPI      RAM left 1992828B/2097152B 104324B,min:1992828B


