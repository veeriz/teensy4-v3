#ifndef CONFIG_H
#define CONFIG_H

#include <FlexCAN_T4.h>
#include <Arduino.h>
#include <unordered_map>
#include <vector>

// CAN bus configuration
const int CAN_BAUDRATE = 500000;  // 500kbps
const int DEBOUNCE_DELAY = 50;    // Base debounce delay in milliseconds

// GPIO setup for PWM
const int PWM_FREQUENCY = 100;    // PWM frequency in Hz for fan control

// Duty cycle mapping for multi-level keys
const int DUTY_CYCLE_MAP[] = {0, 25, 50, 75, 100};

// LED color definitions (0x00-0x09)
const int LED_COLORS[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};

// LED state definitions
const int LED_STATES[] = {0x00, 0x01, 0x02, 0x03};

// Fan control constants
const int MIN_PWM = 5;    // 5% duty cycle = full speed
const int MAX_PWM = 90;   // 90% duty cycle = barely moving/stopped
const int FAN_TEMP_THRESHOLD = 100;    // Temperature threshold in Celsius
const int FAN_STARTUP_DUTY = 50;       // Initial duty cycle for fan startup
const int FAN_RAMP_TIME = 2000;        // Time in ms to ramp up to target speed
const int FAN_MIN_RUN_TIME = 30000;    // Minimum run time in ms
const int FAN_FAULT_THRESHOLD = 3;     // Number of failed starts before fault

#endif // CONFIG_H