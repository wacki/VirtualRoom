//#############################################################################
//  File:      VRColorSphereServer.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRColorSphereServer.h"
#include "VRColorSphereWorldState.h"

using namespace VirtualRoom;

//-----------------------------------------------------------------------------
ColorSphereServer::ColorSphereServer(SLint maxPlayers)
	: Server(maxPlayers, VR_GAME_TYPE_COLOR_SPHERE)
{
	_playerColor = new PlayerColor[maxPlayers];
	
	// init colors
	SLfloat	r;
	SLfloat g;
	SLfloat b;

	for(SLint i = 0; i < maxPlayers; i++)
	{
		// left hand
		r = (rand() % 1000) / 1000.0f;
		g = (rand() % 1000) / 1000.0f;
		b = (rand() % 1000) / 1000.0f;
		_playerColor[i].leftHand = SLCol4f(r, g, b, 1.0f);

		// right hand
		r = (rand() % 1000) / 1000.0f;
		g = (rand() % 1000) / 1000.0f;
		b = (rand() % 1000) / 1000.0f;
		_playerColor[i].rightHand = SLCol4f(r, g, b, 1.0f);
	}
	
}
//-----------------------------------------------------------------------------
ColorSphereServer::~ColorSphereServer()
{
	delete[] _playerColor;
}
//-----------------------------------------------------------------------------
/*! 
calculates the color of the sphere. The color depends on the distance of each
player hands to the sphere
*/
void ColorSphereServer::simulate()
{
	SLVec4f effectivePos[VR_HAND_COUNT];
	SLfloat factor[VR_HAND_COUNT];
	
	SLfloat	colorStartDist = 0.8f;	//distance where we start adding color
	SLfloat colorEndDist = 0.2f;	//distance where we add the full color
	SLfloat colorInterpolationDist = colorStartDist - colorEndDist;
	SLVec3f iniSphere(0.0f, 1.0f, 0.0f);

	//reset sphere color to base (black)
	_sphereColor = SLCol4f(0, 0, 0, 1);
	
	//check distance of both clients
	for(SLint i = 0; i < _maxPlayers; i++) {
		if(_hasSkeletonData[i]) {
			effectivePos[VR_LEFT_HAND] = _leftHandPos[i];
			effectivePos[VR_RIGHT_HAND] = _rightHandPos[i];

			// left hand
			SLfloat leftHand = iniSphere.distance(SLVec3f(effectivePos[VR_LEFT_HAND].x, effectivePos[VR_LEFT_HAND].y, effectivePos[VR_LEFT_HAND].z));
			//interpolation amount
			factor[VR_LEFT_HAND] =  1.0f - SL_clamp((leftHand - colorEndDist) / colorInterpolationDist, 0.0f, 1.0f);
			_sphereColor += factor[VR_LEFT_HAND] * _playerColor[i].leftHand;

			// right hand
			SLfloat rightHand = iniSphere.distance(SLVec3f(effectivePos[VR_RIGHT_HAND].x, effectivePos[VR_RIGHT_HAND].y, effectivePos[VR_RIGHT_HAND].z));
			//interpolation amount
			factor[VR_RIGHT_HAND] =  1.0f - SL_clamp((rightHand - colorEndDist) / colorInterpolationDist, 0.0f, 1.0f);
			_sphereColor += factor[VR_RIGHT_HAND] * _playerColor[i].rightHand;									
		}		
	}
}
//-----------------------------------------------------------------------------
/*! 
returns the state of the color sphere application
*/
WorldState* ColorSphereServer::getWorldState()
{
	ColorSphereWorldState* state = new ColorSphereWorldState(_timer.getElapsedTimeInMilliSec(), _maxPlayers);
	state->_sphereColor = _sphereColor;
	return state;
}
//-----------------------------------------------------------------------------