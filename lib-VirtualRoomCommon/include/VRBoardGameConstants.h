//#############################################################################
//  File:      VRBoardGameConstants.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRBOARDGAMECONSTANTS_H
#define VRBOARDGAMECONSTANTS_H

#define BOARDGAME_NO_OWNER_CODE -1
#define BOX_ALPHA_VALUE 0.6f

#define RESET_BOX_POS_X 0.5f
#define RESET_BOX_POS_Y 0.75f
#define RESET_BOX_CUBE_SIDE 0.25f

#define HAND_SELECTION_RADIUS 0.05f

namespace VirtualRoom
{
	//-----------------------------------------------------------------------------
	//! define the different states a board game can have
	enum VR_BOARDGAME_STATE
	{
		VR_BOARDGAME_STATE_RUNNING,
		VR_BOARDGAME_STATE_WON,
		VR_BOARDGAME_STATE_TIE
	};
}
//-----------------------------------------------------------------------------
#endif