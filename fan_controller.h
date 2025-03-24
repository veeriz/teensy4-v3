#ifndef FAN_CONTROLLER_H
#define FAN_CONTROLLER_H

#include <Arduino.h>

class FanController {
public:
    FanController() {
        reset();
    }

    void reset() {
        last_error = 0;
        integral = 0;
        last_update = 0;
    }

    // Calculate fan speed based on temperature
    int calculate_fan_speed(int current_temp) {
        const int TARGET_TEMP = 85;  // Target temperature in Celsius
        const float KP = 2.0;        // Proportional gain
        const float KI = 0.1;        // Integral gain
        const float KD = 1.0;        // Derivative gain
        
        unsigned long current_time = millis();
        float dt = (current_time - last_update) / 1000.0;  // Convert to seconds
        
        if (last_update == 0) {
            dt = 0.1;  // First run
        }

        // Calculate error
        int error = current_temp - TARGET_TEMP;
        
        // Calculate integral with anti-windup
        integral = constrain(integral + (error * dt), -50, 50);
        
        // Calculate derivative
        float derivative = 0;
        if (dt > 0) {
            derivative = (error - last_error) / dt;
        }
        
        // Calculate PID output
        float output = (KP * error) + (KI * integral) + (KD * derivative);
        
        // Convert to fan speed percentage (0-100)
        int fan_speed = constrain(map(output, -50, 50, 0, 100), 0, 100);
        
        // Store values for next iteration
        last_error = error;
        last_update = current_time;
        
        Serial.printf("Temp: %d°C, Error: %d, Output: %.1f, Fan Speed: %d%%\n",
                     current_temp, error, output, fan_speed);
                     
        return fan_speed;
    }

private:
    int last_error;
    float integral;
    unsigned long last_update;
};

#endif // FAN_CONTROLLER_H