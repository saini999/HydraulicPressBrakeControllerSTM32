#ifndef HydraulicController_h
    #define HydraulicController_h
    #include <Arduino.h>
        enum mstate {
        STATE_READY,
        STATE_RUNNING,
        STATE_EMERGENCY,
        STATE_ERROR
    };

    enum movstate {
        MOVE_DOWN,
        MOVE_UP,
        NO_MOVE
    };
    enum mode {
        AUTO,
        MANUAL,
        IDLE,
        REF
    };
    class HydraulicController {
        public:
            HydraulicController(byte pinUp, byte pinDown, byte pressurePin, byte pressureENPin);
            void setSpeed(uint8_t speed, uint8_t pressure, movstate movestate = NO_MOVE, bool isBending = false);
            void stopMove();
        private:
            byte _pinUp, _pinDown, _pressurePin, _pressureENPin;
    };

#endif