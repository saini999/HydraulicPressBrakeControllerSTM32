//Define User Functions and import Required Libs
#ifndef USR_FNC
    #define USR_FNC
    #include <PinDefines.h>
    #include <PositionGen.h>
    #include <ModbusInit.h>
    void handle_Y1Int();
    void handle_Y2Int();
    void setupModbus();
    void tasks();
    void setupTasks();
    void updatePositionGen();
    void setupEncoder();
    void updateEncoderToHMI();
    mode getMode();
    void updateMove();
    void getMachineData();
    //Init Position Generators
    PositionGen Y1pos;
    PositionGen Y2pos;
    //Init Position Encoders
    Encoder enc1(enc1a, enc1b, handle_Y1Int);
    Encoder enc2(enc2a, enc2b, handle_Y2Int);
    bool checkRef;
    mm y1ref, y2ref;
    mstate machineState;
    movstate movestate;
    mode machineMode;
    #include <HydraulicController.h>
    #include <HydraulicDriver.h>
    void handle_Y2Int () {
        enc2.update();
    }

    void handle_Y1Int () {
        enc1.update();
    }

    void setupEncoder() {
        //My Linear Scale Resolution 0.005mm
        enc1.setResolution(0.005);
        enc2.setResolution(0.005);
    }

    void updatePositionGen(){
        Y1pos.setAngle(getY1Angle);
        Y2pos.setAngle(getY2Angle);
        Y1pos.setAngleCorrection(getY1AngleCorrection);
        Y2pos.setAngleCorrection(getY2AngleCorrection);
        Y1pos.setDye(getDyeWidth);
        Y2pos.setDye(getDyeWidth);
        Y1pos.setSheetThickness(getSheetThicknessFromMb);
        Y2pos.setSheetThickness(getSheetThicknessFromMb);
    }

    void updateEncoderToHMI() {
        setY1PosInmm(enc1.getPos());
        setY2PosInmm(enc2.getPos());
        setY1PosInMicron(enc1.getPos());
        setY2PosInMicron(enc2.getPos());
    }

    void tasks(){
        mb.task();
        updateEncoderToHMI();
        updatePositionGen();
        getMachineData();
        updateMove();
    }

    void setupTasks(){
        //Serial.begin(9600*8);
        setupModbus();
        setupEncoder();
    }

    mode getMode(){
        return getJogMode ? MANUAL : getAutoMode ? AUTO : getRefMode ? REF : IDLE;
    }

    void getMachineData(){
        machineState = getState();
        movestate = getMove();
        machineMode = getMode();
    }
    void refY1(){
        enc1.setPos(y1ref);
        detachInterrupt(refY1Pin);
    }

    void refY2(){
        enc2.setPos(y2ref);
        detachInterrupt(refY2Pin);
    }
    
    void ref(){
        attachInterrupt(refY1Pin, refY1, HIGH);
        attachInterrupt(refY2Pin, refY2, HIGH);
    }



    void updateMove(){
        if(machineState != STATE_EMERGENCY){
            if(machineState == STATE_READY || STATE_RUNNING){
                if(machineMode == AUTO && (getStart0() || getStart)){
                    updatePositionGen();
                    autoMoveTo(Y1pos.getFinalPosition(), Y2pos.getFinalPosition(), getSheetWidth, getSheetThicknessFromMb, movestate);
                } else
                if(machineMode == MANUAL && movestate != NO_MOVE){
                    updatePositionGen();
                    manualMove(movestate);
                } else if(machineMode == REF && movestate != NO_MOVE){
                    updatePositionGen();
                    if(!checkRef){
                        ref();
                        checkRef = true;
                    }
                    manualMove(movestate,true);
                } else {
                    Y1.stopMove();
                    Y2.stopMove();
                }
            }
        }
    }

#endif