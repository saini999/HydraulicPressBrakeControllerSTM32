#ifndef Encoder_h
#define Encoder_h

#include <Arduino.h>

typedef float mm;

//Encoder Class
class Encoder {
    public:
        //Initialize Encoder with Pin A, Pin B and a callback function which calls update
        Encoder(uint8_t pinA, uint8_t pinB, callback_function_t callback);
        //Returns Current Encoder Position
        int32_t read(void);
        //Set Encoder Position
        void write(int32_t pos);
        //Updates Encoder Postion on Interrupt
        void update(void);
        //SetEncoder Resolution
        void setResolution(mm res);
        //Get Encoder Resolution
        mm getResolution(void);
        //Set Position Correction
        void setCorrection(mm corr);
        //Get Position Correction
        mm getCorrection(void);
        //GetMovement in MM
        mm getPos(void);
    private:
        uint32_t _position;
        uint8_t _pinA;
        uint8_t _pinB;
        uint8_t _state;
        callback_function_t _callback;
        mm _res;
        mm _corr;
};
#endif