#ifndef _USB_H_
#define _USB_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"



typedef struct   {
    uint8_t modifier;
    uint8_t reserved;
    uint8_t keycode[6];
} KeyboardReport;

typedef struct   {
    uint8_t buttons;
    int16_t x;
    int16_t y;
    int8_t scroll;
    int8_t pan;
} MouseReport;

typedef struct 
{
    int16_t lx;
    int16_t ly;
    int16_t rx;
    int16_t ry;
    int16_t lz;
    int16_t rz;
    uint32_t buttons;
}GamepadReport;

typedef struct  {
    uint8_t report_id;
    uint8_t report_size;
    uint8_t buttons_0;
    uint8_t buttons_1;
    uint8_t lz;
    uint8_t rz;
    int16_t lx;
    int16_t ly;
    int16_t rx;
    int16_t ry;
    uint8_t reserved[6];
} XInputReport;

enum
{
  REPORT_KEYBOARD = 1,
  REPORT_MOUSE,
  REPORT_GAMEPAD,
  REPORT_XINPUT,
  REPORT_WEBUSB,
  REPORT_COUNT
};

struct hid_report_t
{
    uint8_t report_id;
    uint8_t *data;
    uint16_t len;
};

class USB {
private:
    QueueHandle_t hid_queue;
    static void hid_task(void *pvParameters);
    static void tusb_task(void *pvParameters);
    template<typename T>
    bool send_report(uint8_t report_id, const T& report_data);

public:
    bool keyboard_report(const  KeyboardReport& keyboard_report);
    bool mouse_report(const MouseReport& mouse_report);
    bool gamepad_report(const GamepadReport& gamepad_report);
    bool xinput_report(const XInputReport& xinput_report);
    USB();
    
};



#endif
