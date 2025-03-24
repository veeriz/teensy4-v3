#ifndef MESSAGE_PROCESSOR_H
#define MESSAGE_PROCESSOR_H

#include "message_types.h"
#include "fan_controller.h"

class MessageProcessor {
public:
    static FanController& get_fan_controller() {
        static FanController fan_controller;
        return fan_controller;
    }

    // For 16-bit temperature
    static void handle_engine_temp(int16_t raw_temp, GPIOManager* gpio_manager) {
        int temp_celsius = raw_temp / 10;  // Convert from 0.1°C to °C
        
        // Calculate required fan speed
        int fan_speed = get_fan_controller().calculate_fan_speed(temp_celsius);
        
        // Update both fans
        gpio_manager->set_duty_cycle(18, fan_speed, true, MIN_PWM, MAX_PWM);   // Inverted
        gpio_manager->set_duty_cycle(19, fan_speed, false, MIN_PWM, MAX_PWM);  // Normal
        
        Serial.printf("Engine Temp: %d°C, Fan Speed: %d%%\n", 
                     temp_celsius, fan_speed);
    }

    // For 8-bit temperature
    static void handle_engine_temp_8bit(int8_t raw_temp, GPIOManager* gpio_manager) {
        int temp_celsius = raw_temp;  // Direct use without decimal
        
        // Calculate required fan speed
        int fan_speed = get_fan_controller().calculate_fan_speed(temp_celsius);
        
        // Update both fans
        gpio_manager->set_duty_cycle(18, fan_speed, true, MIN_PWM, MAX_PWM);   // Inverted
        gpio_manager->set_duty_cycle(19, fan_speed, false, MIN_PWM, MAX_PWM);  // Normal
        
        Serial.printf("Engine Temp: %d°C, Fan Speed: %d%%\n", 
                     temp_celsius, fan_speed);
    }
};

#endif // MESSAGE_PROCESSOR_H