#ifndef IOController_h
    #define IOController_h
    #include <ModbusInit.h>
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
    enum mode {
        AUTO,
        MANUAL,
        IDLE,
        REF
    };
    mstate _mstate;
    movstate _movstate;
    bool getEmergency0(){
        return !digitalRead(emgPin); //Assuming Emergency is set when pin reaches GND
    }

    bool getReset0(){
        return digitalRead(resetPin);
    }

    bool getStart0(){
        return digitalRead(startPin);
    }

    bool getStop0(){
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
        if(getEmergency0() || getEmergency)
        {
            _mstate = STATE_EMERGENCY;
            return STATE_EMERGENCY;
        } else
        if((getStop0() || getStop || getReset0() || getReset) && (!getEmergency0() || !getEmergency))
        {
            _mstate = STATE_READY;
            return STATE_READY;
        } else
        if(getStart0() || getStart && _mstate == STATE_READY)
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