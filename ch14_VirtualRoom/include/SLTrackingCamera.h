//#############################################################################
//  File:      VRTurnTableGameScene.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef SL_TRACKINGCAMERA_H
#define SL_TRACKINGCAMERA_H

#include "SLCamera.h"



class SLTrackingCamera : public SLCamera  {

public:
    SLTrackingCamera()
        : _isTracking(true), _trackingPosWS(0, 0, 0)
    { }

    virtual SLbool animateRec(SLfloat timeMS);


private:
    SLbool      _isTracking;
    SLVec3f     _trackingPosWS;
};


//-----------------------------------------------------------------------------
#endif