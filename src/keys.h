#ifndef ARMN_KEYS_H
#define ARMN_KEYS_H
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define KEY_COUNT 4
#define DEBOUNCE_MS 5u
static const uint8_t key_pins[KEY_COUNT] = {16, 18, 20, 22};
/* USB HID Keyboard usages: A, R, M, N. No implicit Shift. */
static const uint8_t key_codes[KEY_COUNT] = {0x04, 0x15, 0x10, 0x11};
typedef struct {
    bool candidate[KEY_COUNT];
    bool stable[KEY_COUNT];
    uint32_t changed_at[KEY_COUNT];
} key_state_t;

static inline void keys_update(key_state_t *s, uint8_t pressed, uint32_t now) {
    for (unsigned i = 0; i < KEY_COUNT; ++i) {
        bool down = (pressed & (1u << i)) != 0;
        if (down != s->candidate[i]) {
            s->candidate[i] = down;
            s->changed_at[i] = now;
        }
        if ((uint32_t)(now - s->changed_at[i]) >= DEBOUNCE_MS)
            s->stable[i] = s->candidate[i];
    }
}
static inline void keys_report(const key_state_t *s, uint8_t report[6]) {
    memset(report, 0, 6);
    unsigned n = 0;
    for (unsigned i = 0; i < KEY_COUNT; ++i)
        if (s->stable[i]) report[n++] = key_codes[i];
}
#endif
