//#############################################################################
//  File:      VRTurnTableServer.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRTurnTableServer.h"
#include "VRTurnTableWorldState.h"

using namespace VirtualRoom;

//-----------------------------------------------------------------------------
/*! 
calculates the rotation of the turn table. The rotation changes if there is
a players hand at the table
*/
void TurnTableServer::simulate()
{
	SLVec4f effectivePos[VR_HAND_COUNT];

	SLfloat yMin = 0.8f;
	SLfloat yMax = 0.84f;

	SLQuat4f tmp;

	for(SLint i = 0; i < _maxPlayers; i++) {
		if(_hasSkeletonData[i]) {
			effectivePos[VR_LEFT_HAND] = _leftHandPos[i];
			effectivePos[VR_RIGHT_HAND] = _rightHandPos[i];

			// left hand
			if(effectivePos[VR_LEFT_HAND].y >= yMin && effectivePos[VR_LEFT_HAND].y <= yMax && sqrt(effectivePos[VR_LEFT_HAND].x * effectivePos[VR_LEFT_HAND].x + effectivePos[VR_LEFT_HAND].z * effectivePos[VR_LEFT_HAND].z) <= 0.5f)
			{
				tmp = SLQuat4f(-1, 0, 0);
				_rotation *= tmp;
			}

			// right hand
			if(effectivePos[VR_RIGHT_HAND].y >= yMin && effectivePos[VR_RIGHT_HAND].y <= yMax && sqrt(effectivePos[VR_RIGHT_HAND].x * effectivePos[VR_RIGHT_HAND].x + effectivePos[VR_RIGHT_HAND].z * effectivePos[VR_RIGHT_HAND].z) <= 0.5f)
			{
				tmp = SLQuat4f(1, 0, 0);
				_rotation *= tmp;
			}
		}		
	}
}
//-----------------------------------------------------------------------------
/*! 
return the state of the turn table application
*/
WorldState* TurnTableServer::getWorldState()
{
	TurnTableWorldState* state = new TurnTableWorldState(_timer.getElapsedTimeInMilliSec(), _maxPlayers);
	state->_rotation = _rotation;
	return state;
}
//-----------------------------------------------------------------------------