#include "axp2101.h"
#include "axp2101reg.h"
#include "board.h"

#include <esp_log.h>
#include "driver/gpio.h"

#define TAG "Axp2101"



Axp2101::Axp2101(i2c_master_bus_config_t i2c_bus_cfg, uint8_t addr, gpio_num_t irq) : I2cDevice(i2c_bus_cfg, addr) {
// ** EFUSE defaults **
    WriteReg(AXP2101_PWROFF_EN, 0b110); // PWRON > OFFLEVEL as POWEROFF Source enable
    WriteReg(AXP2101_IRQ_OFF_ON_LEVEL_CTRL, 0x10);  // hold 4s to power off

    WriteReg(AXP2101_INTEN2, 0xff); 

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



int Axp2101::GetBatteryLevel() {
    return ReadReg(AXP2101_BAT_PERCENT_DATA);
}

void Axp2101::Update(axp2101_data_t *data)
{
    data->event = 0;
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
            data->event |= SHORT_PRESS; 
        if(buffer[1]&0x80)
            data->event |= VBUS_INSERT; 
        if(buffer[1]&0x40)
            data->event |= VBUS_REMOVE; 
        if(buffer[2]&0x10)
            data->event |= CHARGING_DONE; 
        if(buffer[2]&0x08)
            data->event |= CHARGING_START;
    }
    static int count=0;
    if(++count >= 2000)
    {
        count = 0;
        data->battery_level = GetBatteryLevel();
    }
}


