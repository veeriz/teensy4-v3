#include <Arduino.h>
// Public APIs from include/
#include "../include/fan_controller.h"
#include "../include/gpio_manager.h"
#include "../include/fan_safety.h"
#include "../include/can_gateway.h"
// Internal headers from same directory
#include "config.h"
#include "message_types.h"

// Initialize static members
const CANObject* CANBusGateway::can_objects;
size_t CANBusGateway::object_count;
GPIOManager* CANBusGateway::gpio_manager;
int CANBusGateway::states[256] = {0};
uint8_t CANBusGateway::last_states[256];
unsigned long CANBusGateway::last_received_times[256];
FanSafetyManager CANBusGateway::fan_safety;

// FlexCAN instance with error callback
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can0;

// GPIO Manager instance
GPIOManager gpio_manager(PWM_FREQUENCY);

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000) {
        delay(10);
    }
    Serial.println("Starting setup...");
    
    gpio_manager.initialize(CAN_OBJECTS, sizeof(CAN_OBJECTS) / sizeof(CAN_OBJECTS[0]));
    
    Can0.begin();
    
    CANBusGateway gateway(CAN_OBJECTS, sizeof(CAN_OBJECTS) / sizeof(CAN_OBJECTS[0]), 
                         gpio_manager);
    
    Serial.println("Setup complete");
    gateway.run();
}

void loop() {
    // Empty loop since all operations are handled in the gateway's run method
}