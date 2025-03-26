#ifndef MESSAGE_TYPES_H
#define MESSAGE_TYPES_H

#include <vector>

enum class DataType {
    UINT8,
    UINT16,
    INT16,
    // ...other types as needed
};

struct MessageConfig {
    std::vector<DataType> data_types;
    bool little_endian;
    // ...other config fields
};

// MaxxECU message configurations
struct MaxxECUConfig {
    uint32_t id;
    uint8_t data_length;
    const char* description;
    uint8_t default_value;
    std::vector<DataType> data_types;
    bool little_endian;
};

// Working configurations tested with MaxxECU Race
const MaxxECUConfig MAXXECU_CONFIGS[] = {
    // ID,   Len, Description,         Default, Data Types,        Little Endian
    {0x7B,   1,   "Start/Stop",       0x00,    {DataType::UINT8},          true},
    {0x7C,   2,   "Boost Control",    0x00,    {DataType::UINT16},         true},
    {0x7D,   4,   "Launch Control",   0x00,    {DataType::UINT16, DataType::UINT16}, true},
    {0x7E,   8,   "Shift Cut",        0x00,    {DataType::UINT16, DataType::UINT16, DataType::UINT32}, true},
    {0x7F,   2,   "Engine Temp",      0x00,    {DataType::INT16},          true}
};

// Message format structure
struct MessageFormat {
    bool is_extended;
    std::vector<DataType> data_types;
};

// Message format definitions
const std::unordered_map<uint32_t, MessageFormat> messageFormatMap = {
    // Blink Marine PKP-2600SI messages
    {0x18EFFF21, {true, {DataType::UINT8, DataType::UINT8, DataType::UINT8, DataType::UINT8, DataType::UINT8, DataType::UINT8, DataType::UINT8, DataType::UINT8}}},  // Keypad sending
    {0x18EF2100, {true, {DataType::UINT8, DataType::UINT8, DataType::UINT8, DataType::UINT8, DataType::UINT8, DataType::UINT8, DataType::UINT8, DataType::UINT8}}},  // Keypad receiving
    
    // MaxxECU Race configurable messages
    {0x7B, {false, {DataType::UINT8}}},
    {0x7C, {false, {DataType::UINT8, DataType::UINT8}}},
    {0x7D, {false, {DataType::UINT8}}},
    {0x7E, {false, {DataType::UINT8}}},
    {0x7F, {false, {DataType::UINT8, DataType::UINT8}}}
};

#endif // MESSAGE_TYPES_H