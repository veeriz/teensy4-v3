#ifndef FAN_SAFETY_H
#define FAN_SAFETY_H

#include <Arduino.h>
#include "config.h"

class FanSafetyManager {
public:
    void set_fan_speed(int pin, int duty_cycle, bool invert) {
        if (invert) {
            duty_cycle = 100 - duty_cycle;
        }
        
        // Store the target duty cycle
        int target_pwm = (duty_cycle * 255) / 100;
        
        // If starting from off
        if (current_speeds[pin] == (invert ? 255 : 0) && duty_cycle > 0) {
            // Full power start
            analogWrite(pin, invert ? 0 : 255);
            delay(FAN_STARTUP_DELAY);
            
            // Ramp to target speed
            int current_pwm = invert ? 0 : 255;
            int step = invert ? 1 : -1;
            
            while ((invert && current_pwm < target_pwm) || 
                   (!invert && current_pwm > target_pwm)) {
                current_pwm += step;
                analogWrite(pin, current_pwm);
                delay(5);  // Smooth ramping
            }
        } else {
            // Direct set if already running
            analogWrite(pin, target_pwm);
        }
        
        // Store current speed
        current_speeds[pin] = target_pwm;
        
        Serial.printf("Fan on pin %d set to %d%% (PWM: %d, inverted: %s)\n",
                     pin, duty_cycle, target_pwm, invert ? "yes" : "no");
    }

private:
    static const int FAN_STARTUP_DELAY = 250;  // ms
    int current_speeds[54] = {0};  // Store current speed for each possible pin
};

#endif // FAN_SAFETY_H