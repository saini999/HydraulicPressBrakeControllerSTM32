
// Add Modbus Definitions and Include Modbus Lib
#ifndef modbusDef
    //#define DEBUG_MODE
    #define modbusDef
    #define NUM_DISCRETE_INPUT 100
    #define NUM_INPUT_REGISTERS 100
    #define NUM_COILS 100
    #define NUM_HOLDING_REGISTERS 100
    #include <Modbus.h>
    #include <ModbusSerial.h>
    //Init Modbus
    ModbusSerial mb;
#endif

//Change Modbus Registers to Readable Variables
#ifndef mbusVar
    #define mbusVar
    #define getY1Angle mb.Hreg(0) //Angle set by HMI/Modbus Master GUI #00000 + 40001
    #define getY2Angle mb.Hreg(1) //Angle set by HMI/Modbus Master GUI #00001 + 40001
    #define getY1AngleCorrection mb.Hreg(2) //AngleCorrection #00002 + 40001
    #define getY2AngleCorrection mb.Hreg(3) //AngleCorrection #00003 + 40001
    #define getSheetThickness mb.Hreg(4) //Sheet Thickness #00004 + 40001
    #define getDyeWidth mb.Hreg(5) //V Dye Width #00005 + 40001
    #define setY1PosInmm(a) mb.Ireg(0, (int)floor(a)) //Update Y1 Position in MM to HMI #00000 + 30001
    #define setY2PosInmm(a) mb.Ireg(1, (int)floor(a)) //Update Y2 Position in MM #00001 + 30001
    
    //Updating Float Position by sending seprate MM and Micron as MM.Micron i.e 5.015

    #define setY1PosInMicron(a) mb.Ireg(2, (int)((a - floor(a))*1000)) //Update Y2 Position in Microns #00002 + 30001
    #define setY2PosInMicron(a) mb.Ireg(3, (int)((a - floor(a))*1000)) //Update Y2 Position in Microns #00003 + 30001

#endif

//Define Device I/O Pins
#ifndef DVC_IO
    #define DVC_IO
    #define enc1a PB0
    #define enc1b PB1
    #define enc2a PB2
    #define enc2b PB3
#endif

//Define User Functions and import Required Libs
#ifndef USR_FNC
    #define USR_FNC
    #include <Encoder.h> 
    #include <PositionGen.h>
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
#endif



void setup(){
    setupTasks();
}

void loop(){
    tasks();
}

void handle_Y2Int () {
    enc2.update();
}

void handle_Y1Int () {
    enc1.update();
}

void setupModbus(){
    mb.config(&Serial,9600*8, -1);
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

void updatePositionGen(){
    Y1pos.setAngle(getY1Angle);
    Y2pos.setAngle(getY2Angle);
    Y1pos.setAngleCorrection(getY1AngleCorrection);
    Y2pos.setAngleCorrection(getY2AngleCorrection);
    Y1pos.setDye(getDyeWidth);
    Y2pos.setDye(getDyeWidth);
    Y1pos.setSheetThickness(getSheetThickness);
    Y2pos.setSheetThickness(getSheetThickness);
}

void setupEncoder() {
    //My Linear Scale Resolution 0.005mm
    enc1.setResolution(0.005);
    enc2.setResolution(0.005);
}

void updateEncoderToHMI() {
    setY1PosInmm(enc1.getPos());
    setY2PosInmm(enc2.getPos());
    setY1PosInMicron(enc1.getPos());
    setY2PosInMicron(enc2.getPos());
}