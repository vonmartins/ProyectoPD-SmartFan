#ifndef STRUCTS_H
#define STRUCTS_H

    #include <stdint.h>
    #include "config.h"

    enum DeviceState {
        LOCKED,
        UNLOCKED
    };

    typedef struct {
        const uint8_t PIN;
        bool detection;        
    }PIRSensor;

    typedef struct {
        const uint8_t PIN;
        uint8_t debounceTime;
    } Button;

    typedef enum {
        MESSAGE_TYPE_INTEGER,
        MESSAGE_TYPE_DECIMAL,
        MESSAGE_TYPE_STRING,
        MESSAGE_TYPE_STRING_INTEGER,
        MESSAGE_TYPE_STRING_DECIMAL
    } MessageType;

    typedef struct {
        MessageType type;
        float decimal;
        uint16_t integer;
        char text[MAX_STRING_LENGTH];
    } OLEDMessage;

    typedef struct {
        uint8_t pin1;
        uint8_t pin2;
        uint8_t enablePin;
        uint8_t pwmChannel;
        uint16_t frequency;
        uint8_t resolution;
        uint8_t dutyCycle;
        uint8_t speed;
        bool active;
    } DCMotor;

#endif
