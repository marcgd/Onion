#ifndef RUMBLE_H__
#define RUMBLE_H__

#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdbool.h>

#include "utils/file.h"
#include "utils/msleep.h"
#include "settings.h"


#define SHORT_PULSE_MS 100
#define SUPER_SHORT_PULSE_MS 50


void rumble(bool enabled)
{
    file_write("/sys/class/gpio/export", "48", 2);
    file_write("/sys/class/gpio/gpio48/direction", "out", 3);
    file_write("/sys/class/gpio/gpio48/value", enabled ? "0" : "1", 1);
}

/**
 * @brief Turns on vibration for 100ms
 * 
 */
void short_pulse(void) {
    if (!settings.vibration) return;
    rumble(true);
    msleep(SHORT_PULSE_MS);
    rumble(false);
}

/**
 * @brief Turns on vibration for 50ms
 * 
 */
void super_short_pulse(void) {
    if (!settings.vibration) return;
    rumble(true);
    msleep(SUPER_SHORT_PULSE_MS);
    rumble(false);
}

/**
 * @brief Turns on vibration for 50ms
 * 
 */
void menu_short_pulse(void) {
    if (!settings.vibration || !settings.menu_haptics) return;
    rumble(true);
    msleep(SHORT_PULSE_MS);
    rumble(false);
}

/**
 * @brief Turns on vibration for 50ms
 * 
 */
void menu_super_short_pulse(void) {
    if (!settings.vibration || !settings.menu_haptics) return;
    rumble(true);
    msleep(SUPER_SHORT_PULSE_MS);
    rumble(false);
}

#endif // RUMBLE_H__
