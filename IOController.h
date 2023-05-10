#ifndef IOController_h
    #define IOController_h
    #include <Arduino.h>
    #include <PinDefines.h>
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
    mstate _mstate;
    movstate _movstate;
    bool getEmergency(){
        return !digitalRead(emgPin); //Assuming Emergency is set when pin reaches GND
    }

    bool getReset(){
        return digitalRead(resetPin);
    }

    bool getStart(){
        return digitalRead(startPin);
    }

    bool getStop(){
        return digitalRead(stopPin);
    }

    movstate getMove(){
        if(digitalRead(footDwnpin)){
            return MOVE_DOWN;
        } else if (digitalRead(footUpPin)){
            return MOVE_UP;
        } else {
            return NO_MOVE;
        }
    }

    mstate getState(){
        if(getEmergency())
        {
            _mstate = STATE_EMERGENCY;
            return STATE_EMERGENCY;
        } else
        if((getStop() || getReset()) && !getEmergency())
        {
            _mstate = STATE_READY;
            return STATE_READY;
        } else
        if(getStart() && _mstate == STATE_READY)
        {
            _mstate = STATE_RUNNING;
            return STATE_RUNNING;
        }
        else 
        {
            _mstate = STATE_ERROR;
            return STATE_ERROR;
        }
    }
#endif