#ifndef MESSAGE_TYPES_H
#define MESSAGE_TYPES_H

#include <array>
#include <cstdint>
#include <initializer_list>
#include <unordered_map>

// Maximum number of data types per message
constexpr size_t MAX_DATA_TYPES = 4;

enum class DataType : uint8_t {
    UINT8,
    UINT16,
    INT16,
    UINT32
};

struct MessageConfig {
    uint32_t id;
    uint8_t data_length;
    const char* description;
    uint8_t mask;
    std::array<DataType, MAX_DATA_TYPES> data_types;
    uint8_t data_type_count;
    bool little_endian;

    constexpr MessageConfig(uint32_t _id, uint8_t _len, const char* _desc, uint8_t _mask,
                 std::initializer_list<DataType> _types, bool _le) noexcept
        : id(_id), data_length(_len), description(_desc), mask(_mask),
          data_types(), data_type_count(0), little_endian(_le) 
    {
        size_t i = 0;
        for (auto type : _types) {
            if (i < MAX_DATA_TYPES) {
                data_types[i++] = type;
            }
        }
        data_type_count = i;
    }
} __attribute__((packed));

// Define configurations using the constructor
constexpr MessageConfig MAXXECU_CONFIGS[] = {
    {0x7C, 2, "Boost Control",  0xFF, {DataType::UINT16}, true},
    {0x7D, 4, "Launch Control", 0xFF, {DataType::UINT16, DataType::UINT16}, true},
    {0x7E, 8, "Shift Cut",      0xFF, {DataType::UINT16, DataType::UINT16, DataType::UINT32}, true},
    {0x7F, 2, "Engine Temp",    0xFF, {DataType::INT16}, true}
};

// Message format structure
struct MessageFormat {
    bool is_extended;
    std::array<DataType, MAX_DATA_TYPES> data_types;
    uint8_t data_type_count;

    MessageFormat() : is_extended(false), data_types(), data_type_count(0) {}
};

// Message format definitions
const std::unordered_map<uint32_t, MessageFormat> messageFormatMap = {
    // Blink Marine PKP-2600SI messages
    {0x18EFFF21, {true, {DataType::UINT8, DataType::UINT8, DataType::UINT8, DataType::UINT8, DataType::UINT8, DataType::UINT8, DataType::UINT8, DataType::UINT8}, 8}},  // Keypad sending
    {0x18EF2100, {true, {DataType::UINT8, DataType::UINT8, DataType::UINT8, DataType::UINT8, DataType::UINT8, DataType::UINT8, DataType::UINT8, DataType::UINT8}, 8}},  // Keypad receiving
    
    // MaxxECU Race configurable messages
    {0x7B, {false, {DataType::UINT8}, 1}},
    {0x7C, {false, {DataType::UINT8, DataType::UINT8}, 2}},
    {0x7D, {false, {DataType::UINT8}, 1}},
    {0x7E, {false, {DataType::UINT8}, 1}},
    {0x7F, {false, {DataType::UINT8, DataType::UINT8}, 2}}
};

#endif // MESSAGE_TYPES_H