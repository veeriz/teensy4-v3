#ifndef GPIO_MANAGER_H
#define GPIO_MANAGER_H

#include <Arduino.h>
#include "can_objects.h"

class GPIOManager {
public:
    GPIOManager(int pwm_frequency) : pwm_frequency(pwm_frequency) {}

    void initialize(const CANObject* objects, size_t count) {
        // Configure PWM pins
        analogWriteFrequency(18, pwm_frequency);  // Fan 1 (inverted)
        analogWriteFrequency(19, pwm_frequency);  // Fan 2
        analogWriteResolution(8);
        
        // Initialize pins to off state
        analogWrite(18, 255);  // Inverted logic - 255 = OFF
        analogWrite(19, 0);    // Normal logic - 0 = OFF
        
        Serial.println("GPIO Manager initialized");
    }

    void set_duty_cycle(int pin, int duty_cycle, bool invert, int min_pwm, int max_pwm) {
        if (invert) {
            duty_cycle = 100 - duty_cycle;
        }
        
        int mapped_duty = map(duty_cycle, 0, 100, min_pwm, max_pwm);
        analogWrite(pin, (mapped_duty * 255) / 100);
        
        Serial.printf("Pin %d set to duty cycle %d%% (mapped: %d, inverted: %s)\n",
                     pin, duty_cycle, mapped_duty, invert ? "yes" : "no");
    }

private:
    int pwm_frequency;
};

#endif // GPIO_MANAGER_H