//#############################################################################
//  File:      VRBoardGameServer.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRBOARDGAMESERVER_H
#define VRBOARDGAMESERVER_H

#include "VRServer.h"
#include "VRBoardGame.h"

namespace VirtualRoom
{
	//-----------------------------------------------------------------------------
	//! board game server
	/*!      
	implements all basic features for a board game
	*/
	//-----------------------------------------------------------------------------
	class BoardGameServer : public Server
	{
	public:
		BoardGameServer				(Game::BoardGame* boardGame);
		virtual ~BoardGameServer	();

	protected:
		Game::BoardGame* _boardGame;	//!< the game

		void simulate				();
		WorldState* getWorldState	();
	};

}
//-----------------------------------------------------------------------------
#endif