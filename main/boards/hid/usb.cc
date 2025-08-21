#include "usb.h"
#include "esp_log.h"
#include "esp_err.h"
#include <cstring>
#include "tusb.h"
#include "esp_private/usb_phy.h"
#include "webusb.h"

#define TAG "USB"


/**
 * @brief String descriptor
 */
 static const char *const descriptor_string[] = {
    // array of pointer to string descriptors
    (char[]){0x09, 0x04}, // 0: is supported language is English (0x0409)
    "qunke",             // 1: Manufacturer
    "alpakka",          // 2: Product
    "V1",             // 3: Serials, should use chip ID
    "HID",
    "WEBUSB",
    "XINPUT_GENERIC_CONTROLLER"
};

uint8_t const descriptor_report_generic[] = {
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(REPORT_KEYBOARD)),
    TUD_HID_REPORT_DESC_MOUSE_CUSTOM(HID_REPORT_ID(REPORT_MOUSE)),
    TUD_HID_REPORT_DESC_GAMEPAD_CUSTOM(HID_REPORT_ID(REPORT_GAMEPAD)),
};

uint8_t const descriptor_report_xinput[] = {
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(REPORT_KEYBOARD)),
    TUD_HID_REPORT_DESC_MOUSE_CUSTOM(HID_REPORT_ID(REPORT_MOUSE)),
};


uint8_t descriptor_configuration_generic[] = {
    DESCRIPTOR_CONFIGURATION(2),
    DESCRIPTOR_INTERFACE_HID(sizeof(descriptor_report_generic)),
    DESCRIPTOR_INTERFACE_WEBUSB
};

uint8_t descriptor_configuration_xinput[] = {
    DESCRIPTOR_CONFIGURATION(3),
    DESCRIPTOR_INTERFACE_HID(sizeof(descriptor_report_xinput)),
    DESCRIPTOR_INTERFACE_WEBUSB,
    DESCRIPTOR_INTERFACE_XINPUT
};

typedef enum _Protocol {
    PROTOCOL_UNDEFINED = -1,
    PROTOCOL_XINPUT_WIN = 0,
    PROTOCOL_XINPUT_UNIX,
    PROTOCOL_GENERIC,
} Protocol;
uint8_t config_get_protocol() {
    return PROTOCOL_GENERIC;
}
// // // /********* TinyUSB HID callbacks ***************/

uint8_t const *tud_descriptor_device_cb() {
    ESP_LOGI(TAG, "tud_descriptor_device_cb");
    static tusb_desc_device_t descriptor_device = {DESCRIPTOR_DEVICE};
    if (config_get_protocol() == PROTOCOL_XINPUT_WIN) {
        descriptor_device.idVendor = USB_WIN_VENDOR;
        // #ifdef DEVICE_IS_ALPAKKA
             descriptor_device.idProduct = USB_WIN_PRODUCT_ALPAKKA;
        // #elif defined DEVICE_DONGLE
        //    descriptor_device.idProduct = USB_WIN_PRODUCT_DONGLE;
        //#endif
    }
    if (config_get_protocol() == PROTOCOL_XINPUT_UNIX) {
        descriptor_device.idVendor = USB_UNIX_VENDOR;
        descriptor_device.idProduct = USB_UNIX_PRODUCT;
    }
    if (config_get_protocol() == PROTOCOL_GENERIC) {
        descriptor_device.idVendor = USB_GENERIC_VENDOR;
        // #ifdef DEVICE_IS_ALPAKKA
             descriptor_device.idProduct = USB_GENERIC_PRODUCT_ALPAKKA;
        // #elif defined DEVICE_DONGLE
        //    descriptor_device.idProduct = USB_GENERIC_PRODUCT_DONGLE;
        //#endif
    }
    return (uint8_t const *) &descriptor_device;
}
uint8_t const *tud_descriptor_configuration_cb(uint8_t index) {
    if (config_get_protocol() == PROTOCOL_GENERIC) {
        ESP_LOGI(TAG, "tud_descriptor_configuration_cb %d size %d", index, sizeof(descriptor_configuration_generic));
        descriptor_configuration_generic[2] = sizeof(descriptor_configuration_generic);
        return descriptor_configuration_generic;
    } else {
        ESP_LOGI(TAG, "tud_descriptor_configuration_cb %d size %d", index, sizeof(descriptor_configuration_generic));
        descriptor_configuration_xinput[2] = sizeof(descriptor_configuration_xinput);
        return descriptor_configuration_xinput;
    }
}

uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance) {
    ESP_LOGI(TAG, "tud_hid_descriptor_report_cb %d", instance);
    if (config_get_protocol() == PROTOCOL_GENERIC) return descriptor_report_generic;
    else return descriptor_report_xinput;
}


const uint16_t *tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
    ESP_LOGI(TAG, "tud_descriptor_string_cb %d", index);
    if (index == 0xEE && config_get_protocol() != PROTOCOL_XINPUT_UNIX) {
        static uint8_t msos[] = {MS_OS_DESCRIPTOR};
        return (uint16_t*)msos;
    }
    if (index >= sizeof(descriptor_string) / sizeof(descriptor_string[0])) {
        return NULL;
    }
    static uint16_t response[64];
    const char *string = descriptor_string[index];
    uint8_t i = 0;
    for (i=0; string[i]; i++) {
        response[i + 1] = string[i];
    }
    response[0] = TUSB_DESC_STRING << 8;  // String type.
    response[0] |= (i * 2) + 2;           // Total length.
    return response;
}

bool tud_vendor_control_xfer_cb(uint8_t rhport, uint8_t stage, tusb_control_request_t const * request)
{
    ESP_LOGI(TAG, "tud_vendor_control_xfer_cb %i %x", stage, request->wIndex);
    if (stage != CONTROL_STAGE_SETUP) return true;
    // Compatibility IDs.
    if (
        request->wIndex == 0x0004 &&
        request->bRequest == MS_OS_VENDOR
    ) {
        if (config_get_protocol() == PROTOCOL_XINPUT_WIN) {
            static uint8_t response[] = {MS_OS_COMPATIDS_ALL};
            return tud_control_xfer(rhport, request, response, sizeof(response));
        }
        if (config_get_protocol() == PROTOCOL_GENERIC) {
            static uint8_t response[] = {MS_OS_COMPATIDS_GENERIC};
            return tud_control_xfer(rhport, request, response, sizeof(response));
        }

    }
    // Extended properties.
    if (
        request->wIndex == 0x0005 &&
        request->bRequest == MS_OS_VENDOR &&
        config_get_protocol() != PROTOCOL_XINPUT_UNIX
    ) {
        static uint8_t response[] = {MS_OS_PROPERTIES};
        return tud_control_xfer(rhport, request, response, sizeof(response));
    }
    // Return false if there is no control data to transfer.
    return false;
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
    (void) instance;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;

    return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
    ESP_LOGI(TAG, "tud_hid_set_report_cb");
}

void tud_mount_cb(void) {
    ESP_LOGI(TAG, "tud_mount_cb");
}

void tud_umount_cb(void) {
    ESP_LOGI(TAG, "tud_umount_cb");
}
void tud_suspend_cb(bool remote_wakeup_en) {
    ESP_LOGI(TAG, "tud_suspend_cb");
}
void tud_resume_cb(void) {
    ESP_LOGI(TAG, "tud_resume_cb");
}




template<typename T>
bool USB::send_report(uint8_t report_id, const T& report_data) {
    hid_report_t report = {
        .report_id = report_id,
        .data = (uint8_t *)malloc(sizeof(T)),
        .len = sizeof(T)
    };
    if (!report.data) return false;
    
    memcpy(report.data, &report_data, report.len);
    bool result = xQueueSend(hid_queue, &report, 0);
    if (!result) {
        free(report.data);
        return false;
    }
    return true;
}

bool USB::keyboard_report(const KeyboardReport& keyboard_report) {
    return send_report(REPORT_KEYBOARD, keyboard_report);
}

bool USB::mouse_report(const MouseReport& mouse_report) {
    return send_report(REPORT_MOUSE, mouse_report);
}

bool USB::gamepad_report(const GamepadReport& gamepad_report) {
    return send_report(REPORT_GAMEPAD, gamepad_report);
}

bool USB::xinput_report(const XInputReport& xinput_report) {
    return send_report(REPORT_XINPUT, xinput_report);
}
//消息队列
void USB::hid_task(void *pvParameters) {
    QueueHandle_t* hid_queue_p = static_cast<QueueHandle_t*>(pvParameters);
    int i=0;
    while (1) {
        tud_task(); // tinyusb task
        hid_report_t report;
        if(xQueueReceive(*hid_queue_p, &report, 1) == pdPASS)//portMAX_DELAY
        {
            if (tud_ready()) 
            {
                if(report.report_id==REPORT_XINPUT)
                {
                // if (wired) xinput_send_report(&report);
                // else wireless_send_hid(REPORT_XINPUT, &report, sizeof(report));
                // hid_set_gamepad_synced();
                }else
                //if (tud_hid_ready()) 
                {
                    tud_hid_report(report.report_id, report.data, report.len);

                    if(++i>=999)
                    {
                        i=0;
                        ESP_LOGI(TAG, "tud_hid_report %d %d",report.report_id,report.len);
                        for(int j=0;j<report.len;j++)
                        {
                            printf("%02x ",report.data[j]);
                        }
                        printf("\n");
                    }
                }   
            }
            free(report.data);
        }else
        {
                //webusb_read();
                //webusb_flush();
        }
    }
}

USB::USB(void)
{

    ESP_LOGI(TAG, "USB initialization DONE");
    
    static usb_phy_handle_t phy_handle;
    // Configure USB PHY
    usb_phy_config_t phy_conf = {
        .controller = USB_PHY_CTRL_OTG,
        .target = USB_PHY_TARGET_INT,
        .otg_mode = USB_OTG_MODE_DEVICE,
        .otg_speed = USB_PHY_SPEED_FULL,
        .ext_io_conf = NULL,
        .otg_io_conf = NULL,
    };
    usb_new_phy(&phy_conf, &phy_handle);

    tusb_rhport_init_t dev_init = {
        .role = TUSB_ROLE_DEVICE,
        .speed = TUSB_SPEED_AUTO,
    };
    tusb_init(BOARD_TUD_RHPORT, &dev_init);

    hid_queue = xQueueCreate(4, sizeof(hid_report_t));
    xTaskCreatePinnedToCore(hid_task, "hid_task", 4096, &hid_queue, 5, NULL, 1);
}

