#include <HydraulicController.h>

HydraulicController::HydraulicController(byte pinUp, byte pinDown, byte pressurePin, byte pressureENPin) {
    _pinUp = pinUp;
    _pinDown = pinDown;
    _pressurePin = pressurePin;
    _pressureENPin = pressureENPin;
    pinMode(_pinUp, OUTPUT);
    pinMode(_pinDown, OUTPUT);
    pinMode(_pressurePin, OUTPUT);
    pinMode(_pressureENPin, OUTPUT);
    analogWrite(_pinUp, 0);
    analogWrite(_pinDown, 0);
    analogWrite(_pressurePin, 0);
    digitalWrite(_pressureENPin, LOW);
}

void HydraulicController::setSpeed(uint8_t speed, uint8_t pressure, movstate movestate, bool isBending){
    switch (movestate) {
        case MOVE_UP:
            analogWrite(_pinUp, speed);
            analogWrite(_pinDown, 0);
            analogWrite(_pressurePin, pressure);
            digitalWrite(_pressureENPin, isBending);
            break;
        case MOVE_DOWN:
            analogWrite(_pinUp, 0);
            analogWrite(_pinDown, speed);
            analogWrite(_pressurePin, pressure);
            digitalWrite(_pressureENPin, isBending);
            break;
        case NO_MOVE:
            analogWrite(_pinUp, 0);
            analogWrite(_pinDown, 0);
            analogWrite(_pressurePin, 0);
            digitalWrite(_pressureENPin, LOW);
            break;
    }
}

void HydraulicController::stopMove() {
    setSpeed(0, 0);
}