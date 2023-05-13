#ifndef IOController_h
    #define IOController_h
    mstate _mstate;
    movstate _movstate;
    void setupIO() {
        pinMode(LED_D2, OUTPUT);
        pinMode(emgPin, INPUT);
        pinMode(resetPin, INPUT);
        pinMode(startPin, INPUT);
        pinMode(stopPin, INPUT);
        pinMode(footDwnpin, INPUT);
        pinMode(footUpPin, INPUT);
        pinMode(enc1a, INPUT_PULLUP);
        pinMode(enc2a, INPUT_PULLUP);
        pinMode(enc1b, INPUT_PULLUP);
        pinMode(enc2b, INPUT_PULLUP);

    }
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