#ifndef Hydraulic_Driver_H
    #define Hydraulic_Driver_h

    #include <Encoder.h>//Import Encoder Lib
    #include <PositionGen.h>//Import Position Calculation Functions as a class(For Y1, Y2)
    #include <ModbusInit.h>//Mobus Defines
    #include <IOController.h>//ALL Inputs/Outputs Controller
    #include <HydraulicController.h>
    #include <UserFunctionsInit.h>

    #define UFAST 200
    #define FAST 127
    #define SLOW 60
    #define BEND 20
    #define slowup 20
    HydraulicController Y1(y1up, y1dn, pressurePin0, pressureENPin0);
    HydraulicController Y2(y2up, y2dn, pressurePin0, pressureENPin0);
    mm softLimit;
    mm y1pos, y2pos;
    mm maxUnbalance, minUnbalance;
    mm slowDownPos, uSlowDownPos1, uSlowDownPos2, slowUpPos1, slowUpPos2;
    

    void getCurrentPos(){
        y1pos = enc1.getPos();
        y2pos = enc2.getPos();
    }

    void setSoftLimit(mm softlmt){
        softLimit = softlmt;
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
        return (uint8_t)(k*sheetWidth*sheetThickness);
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

#endif