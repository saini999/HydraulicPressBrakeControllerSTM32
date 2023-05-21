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
    uint8_t* buf;
    proglist.read(buf, proglist.size());
    String str = (const char*)buf;
    String tstr;
    tstr = str.substring(str.indexOf("T")+1, str.indexOf("T")+3);
    progDB.totalProgram = tstr.toInt();
    proglist.close();
}

bool loadProgram(int progid) {
    proglist = SD.open("PROGRAMS/PROGLIST.TXT", FILE_READ);
    proglist.seek(0);
    uint8_t* buf;
    proglist.read(buf, proglist.size());
    String str = (const char*)buf;
    String tstr;
    int cur = 0;
    char* temp;
    sprintf(temp, "%03d", progid);
    tstr = (String)temp;
    if(str.indexOf("P" + tstr) != -1){
        progDB.currentProgram = progid;
        tstr = str.substring(str.indexOf("P" + tstr)+5, str.indexOf("P" + tstr)+8);
        progDB.totalBendInProgram = tstr.toInt();
        return true;
    } else {
        return false;
    }
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
    proglist.seek(0);
    //Calculate Postion for placing new line of ProgID and BendID
    sprintf(temp, "%03d", oldprogid);
    tstr = "P";
    tstr += (String)temp;
    cur = str.indexOf(tstr) + 10;
    tstr = "P";
    sprintf(temp, "%03d", newprogid);
    tstr += (String)temp;
    tstr += ".000";
    proglist.seek(cur);
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
    } else {
        return false;
    }
}


