#include <assert.h>
#include <stdio.h>
#include "keys.h"
int main(void) {
    key_state_t s = {0};
    uint8_t r[6];
    keys_update(&s, 1, 0);
    keys_update(&s, 0, 1);
    keys_update(&s, 1, 2);
    keys_update(&s, 1, 6);
    keys_report(&s, r); assert(r[0] == 0);
    keys_update(&s, 1, 7);
    keys_report(&s, r); assert(r[0] == 0x04 && r[1] == 0);
    keys_update(&s, 15, 8);
    keys_update(&s, 15, 13);
    keys_report(&s, r);
    assert(r[0] == 0x04 && r[1] == 0x15 && r[2] == 0x10 && r[3] == 0x11);
    assert(r[4] == 0 && r[5] == 0);
    keys_update(&s, 0, 14);
    keys_update(&s, 15, 15);
    keys_update(&s, 0, 16);
    keys_update(&s, 0, 20);
    keys_report(&s, r); assert(r[3] == 0x11);
    keys_update(&s, 0, 21);
    keys_report(&s, r);
    for (unsigned i = 0; i < 6; ++i) assert(r[i] == 0);
    keys_update(&s, 8, UINT32_MAX - 2);
    keys_update(&s, 8, 2);
    keys_report(&s, r); assert(r[0] == 0x11 && r[1] == 0);
    puts("PASS: mapping, simultaneous keys, press/release debounce, release report, timer wrap");
}
