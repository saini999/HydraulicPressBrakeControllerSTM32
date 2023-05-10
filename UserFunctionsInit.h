//Define User Functions and import Required Libs
#ifndef USR_FNC
    #define USR_FNC
    #include <Encoder.h>
    #include <PositionGen.h>
    #include <PinDefines.h>
    #include <ModbusInit.h>
    #include <IOController.h>
    void handle_Y1Int();
    void handle_Y2Int();
    void setupModbus();
    void tasks();
    void setupTasks();
    void updatePositionGen();
    void setupEncoder();
    void updateEncoderToHMI();
    //Init Position Generators
    PositionGen Y1pos;
    PositionGen Y2pos;

    //Init Position Encoders
    Encoder enc1(enc1a, enc1b, handle_Y1Int);
    Encoder enc2(enc2a, enc2b, handle_Y2Int);

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
    }

    void setupTasks(){
        //Serial.begin(9600*8);
        setupModbus();
        setupEncoder();
    }
#endif