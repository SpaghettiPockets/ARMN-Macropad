#include "tusb.h"
#include "pico/unique_id.h"
#include <string.h>

/* TinyUSB example VID/PID, for this personal prototype only.
 * A commercial product requires an appropriately allocated VID/PID. */
static tusb_desc_device_t const device = {
    .bLength = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor = 0xCAFE, .idProduct = 0x4004,
    .bcdDevice = 0x0100,
    .iManufacturer = 1, .iProduct = 2, .iSerialNumber = 3,
    .bNumConfigurations = 1
};
static uint8_t const hid_report[] = {TUD_HID_REPORT_DESC_KEYBOARD()};
static uint8_t const configuration[] = {
    TUD_CONFIG_DESCRIPTOR(1, 1, 0, TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN, 0, 100),
    TUD_HID_DESCRIPTOR(0, 0, HID_ITF_PROTOCOL_KEYBOARD,
                       sizeof(hid_report), 0x81, CFG_TUD_HID_EP_BUFSIZE, 1)
};
uint8_t const *tud_descriptor_device_cb(void) { return (uint8_t const *)&device; }
uint8_t const *tud_descriptor_configuration_cb(uint8_t index) {
    (void)index; return configuration;
}
uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance) {
    (void)instance; return hid_report;
}
uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
    (void)langid;
    static uint16_t result[32];
    static char serial[PICO_UNIQUE_BOARD_ID_SIZE_BYTES * 2 + 1];
    const char *str;
    if (index == 0) {
        result[0] = (TUSB_DESC_STRING << 8) | 4;
        result[1] = 0x0409;
        return result;
    }
    switch (index) {
        case 1: str = "SpaghettiPockets"; break;
        case 2: str = "ARMN-Macropad"; break;
        case 3:
            pico_get_unique_board_id_string(serial, sizeof(serial));
            str = serial;
            break;
        default: return NULL;
    }
    size_t len = strlen(str);
    if (len > 31) len = 31;
    for (size_t i = 0; i < len; ++i) result[i + 1] = (uint8_t)str[i];
    result[0] = (TUSB_DESC_STRING << 8) | (2 * len + 2);
    return result;
}
