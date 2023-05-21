#ifndef PROGRAM_MANAGER_H

#define PROGRAM_MANAGER_H

    #ifndef HAL_SD_MODULE_ENABLED
    #define HAL_SD_MODULE_ENABLED
    #endif

#include <STM32SD.h>

struct benddb {
    int bendID;//1
    float y1angle; //#2
    float y2angle; //#3
    float y1correction; //#4
    float y2correction; //#5
    float sheetwidth; //#6
    float sheetthickness; //#7
    float vgroovewidth;//#8
    bool enableBackGuage;//#9
    float x1pos;//10
    float x2pos;//11
};

struct programdb {
    int totalProgram;
    int currentProgram;
    int totalBendInProgram;
    int currentBendID;
};

extern File proglist, program;
extern benddb bendDB;
extern programdb progDB;


void initPrograms();
void loadDefaultProgList();
void loadProgList();
bool loadProgram(int progid);
void loadbend(int progid, int bendid);

#endif