#ifndef __AXP2101_H__
#define __AXP2101_H__

#include "i2c_device.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define SHORT_PRESS (0x01)
#define VBUS_INSERT  (0x02)
#define VBUS_REMOVE  (0x04)
#define CHARGING_DONE (0x08)
#define CHARGING_START (0x10)

struct axp2101_data_t
{
    uint8_t event;
    uint8_t battery_level;
};


class Axp2101 : public I2cDevice {
public:
    Axp2101(i2c_master_bus_config_t i2c_bus_cfg, uint8_t addr, gpio_num_t irq);
    void Update(axp2101_data_t *data);
private:
    int GetBatteryLevel();
    gpio_num_t irq_num;
};

#endif
