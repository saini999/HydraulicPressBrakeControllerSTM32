#include <ProgramManager.h>

File proglist, prog;

benddb bendDB;

programdb progDB;

void initPrograms() {
    while(!SD.begin());
    if(!SD.exists("PROGRAMS")){
        SD.mkdir("PROGRAMS");
    }
    if(!SD.exists("PROGRAMS/PROGLIST.TXT")){
        loadDefaultProgList();
    } else {
        loadProgList();
    }
}
void loadDefaultProgList() {
    progDB.totalProgram = 0;
    progDB.currentProgram = 0;
    progDB.totalBendInProgram = 0;
    progDB.currentBendID = 0;
    proglist = SD.open("PROGRAMS/PROGLIST.TXT",FILE_WRITE);
    proglist.seek(0);
    proglist.println("T000");
    proglist.flush();
    proglist.close();
}

void loadProgList() {
    proglist = SD.open("PROGRAMS/PROGLIST.TXT", FILE_READ);
    proglist.seek(0);
    String str;
    str = proglist.readStringUntil('\n');
    if(str.indexOf('T') != -1){
        str = str.substring(str.indexOf('T')+1, str.indexOf('T')+4);
        progDB.totalProgram = str.toInt();
    }
    proglist.close();
}

bool loadProgram(int progid) {
    proglist = SD.open("PROGRAMS/PROGLIST.TXT", FILE_READ);
    proglist.seek(0);
    String str,tstr;
    int tempid;
    while(tempid != progid){
        str = proglist.readStringUntil('\n');
        str = str.substring(0, str.indexOf('\r'));
        tstr = str.substring(str.indexOf('P')+1, str.indexOf('P')+4);
        tempid = tstr.toInt();
    }
    tstr = str.substring(str.indexOf('P')+5, str.indexOf('P')+8);
    progDB.totalBendInProgram = tstr.toInt();
    proglist.close();
}

bool createNewProgram(){
    int oldprogid = progDB.totalProgram;
    int newprogid = oldprogid + 1;
    proglist = SD.open("PROGRAMS/PROGLIST.TXT", FILE_WRITE);
    proglist.seek(0);
    int cur = 0;
    char* temp;
    String str,tstr;
    sprintf(temp, "%03d", newprogid);
    tstr = (String)temp;
    str = "T" + tstr;
    proglist.println(str);
    proglist.flush();
    //Calculate Postion for placing new line of ProgID and BendID
    if(oldprogid != 0){
    sprintf(temp, "%03d", oldprogid);
    tstr = "P";
    tstr += (String)temp;
    proglist.seek(0);
    do {str = proglist.readStringUntil('\n');}
    while(str.indexOf(tstr) == -1);
    }
    tstr = "P";
    sprintf(temp, "%03d", newprogid);
    tstr += (String)temp;
    tstr += ".000";
    proglist.println(tstr);
    proglist.flush();
    proglist.close();
    tstr = "PROGRAMS/";
    tstr += (String)temp;
    SD.mkdir(tstr.c_str());
    return true;
}

bool createNewBend(int progid){
    char* temp;
    int cur = 0;
    String str, tstr;
    sprintf(temp, "%03d", progid);
    tstr = "PROGRAMS/";
    tstr += (String)temp;
    if(SD.exists(tstr.c_str())){
        loadProgram(progid);
        int bendOld = progDB.totalBendInProgram;
        int newBend = bendOld + 1;
        sprintf(temp, "%03d", newBend);
        tstr += "/";
        tstr += (String)temp;
        tstr += ".TXT";
        if(!SD.exists(tstr.c_str())){
            prog = SD.open(tstr.c_str(), FILE_WRITE);
            prog.seek(0);
            for(int i=0; i < 11; i++){
                switch(i){
                    case 0:
                        str = (String)bendDB.bendID;
                        break;
                    case 1:
                        str = String(bendDB.y1angle, 3);
                        break;
                    case 2:
                        str = String(bendDB.y2angle, 3);
                        break;
                    case 3:
                        str = String(bendDB.y1correction, 3);
                        break;
                    case 4:
                        str = String(bendDB.y2correction, 3);
                        break;
                    case 5:
                        str = String(bendDB.sheetwidth, 3);
                        break;
                    case 6:
                        str = String(bendDB.sheetthickness, 3);
                        break;
                    case 7:
                        str = String(bendDB.vgroovewidth, 3);
                        break;
                    case 8:
                        str = (String)((uint8_t)bendDB.enableBackGuage);
                        break;
                    case 9:
                        str = String(bendDB.x1pos, 3);
                        break;
                    case 10:
                        str = String(bendDB.x2pos, 3);
                        break;
                }
                prog.println(str);
            }
            prog.flush();
            prog.close();
            return true;
        } else {return false;}
    } else {return false;}
}

bool loadBend(int progid, int bendid){
    char* temp;
    String str, tstr;
    tstr = "PROGRAMS/";
    sprintf(temp, "%03d", progid);
    tstr += (String)temp;
    tstr += "/";
    sprintf(temp, "%03d", bendid);
    tstr += (String)temp;
    tstr += ".TXT";
    if(SD.exists(tstr.c_str())){
        prog = SD.open(tstr.c_str(), FILE_READ);
        tstr = "";
        for(int i=0; i < 11; i++){
            str = prog.readStringUntil('\n');
            str = str.substring(0, str.indexOf('\r'));
            switch(i){
                case 0:
                    bendDB.bendID = str.toInt();
                    break;
                case 1:
                    bendDB.y1angle = str.toFloat();
                    break;
                case 2:
                    bendDB.y2angle = str.toFloat();
                    break;
                case 3:
                    bendDB.y1correction = str.toFloat();
                    break;
                case 4:
                    bendDB.y2correction = str.toFloat();
                    break;
                case 5:
                    bendDB.sheetwidth = str.toFloat();
                    break;
                case 6:
                    bendDB.sheetthickness = str.toFloat();
                    break;
                case 7:
                    bendDB.vgroovewidth = str.toFloat();
                    break;
                case 8:
                    bendDB.enableBackGuage = (uint8_t)str.toInt();
                    break;
                case 9:
                    bendDB.x1pos = str.toFloat();
                    break;
                case 10:
                    bendDB.x2pos = str.toFloat();
                    break;
            }
        }
        prog.close();
        return true;
    } else {
        return false;
    }
}

bool editBend(int progid, int bendid){
    char* temp;
    int cur = 0;
    String str, tstr;
    sprintf(temp, "%03d", progid);
    tstr = "PROGRAMS/";
    tstr += (String)temp;
    if(SD.exists(tstr.c_str())){
        loadProgram(progid);
        sprintf(temp, "%03d", bendid);
        tstr += "/";
        tstr += (String)temp;
        tstr += ".TXT";
        if(SD.exists(tstr.c_str())){
            prog = SD.open(tstr.c_str(), FILE_WRITE);
            prog.seek(0);
            for(int i=0; i < 11; i++){
                switch(i){
                    case 0:
                        str = (String)bendDB.bendID;
                        break;
                    case 1:
                        str = String(bendDB.y1angle, 3);
                        break;
                    case 2:
                        str = String(bendDB.y2angle, 3);
                        break;
                    case 3:
                        str = String(bendDB.y1correction, 3);
                        break;
                    case 4:
                        str = String(bendDB.y2correction, 3);
                        break;
                    case 5:
                        str = String(bendDB.sheetwidth, 3);
                        break;
                    case 6:
                        str = String(bendDB.sheetthickness, 3);
                        break;
                    case 7:
                        str = String(bendDB.vgroovewidth, 3);
                        break;
                    case 8:
                        str = (String)((uint8_t)bendDB.enableBackGuage);
                        break;
                    case 9:
                        str = String(bendDB.x1pos, 3);
                        break;
                    case 10:
                        str = String(bendDB.x2pos, 3);
                        break;
                }
                prog.println(str);
            }
            prog.flush();
            prog.close();
            return true;
        } else {return false;}
    } else {return false;}
}

bool deleteBend(int progid, int bendid){
    //Work in progress, not finished yet.
/*  char* temp;
    String str, tstr;
    sprintf(temp, "%03d", progid);
    tstr = "PROGRAMS/";
    tstr += (String)temp;
    if(SD.exists(tstr.c_str())){
        loadProgram(progid);
        sprintf(temp, "%03d", bendid);
        tstr += "/";
        tstr += (String)temp;
        tstr += ".TXT";
        if(SD.exists(tstr.c_str())){
            SD.remove(tstr.c_str());
            proglist = SD.open("PROGRAMS/PROGLIST.TXT", FILE_WRITE);
            str = proglist.readStringUntil('\n'); //ignore the first line
            if(progid != 0){
                tstr = "P";
                sprintf(temp, "%03d", progid - 1);
                tstr += (String)temp;
                do {
                    str = proglist.readStringUntil('/n');
                } while (str.indexOf(tstr) == -1);
            }
            
        }
    } */
}