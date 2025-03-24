#ifndef MESSAGE_TYPES_H
#define MESSAGE_TYPES_H

#include <vector>

// MaxxECU message configurations
struct MaxxECUConfig {
    uint32_t id;
    uint8_t data_length;
    const char* description;
    uint8_t default_value;
    std::vector<DataType> data_types;
    bool little_endian;
};

// Data format definitions
enum DataType { 
    UINT8, 
    UINT16, 
    UINT32, 
    INT8, 
    INT16, 
    INT32 
};

// Working configurations tested with MaxxECU Race
const MaxxECUConfig MAXXECU_CONFIGS[] = {
    // ID,   Len, Description,         Default, Data Types,        Little Endian
    {0x7B,   1,   "Start/Stop",       0x00,    {UINT8},          true},
    {0x7C,   2,   "Boost Control",    0x00,    {UINT16},         true},
    {0x7D,   4,   "Launch Control",   0x00,    {UINT16, UINT16}, true},
    {0x7E,   8,   "Shift Cut",        0x00,    {UINT16, UINT16, UINT32}, true},
    {0x7F,   2,   "Engine Temp",      0x00,    {INT16},          true}
};

// Message format structure
struct MessageFormat {
    bool is_extended;
    std::vector<DataType> data_types;
};

// Message format definitions
const std::unordered_map<uint32_t, MessageFormat> messageFormatMap = {
    // Blink Marine PKP-2600SI messages
    {0x18EFFF21, {true, {UINT8, UINT8, UINT8, UINT8, UINT8, UINT8, UINT8, UINT8}}},  // Keypad sending
    {0x18EF2100, {true, {UINT8, UINT8, UINT8, UINT8, UINT8, UINT8, UINT8, UINT8}}},  // Keypad receiving
    
    // MaxxECU Race configurable messages
    {0x7B, {false, {UINT8}}},
    {0x7C, {false, {UINT8, UINT8}}},
    {0x7D, {false, {UINT8}}},
    {0x7E, {false, {UINT8}}},
    {0x7F, {false, {UINT8, UINT8}}}
};

#endif // MESSAGE_TYPES_H