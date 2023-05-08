#ifndef PositionGen_h
#define PositionGen_h
#include <Arduino.h>

typedef float mm, degree, rad;



class PositionGen {
    public:
        //Init Position Generator
        PositionGen();
        //Set Bending Angle
        void setAngle(degree angle);
        //Get Current Bending Angle
        degree getAngle();
        //Set Dye VGroove Width (Not the Depth)
        void setDye(mm vGrooveWidth);
        //Get Dye VGroove Width (Not the Depth)
        mm getDye();
        //Set Angle Correction for Bending
        void setAngleCorrection(degree angle);
        //Get Saved Angle Corrections
        degree getAngleCorrection();
        //Set Sheet Thickness
        void setSheetThickness(mm thickness);
        //Get Sheet Thickness
        mm getSheetThickness();
        //Returns the Value to move the machine to bending point
        mm getFinalPosition();
    private:
        degree _angle;
        rad _anglerad;
        mm _dye;
        degree _angleCorrection;
        mm _postionCorrection;
        mm _thickness;
        mm _finalPos;
};


#endif
