#include <ParameterManager.h>

File param;
parameterdb parameter;
//Load Data from SD Card
void initParameters() {
    while(!SD.begin());
    if(!SD.exists("PARAMETERS")){
        SD.mkdir("PARAMETERS");
    }
    if(!SD.exists("PARAMETERS/PARAMETERS.TXT")){
        loadDefaultParameters();
    } else {
        loadParameters();
    }
}

void loadDefaultParameters() {
    param = SD.open("PARAMETERS/PARAMETERS.TXT", FILE_WRITE);
    parameter.starRunTime = 5.123;
    parameter.starToDeltaDelay = 0.150;
    parameter.y1ref = 200.0;
    parameter.y2ref = 200.0;
    parameter.fastUpLimit = 150.0;
    parameter.fastDownLimit = 20.0;
    parameter.maxDiff = 10.0;
    parameter.softLimit = 180.0;
    String str;
    for(uint8_t i=0; i<8; i++){
        switch(i){
            case 0:
                str = String(parameter.starRunTime, 3);
                break;
            case 1:
                str = String(parameter.starToDeltaDelay, 3);
                break;
            case 2:
                str = String(parameter.y1ref, 3);
                break;
            case 3:
                str = String(parameter.y2ref, 3);
                break;
            case 4:
                str = String(parameter.fastUpLimit, 3);
                break;
            case 5:
                str = String(parameter.fastDownLimit, 3);
                break;
            case 6:
                str = String(parameter.maxDiff, 3);
                break;
            case 7:
                str = String(parameter.softLimit, 3);
                break;
        }
        param.println(str);
    }
    param.flush();
    param.close();
}

void updateParameters() {
    param = SD.open("PARAMETERS/PARAMETERS.TXT", FILE_WRITE);
    param.seek(0);
    String str;
    for(uint8_t i=0; i<8; i++){
        switch(i){
            case 0:
                str = String(parameter.starRunTime, 3);
                break;
            case 1:
                str = String(parameter.starToDeltaDelay, 3);
                break;
            case 2:
                str = String(parameter.y1ref, 3);
                break;
            case 3:
                str = String(parameter.y2ref, 3);
                break;
            case 4:
                str = String(parameter.fastUpLimit, 3);
                break;
            case 5:
                str = String(parameter.fastDownLimit, 3);
                break;
            case 6:
                str = String(parameter.maxDiff, 3);
                break;
            case 7:
                str = String(parameter.softLimit, 3);
                break;
        }
        param.println(str);
    }
    param.flush();
    param.close();
}

void loadParameters() {
    param = SD.open("PARAMETERS/PARAMETERS.TXT", FILE_READ);
    String str, tstr;
    for(uint8_t i = 0; i < 8; i++){
        str = param.readStringUntil('\n');
        tstr = str.substring(0, str.indexOf('\r'));
        switch(i) {
            case 0:
                parameter.starRunTime = tstr.toFloat();
                break;
            case 1:
                parameter.starToDeltaDelay = tstr.toFloat();
                break;
            case 2:
                parameter.y1ref = tstr.toFloat();
                break;
            case 3:
                parameter.y2ref = tstr.toFloat();
                break;
            case 4:
                parameter.fastUpLimit = tstr.toFloat();
                break;
            case 5:
                parameter.fastDownLimit = tstr.toFloat();
                break;
            case 6:
                parameter.maxDiff = tstr.toFloat();
                break;
            case 7:
                parameter.softLimit = tstr.toFloat();
                break;
        }
    }
    param.close();
}