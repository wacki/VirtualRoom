//#############################################################################
//  File:      VRTurnTableGameScene.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "SLTrackingCamera.h"



SLbool SLTrackingCamera::animateRec(SLfloat timeMS)
{
    SLbool gotAnimated = SLCamera::animateRec(timeMS);

    if (_isTracking) {
        lookAt(_trackingPosWS);
        gotAnimated = true;
    }
    return gotAnimated;
}