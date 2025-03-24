#ifndef CAN_GATEWAY_H
#define CAN_GATEWAY_H

#include "../src/config.h"
#include "../src/message_types.h"
#include "can_objects.h"
#include "gpio_manager.h"
#include "fan_safety.h"
#include "message_processor.h"

class CANBusGateway {
public:
    CANBusGateway(const CANObject* can_objects, size_t object_count, 
                  GPIOManager& gpio_manager) {
        CANBusGateway::can_objects = can_objects;
        CANBusGateway::object_count = object_count;
        CANBusGateway::gpio_manager = &gpio_manager;
        
        // Enhanced CAN initialization
        Can0.setBaudRate(CAN_BAUDRATE);
        Can0.setMaxMB(16);
        Can0.enableFIFO();
        Can0.enableFIFOInterrupt();
        Can0.onReceive(CANBusGateway::handle_message);
        
        // Set up message filters - Updated to include Start Key (0x07B) - Updated to include Start Key (0x07B)
        Can0.setMBFilter(REJECT_ALL);
        Can0.setMBFilter(MB0, 0x18EFFF21); // BlinkMarine
        Can0.setMBFilter(MB1, 0x07B);      // StartKey
        Can0.setMBFilter(MB2, 0x7C);       // MaxxECU
        Can0.setMBFilter(MB3, 0x7D);       // MaxxECU
        Can0.setMBFilter(MB4, 0x7F);       // MaxxECU
        
        // Error handling
        Can0.onError([]() {
            static uint32_t error_count = 0;
            if (++error_count > CAN_ERROR_THRESHOLD) {
                Serial.println("CAN Error threshold exceeded - Resetting");n("CAN Error threshold exceeded - Resetting");
                Can0.reset();
                error_count = 0;   error_count = 0;
            } }
        });});
        
        Serial.println("CANBusGateway initialized");Serial.println("CANBusGateway initialized");
        
        // Initialize states
        for (int i = 0; i < 256; i++) {++) {
            last_states[i] = 0xFF;
            last_received_times[i] = 0;times[i] = 0;
            states[i] = 0;   states[i] = 0;
        }   }
    }    }

    void run() {
        while (true) {
            Can0.events();
            check_can_bus_status();us_status();
            delay(100);   delay(100);
        }   }
    }    }

private:
    static const CANObject* can_objects;_objects;
    static size_t object_count;
    static GPIOManager* gpio_manager;o_manager;
    static int states[256];
    static uint8_t last_states[256];
    static unsigned long last_received_times[256];times[256];
    static FanSafetyManager fan_safety;    static FanSafetyManager fan_safety;

    static void handle_message(const CAN_message_t& message) {
        Serial.printf("Received CAN message: ID=0x%X, Len=%d, Data=", D=0x%X, Len=%d, Data=", 
                     message.id, message.len);
        for (int i = 0; i < message.len; i++) {
            Serial.printf("%02X ", message.buf[i]);   Serial.printf("%02X ", message.buf[i]);
        }
        Serial.println();        Serial.println();

        if (message.flags.extended) {
            handle_blink_marine_message(message);le_blink_marine_message(message);
        } else {
            handle_maxxecu_message(message);   handle_maxxecu_message(message);
        }   }
    }    }

    static void handle_blink_marine_message(const CAN_message_t& message) {sage(const CAN_message_t& message) {
        if (message.id == 0x18EFFF21) {
            uint8_t key_number = message.buf[3];;
            uint8_t key_state = message.buf[4];uint8_t key_state = message.buf[4];
            
            const CANObject& obj = find_can_object(key_number);const CANObject& obj = find_can_object(key_number);
            
            // Check if key is enabledenabled
            if (!obj.enabled) {
                Serial.printf("Key %d is disabled, ignoring message\n", key_number);printf("Key %d is disabled, ignoring message\n", key_number);
                return;   return;
            }            }

            if (is_valid_key_number(key_number) && is_valid_key_state(key_state)) {er) && is_valid_key_state(key_state)) {
                process_key(obj, key_state);   process_key(obj, key_state);
            }   }
        }   }
    }    }

    static void handle_maxxecu_message(const CAN_message_t& message) {(const CAN_message_t& message) {
        // Find matching configuration
        for (const auto& config : MAXXECU_CONFIGS) {CONFIGS) {
            if (message.id == config.id) {
                // Validate message length
                if (message.len < config.data_length) {
                    Serial.printf("Error: Message too short for ID 0x%X. Expected %d, got %d bytes\n",cted %d, got %d bytes\n",
                                message.id, config.data_length, message.len);     message.id, config.data_length, message.len);
                    return;   return;
                }                }

                // Process message using the new processor
                MessageProcessor::process_maxxecu_message(message, config);                MessageProcessor::process_maxxecu_message(message, config);

                // Handle specific message typesssage types
                switch (message.id) {
                    case 0x7C: {  // Boost Control
                        uint16_t boost_value = MessageProcessor::extract_value<uint16_t>(xtract_value<uint16_t>(
                            message.buf, 0, config.little_endian);
                        MessageProcessor::handle_boost_control(boost_value);eProcessor::handle_boost_control(boost_value);
                        break;   break;
                    }
                    case 0x7D: {  // Launch Control
                        uint16_t rpm = MessageProcessor::extract_value<uint16_t>(alue<uint16_t>(
                            message.buf, 0, config.little_endian);
                        uint16_t timer = MessageProcessor::extract_value<uint16_t>(_value<uint16_t>(
                            message.buf, 2, config.little_endian);
                        MessageProcessor::handle_launch_control(rpm, timer);eProcessor::handle_launch_control(rpm, timer);
                        break;   break;
                    }
                    case 0x7F: {  // Engine Temperature
                        int16_t temp = MessageProcessor::extract_value<int16_t>(alue<int16_t>(
                            message.buf, 0, config.little_endian);
                        MessageProcessor::handle_engine_temp(temp);eProcessor::handle_engine_temp(temp);
                        break;   break;
                    }   }
                }
                return;   return;
            }   }
        }
        Serial.printf("Unknown MaxxECU message ID: 0x%X\n", message.id);   Serial.printf("Unknown MaxxECU message ID: 0x%X\n", message.id);
    }    }

    static void process_key(const CANObject& obj, uint8_t key_state) {
        Serial.printf("Processing key: %s with state: %d\n", obj.name, key_state);        Serial.printf("Processing key: %s with state: %d\n", obj.name, key_state);

        if (strcmp(obj.mode, "momentary") == 0) {
            handle_momentary_key(obj, key_state);
        } else if (strcmp(obj.mode, "latch") == 0) {== 0) {
            handle_latch_key(obj, key_state);   handle_latch_key(obj, key_state);
        }   }
    }    }

    static void handle_momentary_key(const CANObject& obj, uint8_t key_state) {omentary_key(const CANObject& obj, uint8_t key_state) {
        // Set LED color
        set_led_color(obj.key_number, 
                     key_state ? obj.led_colors[1] : obj.led_colors[0], j.led_colors[1] : obj.led_colors[0], 
                     key_state, 0);                     key_state, 0);

        // Forward message if configuredd
        if (obj.forward_address != 0) {s != 0) {
            CAN_message_t msg;
            msg.id = obj.forward_address;
            msg.flags.extended = false;  // MaxxECU uses standard framestended = false;  // MaxxECU uses standard frames
            msg.len = 1;
            msg.buf[0] = key_state ? obj.forward_messages[1][0] : obj.forward_messages[0][0];msg.buf[0] = key_state ? obj.forward_messages[1][0] : obj.forward_messages[0][0];
            
            if (Can0.write(msg)) {
                Serial.printf("Forwarded message to 0x%X with data: 0x%X\n",  to 0x%X with data: 0x%X\n", 
                            msg.id, msg.buf[0]);        msg.id, msg.buf[0]);
            } else {
                Serial.println("Failed to forward message");   Serial.println("Failed to forward message");
            }   }
        }   }
    }    }

    static void handle_latch_key(const CANObject& obj, uint8_t key_state) {(const CANObject& obj, uint8_t key_state) {
        if (key_state == 0x01) {
            int state = (states[obj.key_number] + 1) % (obj.levels + 1);ber] + 1) % (obj.levels + 1);
            states[obj.key_number] = state;
            set_led_color(obj.key_number, obj.led_colors[state], ed_colors[state], 
                         state > 0 ? 1 : 0, 0);                         state > 0 ? 1 : 0, 0);

            if (strcmp(obj.name, "CoolingFan") == 0) {
                int duty_cycle = DUTY_CYCLE_MAP[state];
                for (int j = 0; j < obj.pwm_pin_count; j++) {
                    fan_safety.set_fan_speed(obj.pwm_pins[j], duty_cycle, true);   fan_safety.set_fan_speed(obj.pwm_pins[j], duty_cycle, true);
                }
            } else {
                if (obj.forward_address != 0) {s != 0) {
                    CAN_message_t msg;
                    msg.id = obj.forward_address;s;
                    msg.flags.extended = false;tended = false;
                    msg.len = 1;
                    msg.buf[0] = obj.forward_messages[state][0];msg.buf[0] = obj.forward_messages[state][0];
                    
                    if (Can0.write(msg)) {
                        Serial.printf("Forwarded latch message to 0x%X with data: 0x%X\n", essage to 0x%X with data: 0x%X\n", 
                                    msg.id, msg.buf[0]);        msg.id, msg.buf[0]);
                    } else {
                        Serial.println("Failed to forward latch message");   Serial.println("Failed to forward latch message");
                    }   }
                }                }

                if (obj.pwm_pin_count > 0) {
                    int duty_cycle = DUTY_CYCLE_MAP[state];
                    for (int j = 0; j < obj.pwm_pin_count; j++) {
                        gpio_manager->set_duty_cycle(obj.pwm_pins[j], duty_cycle,ty_cycle,
                                                  obj.invert_duty_cycle,
                                                  obj.min_pwm, obj.max_pwm);                             obj.min_pwm, obj.max_pwm);
                    }   }
                }   }
            }   }
        }   }
    }    }

    static void set_led_color(int key_number, int color, int state, int secondary_color) {lor(int key_number, int color, int state, int secondary_color) {
        CAN_message_t msg;
        msg.id = 0x18EF2100;
        msg.flags.extended = 1;tended = 1;
        msg.len = 8;
        msg.buf[0] = 0x04;
        msg.buf[1] = 0x1B;
        msg.buf[2] = 0x01;
        msg.buf[3] = key_number;mber;
        msg.buf[4] = color;
        msg.buf[5] = state;
        msg.buf[6] = secondary_color;dary_color;
        msg.buf[7] = 0xFF;        msg.buf[7] = 0xFF;

        if (Can0.write(msg)) {
            Serial.printf("Set LED for key %d: color=%d, state=%d\n", r=%d, state=%d\n", 
                         key_number, color, state);         key_number, color, state);
        } else {
            Serial.println("Failed to send LED command");   Serial.println("Failed to send LED command");
        }   }
    }    }

    static void check_can_bus_status() {{
        CAN_error_t err = Can0.error();.error();
        if (err != ERROR_OK) {
            Serial.printf("CAN error detected: %d\n", err);"CAN error detected: %d\n", err);
            restart_CAN();   restart_CAN();
        }   }
    }    }

    static void restart_CAN() {
        Serial.println("Restarting CAN bus...");n("Restarting CAN bus...");
        Can0.begin();
        Can0.setBaudRate(CAN_BAUDRATE);AN_BAUDRATE);
        Can0.setMaxMB(16);
        Can0.enableFIFO();
        Can0.enableFIFOInterrupt();
        Can0.onReceive(CANBusGateway::handle_message);_message);
        Serial.println("CAN bus restarted");   Serial.println("CAN bus restarted");
    }    }

    static bool is_valid_key_number(uint8_t key_number) {ber) {
        for (size_t i = 0; i < object_count; i++) {
            if (can_objects[i].key_number == key_number) {i].key_number == key_number) {
                return true;   return true;
            }   }
        }
        return false;   return false;
    }    }

    static bool is_valid_key_state(uint8_t key_state) {
        return (key_state == 0x00 || key_state == 0x01);   return (key_state == 0x00 || key_state == 0x01);
    }    }

    static const CANObject& find_can_object(uint8_t key_number) { key_number) {
        for (size_t i = 0; i < object_count; i++) {
            if (can_objects[i].key_number == key_number) {ber == key_number) {
                return can_objects[i];   return can_objects[i];
            }   }
        }
        static const CANObject default_object = {
            "Default", "binary", 1, 0xFF, "momentary", {0x00}, 0, {0}, 0,tary", {0x00}, 0, {0}, 0,
            {{0x00, 0x00}}, false, 0, 100, false  {{0x00, 0x00}}, false, 0, 100, false
        };
        return default_object;   return default_object;
    }  }
};};


#endif // CAN_GATEWAY_H#endif // CAN_GATEWAY_H