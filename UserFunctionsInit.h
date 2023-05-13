//Define User Functions and import Required Libs
#ifndef USR_FNC
    #define USR_FNC
    #include <PinDefines.h>
    #include <Encoder.h>
    #include <HydraulicController.h>
    #define NUM_DISCRETE_INPUT 100
    #define NUM_INPUT_REGISTERS 100
    #define NUM_COILS 100
    #define NUM_HOLDING_REGISTERS 100
    #include <ModbusSerial.h>
    //Init Modbus
    ModbusSerial mb;
    //Change Modbus Registers to Readable Variables
    #define getY1Angle mb.Hreg(0) //Angle set by HMI/Modbus Master GUI #00000 + 40001
    #define getY2Angle mb.Hreg(1) //Angle set by HMI/Modbus Master GUI #00001 + 40001
    #define getY1AngleCorrection mb.Hreg(2) //AngleCorrection #00002 + 40001
    #define getY2AngleCorrection mb.Hreg(3) //AngleCorrection #00003 + 40001
    #define getSheetThicknessFromMb mb.Hreg(4) //Sheet Thickness #00004 + 40001
    #define getDyeWidth mb.Hreg(5) //V Dye Width #00005 + 40001
    #define getSheetWidth mb.Hreg(6) //Get Sheet Width for Pressure Calc
    #define getCustomY1PosInmm mb.Hreg(7)
    #define getCustomY2PosInmm mb.Hreg(8)
    #define getCustomY1PosInMicron mb.Hreg(9)
    #define getCustomY2PosInMicron mb.Hreg(10)
    #define getSoftLmt mb.Hreg(11)
    #define setY1PosInmm(a) mb.Ireg(0, (int)ceil(a)) //Update Y1 Position in MM to HMI #00000 + 30001
    #define setY2PosInmm(a) mb.Ireg(1, (int)ceil(a)) //Update Y2 Position in MM #00001 + 30001

    //Updating Float Position by sending seprate MM and Micron as MM.Micron i.e 5.015

    #define setY1PosInMicron(a) mb.Ireg(2, (int)((a - (int)a)*1000)) //Update Y2 Position in Microns #00002 + 30001
    #define setY2PosInMicron(a) mb.Ireg(3, (int)((a - (int)a)*1000)) //Update Y2 Position in Microns #00003 + 30001

    //Coils
    #define getAutoMode mb.Coil(0) // if true Machine is in Auto Mode ------|
    #define setAutoMode(a) mb.Coil(0, a) // if true Machine is in Auto Mode ------|
    #define getJogMode mb.Coil(1) // if true Machine is in Manual Mode-----| if Both false Machine in Idle mode
    #define setJogMode(a) mb.Coil(1, a) // if true Machine is in Manual Mode-----| if Both false Machine in Idle mode
    #define getRefMode mb.Coil(2) //Ref Coil
    #define setRefMode(a) mb.Coil(2, a) //Ref Coil
    #define getStart mb.Coil(3) //Started??
    #define setStart(a) mb.Coil(3, a) //Started??
    #define getStop mb.Coil(4) //Stopped??
    #define setStop(a) mb.Coil(4, a) //Stopped??
    #define getReset mb.Coil(5) //Reset
    #define setReset(a) mb.Coil(5, a) //Reset
    #define getEmergency mb.Coil(6)
    #define setEmergency(a) mb.Coil(6, a)
    #define setCustomPos mb.Coil(7)

    void setupModbus(){
        mb.config(&Serial,115200, -1);
        mb.setSlaveId(1);
        for (int i = 0; i < NUM_DISCRETE_INPUT; ++i) 
        {
            mb.addIsts(i);
        }
        for (int i = 0; i < NUM_INPUT_REGISTERS; ++i) 
        {
            mb.addIreg(i);
        }
        for (int i = 0; i < NUM_COILS; ++i) 
        {
            mb.addCoil(i);
        }
        for (int i = 0; i < NUM_HOLDING_REGISTERS; ++i) 
        {
            mb.addHreg(i);
        }
    }
    bool checkRef;
    mm y1ref, y2ref;
    #include <IOController.h>
    mstate machineState;
    movstate movestate;
    mode machineMode;
    mm softLimit;
    mm y1pos, y2pos;
    mm maxUnbalance, minUnbalance;
    mm slowDownPos, uSlowDownPos1, uSlowDownPos2, slowUpPos1, slowUpPos2;
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
    #include <PositionGen.h>
    //#include <HydraulicController.h>
    //Init Position Generators
    PositionGen Y1pos;
    PositionGen Y2pos;
    //Init Position Encoders
    Encoder enc1(enc1a, enc1b/*, handle_Y1Int*/);
    Encoder enc2(enc2a, enc2b/*, handle_Y2Int*/);

    #define UFAST 200
    #define FAST 127
    #define SLOW 60
    #define BEND 20
    #define slowup 20
    HydraulicController Y1(y1up, y1dn, pressurePin0, pressureENPin0);
    HydraulicController Y2(y2up, y2dn, pressurePin0, pressureENPin0);

    

    void getCurrentPos(){
        y1pos = enc1.getPos();
        y2pos = enc2.getPos();
    }

    void setSoftLimit(){
        softLimit = getSoftLmt;
    }

    mm getSoftLimit(){
        return softLimit;
    }

    mm difference(mm y1loc, mm y2loc){
        return y1loc - y2loc;
    }

    uint8_t calculatePressure(mm sheetWidth, mm sheetThickness){
        //setMax Pressure 255 when sheet width is 3200mm  and thicknes 10mm
        int maxWidth = 3200;
        int minSheetWidth = 2400;
        int maxThickness = 10;
        int minSheetThickness = 1;
        float k = 255/(maxWidth*maxThickness);
        sheetWidth = minSheetWidth > sheetWidth ? minSheetWidth : sheetWidth;
        sheetThickness = minSheetThickness > sheetThickness ? minSheetThickness : sheetThickness;
        return (uint8_t)(k*(sheetWidth*sheetThickness));
    }


    void manualMove(movstate movestate, bool isRef = false){
        getCurrentPos();
        uint8_t speed = SLOW;
        uint8_t y1speed, y2speed;
        mm diff = difference(y1pos, y2pos);
        float k  = diff/(maxUnbalance/speed);
        if(k < 0){
            k = k * -1;
        }
        switch(movestate){
            case MOVE_UP:
                if(diff < 0) {
                    y1speed = speed + k;
                    y2speed = speed - k;
                } else {
                    y1speed = speed - k;
                    y2speed = speed + k;
                }
                if(y1pos > softLimit || isRef) {
                    Y1.setSpeed(y1speed,127,movestate,true);
                } else {Y1.stopMove();}
                if(y2pos > softLimit || isRef) {
                    Y2.setSpeed(y2speed,127,movestate,true);
                } else {Y2.stopMove();}
                break;
            case MOVE_DOWN:
                if(diff < 0) {
                    y1speed = speed - k;
                    y2speed = speed + k;
                } else {
                    y1speed = speed + k;
                    y2speed = speed - k;
                }
                Y1.setSpeed(y1speed,127,movestate,true);
                Y2.setSpeed(y2speed,127,movestate,true);
                break;
            case NO_MOVE:
                Y1.stopMove();
                Y2.stopMove();
                break;
        }
        
    }

    void autoMoveTo(mm y1loc, mm y2loc, mm sheetWidth, mm sheetThickness, movstate movestate0){
        getCurrentPos();
        uint8_t speed1, speed2;
        uint8_t y1speed, y2speed;
        bool isBendingSheet;
        if(movestate0 == MOVE_DOWN){
            slowDownPos = 1.5 * sheetThickness;
            uSlowDownPos1 = y1loc + (1.5*sheetThickness);
            uSlowDownPos2 = y2loc + (1.5*sheetThickness);
            isBendingSheet = slowDownPos > y1loc || slowDownPos > y2loc ? true : false;
            speed1 = slowDownPos > y1loc ? SLOW : FAST;
            speed1 = uSlowDownPos1 > y1loc ? BEND : speed1;
            speed2 = slowDownPos > y2loc ? SLOW : FAST;
            speed2 = uSlowDownPos2 > y2loc ? BEND : speed2;
        } else if(movestate0 == MOVE_UP){
            slowUpPos1 = y1ref - slowup;
            slowUpPos2 = y2ref - slowup;
            speed1 = slowUpPos1 < y1loc ? SLOW : FAST;
            speed2 = slowUpPos2 < y2loc ? SLOW : FAST;
        }
        mm diff = difference(y1pos, y2pos); 
        if(diff < 0) {
            mm tdiff = diff * -1;
            if(tdiff > maxUnbalance){
                setEmergency(true);
            }
        } else if (diff > maxUnbalance) {
            setEmergency(true);
        }
        float k1 = diff/(maxUnbalance/speed1); //50% of max analog Resolution
        float k2 = diff/(maxUnbalance/speed2); //50% of max analog Resolution
        if(k1 < 0) {
            k1 = k1 * -1;
        }
        if(k2 < 0) {
            k2 = k2 * -1;
        }

        switch(movestate0){
            case MOVE_DOWN:
                if(diff < 0) {
                    y1speed = speed1 - k1;
                    y2speed = speed2 + k2;
                } else {
                    y1speed = speed1 + k1;
                    y2speed = speed2 - k2;
                }
                if(y1pos > y1loc) {
                    Y1.setSpeed(y1speed, calculatePressure(sheetWidth, sheetThickness), movestate0, isBendingSheet);
                } else {
                    Y1.stopMove();
                }
                if(y2pos > y2loc) {
                    Y2.setSpeed(y2speed, calculatePressure(sheetWidth, sheetThickness), movestate0, isBendingSheet);
                } else {
                    Y2.stopMove();
                }
                break;
            case MOVE_UP:
                if(diff < 0) {
                    y1speed = speed1 + k1;
                    y2speed = speed2 - k2;
                } else {
                    y1speed = speed1 - k1;
                    y2speed = speed2 + k2;
                }
                if(y1pos < getSoftLimit()) {
                    Y1.setSpeed(y1speed, calculatePressure(sheetWidth, sheetThickness), movestate0);
                } else {
                    Y1.stopMove();
                }
                if(y2pos < getSoftLimit()) {
                    Y2.setSpeed(y2speed, calculatePressure(sheetWidth, sheetThickness), movestate0);
                } else {
                    Y2.stopMove();
                }
                break;
            case NO_MOVE:
                Y1.stopMove();
                Y2.stopMove();
                break;
        }
    }

    void handle_Y2Int () {
        enc2.update();
    }

    void handle_Y1Int () {
        enc1.update();
    }

    void setupEncoder() {
        attachInterrupt(enc1a, handle_Y1Int, CHANGE);
        attachInterrupt(enc1b, handle_Y1Int, CHANGE);
        attachInterrupt(enc2a, handle_Y2Int, CHANGE);
        attachInterrupt(enc2b, handle_Y2Int, CHANGE);
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

    void updateCustomPos() {
        if(setCustomPos){
            mm pos1, pos2;
            pos1 = getCustomY1PosInmm;
            pos1 = pos1 + (getCustomY1PosInMicron/1000.0);
            enc1.setPos(pos1);
            pos2 = getCustomY2PosInmm;
            pos2 = pos2 + (getCustomY2PosInMicron/1000.0);
            enc2.setPos(pos2);
        }
    }

uint32_t t;
bool led;

void blink() {
    if(millis() - t > 500){
        digitalWrite(LED_D2, led);
        led = !led;
        t = millis();
    }
}

    void tasks(){
        mb.task();
        blink();
        getCurrentPos();
        updateEncoderToHMI();
        updatePositionGen();
        getMachineData();
        updateMove();
        updateCustomPos();
    }

    void setupTasks(){
        //Serial.begin(9600*8);
        setupIO();
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
        setEmergency(getEmergency0());
        setStart(getStart0());
        setStop(getStop0());
        setReset(getReset0());
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
        pinMode(refY1Pin, INPUT_PULLUP);
        pinMode(refY2Pin, INPUT_PULLUP);
        attachInterrupt(refY1Pin, refY1, LOW);
        attachInterrupt(refY2Pin, refY2, LOW);
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
                    if(getRefMode){
                        checkRef = false;
                    }
                    if(!checkRef){
                        ref();
                        setRefMode(true);
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