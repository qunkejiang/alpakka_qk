#ifndef __AXP2101_H__
#define __AXP2101_H__

#include "i2c_device.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

class Axp2101 : public I2cDevice {
public:
    Axp2101(i2c_master_bus_config_t i2c_bus_cfg, uint8_t addr, gpio_num_t irq);
    void Update(uint8_t *data);
//     bool IsCharging();
//     bool IsDischarging();
//     bool IsChargingDone();
//     int GetBatteryLevel();
//     void PowerOff();
//     void isKeyPressed();

private:
    gpio_num_t irq_num;
    //int GetBatteryCurrentDirection();
};

#endif
