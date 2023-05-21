#ifndef PARAMETER_MANAGER_H

#define PARAMETER_MANAGER_H

    #ifndef HAL_SD_MODULE_ENABLED
    #define HAL_SD_MODULE_ENABLED
    #endif

#include <STM32SD.h>

struct parameterdb {
    float starRunTime; //#1
    float starToDeltaDelay; //#2
    float y1ref; //#3
    float y2ref; //#4
    float fastUpLimit; //#5
    float fastDownLimit; //#6
    float maxDiff;//#7
    float softLimit;//#8
};
extern File param;
extern parameterdb parameter;

void initParameters();
void loadDefaultParameters();
void loadParameters();
void initParameters();

#endif