#include "pico/stdlib.h"
#include "bsp/board.h"
#include "tusb.h"
#include "keys.h"

static key_state_t keys;

int main(void) {
    board_init();
    for (unsigned i = 0; i < KEY_COUNT; ++i) {
        gpio_init(key_pins[i]);
        gpio_set_dir(key_pins[i], GPIO_IN);
        gpio_pull_up(key_pins[i]);
    }
    tusb_init();
    while (true) {
        tud_task();
        uint8_t pressed = 0;
        for (unsigned i = 0; i < KEY_COUNT; ++i)
            if (!gpio_get(key_pins[i])) pressed |= (1u << i);
        keys_update(&keys, pressed, to_ms_since_boot(get_absolute_time()));
        /* Send full current state whenever USB is ready (1 ms polling).
         * Releases are zero reports; reconnects cannot retain an old report.
         * No firmware sleep, radio, macros or automatic modifiers. */
        if (tud_hid_ready()) {
            uint8_t report[6];
            keys_report(&keys, report);
            tud_hid_keyboard_report(0, 0, report);
        }
    }
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id,
                             hid_report_type_t report_type, uint8_t *buffer,
                             uint16_t reqlen) {
    (void)instance;
    (void)report_id;
    if (report_type != HID_REPORT_TYPE_INPUT) return 0;
    uint8_t report[8] = {0};
    keys_report(&keys, report + 2);
    uint16_t len = reqlen < sizeof(report) ? reqlen : sizeof(report);
    memcpy(buffer, report, len);
    return len;
}
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id,
                          hid_report_type_t report_type, uint8_t const *buffer,
                          uint16_t bufsize) {
    /* Host keyboard LEDs are intentionally unused. */
    (void)instance; (void)report_id; (void)report_type;
    (void)buffer; (void)bufsize;
}
