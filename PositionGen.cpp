
#include <PositionGen.h>


PositionGen::PositionGen(){

}

void PositionGen::setAngle(degree angle){
    if(angle >= 90 && angle <= 180){
        _angle = angle;
    } else {
        _angle = 180;
    }
}

degree PositionGen::getAngle(){
    return _angle;
}

void PositionGen::setDye(mm vGroveWidth){
    _dye = vGroveWidth / 2;
}

mm PositionGen::getDye(){
    return _dye * 2;
}

void PositionGen::setAngleCorrection(degree angle){
    _angleCorrection = angle;
}

degree PositionGen::getAngleCorrection(){
    return _angleCorrection;
}

void PositionGen::setSheetThickness(mm thickness){
    _thickness = thickness;
}

mm PositionGen::getSheetThickness(){
    return _thickness;
}

mm PositionGen::getFinalPosition(){
    mm finalpos;
    _anglerad = radians(_angle) + radians(_angleCorrection);
    finalpos = _dye;
    finalpos = finalpos * sin(_anglerad);
    finalpos = finalpos * -1;
    finalpos = finalpos + _thickness;
    return finalpos;
}