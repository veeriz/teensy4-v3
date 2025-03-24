#ifndef CAN_OBJECTS_H
#define CAN_OBJECTS_H

#include "config.h"

// CAN objects configuration structure
struct CANObject {
    const char* name;
    const char* type;
    int levels;
    int key_number;
    const char* mode;
    int led_colors[5];
    uint32_t forward_address;
    int pwm_pins[2];
    int pwm_pin_count;
    uint16_t forward_messages[3][2];
    bool invert_duty_cycle[2];  // Per-pin inversion settings
    int min_pwm;
    int max_pwm;
    bool enabled;
};

// CAN objects array with configurations
const CANObject CAN_OBJECTS[] = {
    // name,     type,      lvls, key#, mode,     LED colors,                  fwd_addr,   PWM pins, #pins, fwd_msgs,                    invert,           min, max,  enabled
    {"1Key",     "binary",  1,    0x05, "latch",  {0x00, 0x02},              0,          {24},     1,     {{0x00, 0x00}, {2500}, {3000}}, {false, false}, 0,   100,  true},
    {"BoostKey", "multi",   2,    0x04, "latch",  {0x00, 0x03, 0x06},        0x07C,      {0},      0,     {{0x00, 0x00}, {2500}, {3000}}, {false, false}, 0,   100,  true},
    {"TCKey",    "binary",  1,    0x03, "latch",  {0x00, 0x04, 0x05, 0x07, 0x08}, 0,    {0},      0,     {{0x00, 0x00}, {2500}, {3000}}, {false, false}, 0,   100,  false},
    {"StartKey", "momentary",1,   0x0C, "momentary",{0x00, 0x05},            0x07B,      {0},      0,     {{0x00, 0x00}, {0x01}},        {false, false}, 0,   100,  true},
    {"2Key",     "binary",  1,    0x0B, "momentary",{0x00, 0x06},           0,          {25},     1,     {{0x00, 0x00}, {2500}, {3000}}, {false, false}, 0,   100,  true},
    {"LCKey",    "binary",  1,    0x0A, "latch",  {0x00, 0x07},              0,          {0},      0,     {{0x00, 0x00}, {2500}, {3000}}, {false, false}, 0,   100,  true},
    {"ALSKey",   "binary",  1,    0x09, "latch",  {0x00, 0x08},              0,          {0},      0,     {{0x00, 0x00}, {2500}, {3000}}, {false, false}, 0,   100,  false},
    {"CoolingFan","multi",  4,    0x06, "latch",  {0x07, 0x03, 0x02, 0x04, 0x01}, 0x18FFA07A, {18, 19}, 2, {{0x00, 0x00}, {2500}, {3000}}, {true, false}, MIN_PWM, MAX_PWM, true},
    {"Key0",     "binary",  1,    0x00, "momentary",{0x00, 0x01},           0,          {0},      0,     {{0x00, 0x00}, {0x01}},        {false, false}, 0,   100,  true}
};

#endif // CAN_OBJECTS_H