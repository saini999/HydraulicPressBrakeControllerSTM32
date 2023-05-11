// Add Modbus Definitions and Include Modbus Lib
#ifndef modbusDef
    //#define DEBUG_MODE
    #define modbusDef
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
    #define setY1PosInmm(a) mb.Ireg(0, (int)floor(a)) //Update Y1 Position in MM to HMI #00000 + 30001
    #define setY2PosInmm(a) mb.Ireg(1, (int)floor(a)) //Update Y2 Position in MM #00001 + 30001

    //Updating Float Position by sending seprate MM and Micron as MM.Micron i.e 5.015

    #define setY1PosInMicron(a) mb.Ireg(2, (int)((a - floor(a))*1000)) //Update Y2 Position in Microns #00002 + 30001
    #define setY2PosInMicron(a) mb.Ireg(3, (int)((a - floor(a))*1000)) //Update Y2 Position in Microns #00003 + 30001

    //Coils
    #define getAutoMode mb.Coil(0) // if true Machine is in Auto Mode ------|
    #define getJogMode mb.Coil(1) // if true Machine is in Manual Mode-----| if Both false Machine in Idle mode
    #define getRefMode mb.Coil(2) //Ref Coil
    #define getStart mb.Coil(3) //Started??
    #define getStop mb.Coil(4) //Stopped??
    #define getReset mb.Coil(5) //Reset
    #define getEmergency mb.Coil(6)

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

#endif