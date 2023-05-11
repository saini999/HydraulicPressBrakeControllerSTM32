#include <PinDefines.h>
#include <Encoder.h>
#include <IOController.h>
#include <PositionGen.h>
#include <ModbusInit.h>
#include <HydraulicController.h>
#include <HydraulicDriver.h>
#include <UserFunctionsInit.h>

void setup(){
    setupTasks();
}

void loop(){
    tasks();
}