# MaxxECU Fan Controller

## Overview
Custom fan controller interface integrating MaxxECU Race and Blink Marine PKP-2600-SI keypad control for dual cooling fans, featuring temperature-based PID control and CAN bus integration.

*Last Updated: 2025-03-24 01:48:56 UTC*  
*Author: veeriz*

## Features
- Blink Marine PKP-2600-SI keypad integration
- Dual fan control with independent PWM channels
- Temperature-based PID control using MaxxECU data
- Support for inverted and non-inverted PWM outputs
- Advanced safety features and fault detection
- Comprehensive monitoring and diagnostics
- CAN bus integration with MaxxECU Race

## Hardware Requirements
- Arduino-compatible board with CAN bus support
- Blink Marine PKP-2600-SI keypad
- Dual PWM-capable cooling fans
- MaxxECU Race ECU
- CAN bus interface (120Ω termination required)

### Pin Configuration
```
Pin Assignments:
- Pin 18: Fan 1 (Inverted PWM)
- Pin 19: Fan 2 (Normal PWM)
```

## Software Dependencies
- FlexCAN_T4 library
- Arduino core libraries

## CAN Message Format

### Blink Marine Keypad Messages
| ID         | Length | Description      | Direction |
|------------|--------|------------------|-----------|
| 0x18EFFF21 | 8      | Keypad Input    | In       |
| 0x18EF2100 | 8      | Keypad Response | Out      |

### MaxxECU Messages
| ID    | Length | Description    | Data Type | Format    |
|-------|---------|---------------|-----------|-----------|
| 0x7F  | 2       | Engine Temp   | INT16     | 0.1°C/bit |
| 0x7B  | 1       | Start/Stop    | UINT8     | Binary    |
| 0x7C  | 2       | Boost Control | UINT16    | 0.1kPa/bit|

### Keypad Configuration
```cpp
Key Assignments:
- Key 0: Momentary start/stop
- Key 1: Binary toggle
- Key 2: Binary toggle
- Key 3: TC control
- Key 4: Boost control (multi-level)
- Key 5: Binary toggle
- Key 6: Cooling fan control (multi-level)
- Key 9: ALS control
- Key 10: LC control
- Key 11: Binary toggle
```

### LED Color Mapping
```cpp
LED States:
0x00: Off
0x01: Red
0x02: Green
0x03: Yellow
0x04: Blue
0x05: Magenta
0x06: Cyan
0x07: White
0x08: Orange
0x09: Custom
```

### Temperature Control
The system maintains engine temperature around 85°C using PID control:
```
Temperature -> Fan Speed
75°C -> 20% (minimal cooling)
80°C -> 40% (light cooling)
85°C -> 50% (target temperature)
90°C -> 70% (increased cooling)
95°C -> 85% (heavy cooling)
100°C -> 100% (maximum cooling)
```

## Configuration

### Key Types
- `binary`: Toggle on/off
- `momentary`: Active while pressed
- `multi`: Multiple states cycling

### PID Parameters
```cpp
const float KP = 2.0;  // Proportional gain
const float KI = 0.1;  // Integral gain
const float KD = 1.0;  // Derivative gain
```

### Fan Parameters
```cpp
const int MIN_PWM = 5;     // Minimum PWM duty cycle
const int MAX_PWM = 90;    // Maximum PWM duty cycle
const int TARGET_TEMP = 85; // Target temperature (°C)
```

## Safety Features
1. Keypad debouncing (50ms)
2. Soft start with ramping
3. Minimum run time protection
4. Fault detection and counting
5. Temperature trend monitoring
6. Hysteresis to prevent rapid cycling

## Implementation Details

### Files Structure
```
├── config.h           // Configuration constants
├── message_types.h    // CAN message definitions
├── fan_controller.h   // PID control implementation
├── gpio_manager.h     // PWM and GPIO handling
├── fan_safety.h       // Safety features
├── can_gateway.h      // CAN message handling
└── README.md         // This documentation
```

### Key Classes
1. `CANGateway`
   - Handles CAN message processing
   - Manages keypad state
   - Controls LED feedback
   - Implements debouncing

2. `FanController`
   - Implements PID control logic
   - Handles temperature trending
   - Manages fan speed calculations

3. `GPIOManager`
   - Controls PWM outputs
   - Handles pin initialization
   - Manages duty cycle mapping
   - Inverted/non-inverted output handling

4. `FanSafetyManager`
   - Implements safety features
   - Handles fault detection
   - Manages minimum run times
   - Soft start implementation

## Debug Output
The system provides detailed debug output via Serial:
```
Temp: 85°C (trend: 0.2°C/s), Error: 0, Output: 0.0, Fan: 50%
Pin 18 set to 50% (PWM: 128, mapped: 50, inverted: yes)
Pin 19 set to 50% (PWM: 128, mapped: 50, inverted: no)
Key 6 pressed: Fan control level 2
```

[Rest of the README remains the same...]

## License
Proprietary - All rights reserved

## Support
For support, please contact the author or raise an issue in the repository.

---
*Generated: 2025-03-24 01:48:56 UTC*  
*Author: veeriz*
