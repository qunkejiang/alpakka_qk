#include "usb.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_private/usb_phy.h"
#include "tusb_config.h"
#include "tusb.h"
#include "board.h"



 static const char *const descriptor_string[] = {
    (const char[]){0x09, 0x04},  // English.
    STRING_VENDOR,
    STRING_PRODUCT_ALPAKKA,
    STRING_VERSION_ALPAKKA_V1,
    STRING_HID,
    STRING_WEBUSB,
    STRING_XINPUT
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

uint8_t const *tud_descriptor_device_cb() {
    logging::debug_uart("tud_descriptor_device_cb\n");
    uint8_t protocol = Board::get_protocol();
    logging::debug_uart("protocol: %d\n", protocol);
    static tusb_desc_device_t descriptor_device = {DESCRIPTOR_DEVICE};
    if (protocol == PROTOCOL_XINPUT_WIN) {
        descriptor_device.idVendor = USB_WIN_VENDOR;
        descriptor_device.idProduct = USB_WIN_PRODUCT_ALPAKKA;
    }
    if (protocol == PROTOCOL_XINPUT_UNIX) {
        descriptor_device.idVendor = USB_UNIX_VENDOR;
        descriptor_device.idProduct = USB_UNIX_PRODUCT;
    }
    if (protocol == PROTOCOL_GENERIC) {
        descriptor_device.idVendor = USB_GENERIC_VENDOR;
        descriptor_device.idProduct = USB_GENERIC_PRODUCT_ALPAKKA;
    }
    return (uint8_t const *) &descriptor_device;
}

uint8_t const *tud_descriptor_configuration_cb(uint8_t index) {
    logging::debug_uart("tud_descriptor_configuration_cb %d\n", index);
    if (Board::get_protocol() == PROTOCOL_GENERIC) {
        descriptor_configuration_generic[2] = sizeof(descriptor_configuration_generic);
        return descriptor_configuration_generic;
    } else {
        descriptor_configuration_xinput[2] = sizeof(descriptor_configuration_xinput);
        return descriptor_configuration_xinput;
    }
}

uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance) {
    logging::debug_uart("tud_hid_descriptor_report_cb %d\n", instance);
    if (Board::get_protocol() == PROTOCOL_GENERIC) return descriptor_report_generic;
    else return descriptor_report_xinput;
}


const uint16_t *tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
    logging::debug_uart("tud_descriptor_string_cb %d\n", index);
    if (index == 0xEE && Board::get_protocol() != PROTOCOL_XINPUT_UNIX) {
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
    logging::debug_uart("tud_vendor_control_xfer_cb %i %x\n", stage, request->wIndex);
    if (stage != CONTROL_STAGE_SETUP) return true;
    // Compatibility IDs.
    if (
        request->wIndex == 0x0004 &&
        request->bRequest == MS_OS_VENDOR
    ) {
        if (Board::get_protocol() == PROTOCOL_XINPUT_WIN) {
            static uint8_t response[] = {MS_OS_COMPATIDS_ALL};
            return tud_control_xfer(rhport, request, response, sizeof(response));
        }
        if (Board::get_protocol() == PROTOCOL_GENERIC) {
            static uint8_t response[] = {MS_OS_COMPATIDS_GENERIC};
            return tud_control_xfer(rhport, request, response, sizeof(response));
        }

    }
    // Extended properties.
    if (
        request->wIndex == 0x0005 &&
        request->bRequest == MS_OS_VENDOR &&
        Board::get_protocol() != PROTOCOL_XINPUT_UNIX
    ) {
        static uint8_t response[] = {MS_OS_PROPERTIES};
        return tud_control_xfer(rhport, request, response, sizeof(response));
    }
    // Return false if there is no control data to transfer.
    return false;
}
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
    logging::debug_uart("tud_hid_get_report_cb %d %d %d\n", instance, report_id, report_type);
    return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
    logging::debug_uart("tud_hid_set_report_cb %d %d %d %d\n", instance, report_id, report_type, bufsize);
}


void tud_mount_cb(void) {
    logging::debug_uart("tud_mount_cb\n");
}

void tud_umount_cb(void) {
    logging::debug_uart("tud_umount_cb\n");
}
void tud_suspend_cb(bool remote_wakeup_en) {
    logging::debug_uart("tud_suspend_cb\n");
}

void tud_resume_cb(void) {
    logging::debug_uart("tud_resume_cb\n");
}

USB::USB()
{
    logging::debug_uart("USB initialization DONE\n");
    
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
    //tusb_init();
}




