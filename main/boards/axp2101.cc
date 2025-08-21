#include "axp2101.h"
#include "board.h"

#include <esp_log.h>
#include "driver/gpio.h"

#define TAG "Axp2101"


// #define XPOWERS_AXP2101_STATUS1                          (0x00)
#define AXP2101_STATUS2                          (0x01)
// #define XPOWERS_AXP2101_IC_TYPE                          (0x03)


// #define XPOWERS_AXP2101_DATA_BUFFER1                     (0x04)
// #define XPOWERS_AXP2101_DATA_BUFFER2                     (0x05)
// #define XPOWERS_AXP2101_DATA_BUFFER3                     (0x06)
// #define XPOWERS_AXP2101_DATA_BUFFER4                     (0x07)
// #define XPOWERS_AXP2101_DATA_BUFFER_SIZE                 (4u)

#define AXP2101_COMMON_CONFIG                    (0x10)
// #define XPOWERS_AXP2101_BATFET_CTRL                      (0x12)
// #define XPOWERS_AXP2101_DIE_TEMP_CTRL                    (0x13)
#define AXP2101_MIN_SYS_VOL_CTRL                 (0x14)
#define AXP2101_INPUT_VOL_LIMIT_CTRL             (0x15)
#define AXP2101_INPUT_CUR_LIMIT_CTRL             (0x16)
// #define XPOWERS_AXP2101_RESET_FUEL_GAUGE                 (0x17)
// #define XPOWERS_AXP2101_CHARGE_GAUGE_WDT_CTRL            (0x18)


// #define XPOWERS_AXP2101_WDT_CTRL                         (0x19)
// #define XPOWERS_AXP2101_LOW_BAT_WARN_SET                 (0x1A)


// #define XPOWERS_AXP2101_PWRON_STATUS                     (0x20)
// #define XPOWERS_AXP2101_PWROFF_STATUS                    (0x21)
#define AXP2101_PWROFF_EN                        (0x22)
// #define XPOWERS_AXP2101_DC_OVP_UVP_CTRL                  (0x23)
#define AXP2101_VOFF_SET                         (0x24)
// #define XPOWERS_AXP2101_PWROK_SEQU_CTRL                  (0x25)
// #define XPOWERS_AXP2101_SLEEP_WAKEUP_CTRL                (0x26)
#define AXP2101_IRQ_OFF_ON_LEVEL_CTRL            (0x27)

// #define XPOWERS_AXP2101_FAST_PWRON_SET0                  (0x28)
// #define XPOWERS_AXP2101_FAST_PWRON_SET1                  (0x29)
// #define XPOWERS_AXP2101_FAST_PWRON_SET2                  (0x2A)
// #define XPOWERS_AXP2101_FAST_PWRON_CTRL                  (0x2B)

// #define XPOWERS_AXP2101_ADC_CHANNEL_CTRL                 (0x30)
#define AXP2101_ADC_DATA_RELUST0                 (0x34)
#define AXP2101_ADC_DATA_RELUST1                 (0x35)
// #define XPOWERS_AXP2101_ADC_DATA_RELUST2                 (0x36)
// #define XPOWERS_AXP2101_ADC_DATA_RELUST3                 (0x37)
// #define XPOWERS_AXP2101_ADC_DATA_RELUST4                 (0x38)
// #define XPOWERS_AXP2101_ADC_DATA_RELUST5                 (0x39)
// #define XPOWERS_AXP2101_ADC_DATA_RELUST6                 (0x3A)
// #define XPOWERS_AXP2101_ADC_DATA_RELUST7                 (0x3B)
// #define XPOWERS_AXP2101_ADC_DATA_RELUST8                 (0x3C)
// #define XPOWERS_AXP2101_ADC_DATA_RELUST9                 (0x3D)


// //XPOWERS INTERRUPT REGISTER
// #define XPOWERS_AXP2101_INTEN1                           (0x40)
#define AXP2101_INTEN2                           (0x41)
// #define XPOWERS_AXP2101_INTEN3                           (0x42)


// //XPOWERS INTERRUPT STATUS REGISTER
#define AXP2101_INTSTS1                          (0x48)
#define AXP2101_INTSTS2                          (0x49)
#define AXP2101_INTSTS3                          (0x4A)
// #define XPOWERS_AXP2101_INTSTS_CNT                       (3)

#define AXP2101_TS_PIN_CTRL                      (0x50)
// #define XPOWERS_AXP2101_TS_HYSL2H_SET                    (0x52)
// #define XPOWERS_AXP2101_TS_LYSL2H_SET                    (0x53)


// #define XPOWERS_AXP2101_VLTF_CHG_SET                     (0x54)
// #define XPOWERS_AXP2101_VHLTF_CHG_SET                    (0x55)
// #define XPOWERS_AXP2101_VLTF_WORK_SET                    (0x56)
// #define XPOWERS_AXP2101_VHLTF_WORK_SET                   (0x57)


// #define XPOWERS_AXP2101_JIETA_EN_CTRL                    (0x58)
// #define XPOWERS_AXP2101_JIETA_SET0                       (0x59)
// #define XPOWERS_AXP2101_JIETA_SET1                       (0x5A)
// #define XPOWERS_AXP2101_JIETA_SET2                       (0x5B)


#define AXP2101_IPRECHG_SET                      (0x61)
#define AXP2101_ICC_CHG_SET                      (0x62)
#define AXP2101_ITERM_CHG_SET_CTRL               (0x63)

#define AXP2101_CV_CHG_VOL_SET                   (0x64)

// #define XPOWERS_AXP2101_THE_REGU_THRES_SET               (0x65)
// #define XPOWERS_AXP2101_CHG_TIMEOUT_SET_CTRL             (0x67)

// #define XPOWERS_AXP2101_BAT_DET_CTRL                     (0x68)
// #define XPOWERS_AXP2101_CHGLED_SET_CTRL                  (0x69)

// #define XPOWERS_AXP2101_BTN_VOL_MIN                      (2600)
// #define XPOWERS_AXP2101_BTN_VOL_MAX                      (3300)
// #define XPOWERS_AXP2101_BTN_VOL_STEPS                    (100)


// #define XPOWERS_AXP2101_BTN_BAT_CHG_VOL_SET              (0x6A)


#define AXP2101_DC_ONOFF_DVM_CTRL                (0x80)
// #define XPOWERS_AXP2101_DC_FORCE_PWM_CTRL                (0x81)
#define AXP2101_DC_VOL0_CTRL                     (0x82)
// #define XPOWERS_AXP2101_DC_VOL1_CTRL                     (0x83)
// #define XPOWERS_AXP2101_DC_VOL2_CTRL                     (0x84)
// #define XPOWERS_AXP2101_DC_VOL3_CTRL                     (0x85)
// #define XPOWERS_AXP2101_DC_VOL4_CTRL                     (0x86)


#define AXP2101_LDO_ONOFF_CTRL0                  (0x90)
// #define XPOWERS_AXP2101_LDO_ONOFF_CTRL1                  (0x91)
#define AXP2101_LDO_VOL0_CTRL                    (0x92)
#define AXP2101_LDO_VOL1_CTRL                    (0x93)
// #define XPOWERS_AXP2101_LDO_VOL2_CTRL                    (0x94)
// #define XPOWERS_AXP2101_LDO_VOL3_CTRL                    (0x95)
// #define XPOWERS_AXP2101_LDO_VOL4_CTRL                    (0x96)
// #define XPOWERS_AXP2101_LDO_VOL5_CTRL                    (0x97)
// #define XPOWERS_AXP2101_LDO_VOL6_CTRL                    (0x98)
// #define XPOWERS_AXP2101_LDO_VOL7_CTRL                    (0x99)
// #define XPOWERS_AXP2101_LDO_VOL8_CTRL                    (0x9A)


// #define XPOWERS_AXP2101_BAT_PARAME                       (0xA1)
// #define XPOWERS_AXP2101_FUEL_GAUGE_CTRL                  (0xA2)
#define AXP2101_BAT_PERCENT_DATA                 (0xA4)


enum BatteryCurrentDirection {
    IDLE = 0,
    CHARGING = 1,
    DISCHARGING = 2
};

enum ChargingStatus {
    NOT_CHARGING = 0,
    CHARGING_IN_PROGRESS = 1,
    CHARGING_DONE = 4
};



Axp2101::Axp2101(i2c_master_bus_config_t i2c_bus_cfg, uint8_t addr, gpio_num_t irq) : I2cDevice(i2c_bus_cfg, addr) {
// ** EFUSE defaults **
    WriteReg(AXP2101_PWROFF_EN, 0b110); // PWRON > OFFLEVEL as POWEROFF Source enable
    WriteReg(AXP2101_IRQ_OFF_ON_LEVEL_CTRL, 0x10);  // hold 4s to power off

    WriteReg(AXP2101_DC_ONOFF_DVM_CTRL, 0b1); //DCDC1 enable
    //Set DC1 to 3.3V
    WriteReg(AXP2101_DC_VOL0_CTRL, (3300 - 1500) / 100);
    
    WriteReg(AXP2101_LDO_ONOFF_CTRL0, 0x00);
    WriteReg(AXP2101_LDO_VOL0_CTRL, (3300 - 500) / 100);// Set ALDO1 to 3.3V
    WriteReg(AXP2101_LDO_VOL1_CTRL, (1800 - 500) / 100);
    WriteReg(AXP2101_LDO_ONOFF_CTRL0, 0b11);

    WriteReg(AXP2101_CV_CHG_VOL_SET, 0x03); // CV charger voltage setting to 4.2V
    
    WriteReg(AXP2101_IPRECHG_SET, 0x05); // set Main battery precharge current to 125mA
    WriteReg(AXP2101_ICC_CHG_SET, 0x0e); // set Main battery charger current to 800mA ( 0x08-200mA, 0x09-300mA, 0x0A-400mA )
    WriteReg(AXP2101_ITERM_CHG_SET_CTRL, 0x15); // set Main battery term charge current to 125mA

    WriteReg(AXP2101_MIN_SYS_VOL_CTRL, 0x01); // set minimum system voltage to 4.2V (default 4.7V), for poor USB cables
    WriteReg(AXP2101_INPUT_VOL_LIMIT_CTRL, 0x00); // set input voltage limit to 3.88v, for poor USB cables
    WriteReg(AXP2101_INPUT_CUR_LIMIT_CTRL, 0x05); // set input current limit to 2000mA

    WriteReg(AXP2101_VOFF_SET, 0x04); // set Vsys for PWROFF threshold to 3.0V (default - 2.6V and kill battery)
    WriteReg(AXP2101_TS_PIN_CTRL, 0x16); // set TS pin to EXTERNAL input (not temperature)

    

    gpio_config_t io_conf = {};
    //interrupt of rising edge
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = 1<<irq;
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);

    irq_num = irq;
}
void Axp2101::Update(uint8_t *data)
{
    if(gpio_get_level(irq_num) == 0)
    {
        uint8_t buffer[3];
        ReadRegs(AXP2101_INTSTS1, buffer,3);
        for (size_t i = 0; i < 3; i++)
        {
            ESP_LOGI(TAG, "INTSTS[%d]: 0x%02x", i, buffer[i]);
            if(buffer[i])
                WriteReg(AXP2101_INTSTS1+i, buffer[i]);
        }
        if(buffer[1]&0x08)
        {   
            data[0] |= 0x01; // Set bit 0 to indicate that the interrupt was handled
        }
        if(buffer[1]&0x04)
        {
            data[0] |= 0x01;
        }
    }
}
// int Axp2101::GetBatteryCurrentDirection() {
//     return (ReadReg(AXP2101_STATUS2) & 0b01100000) >> 5;
// }

// bool Axp2101::IsCharging() {
//     return GetBatteryCurrentDirection() == CHARGING;
// }

// bool Axp2101::IsDischarging() {
//     return GetBatteryCurrentDirection() == DISCHARGING;
// }

// bool Axp2101::IsChargingDone() {
//     uint8_t value = ReadReg(AXP2101_STATUS2);
//     return (value & 0b00000111) == CHARGING_DONE;
// }

// int Axp2101::GetBatteryLevel() {
//     return ReadReg(AXP2101_BAT_PERCENT_DATA);
// }

// void Axp2101::PowerOff() {
//     uint8_t value = ReadReg(AXP2101_COMMON_CONFIG);
//     value = value | 0x01;
//     WriteReg(AXP2101_COMMON_CONFIG, value);
// }
