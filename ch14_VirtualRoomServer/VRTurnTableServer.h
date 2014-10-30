//#############################################################################
//  File:      VRTurnTableServer.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRTURNTABLESERVER_H
#define VRTURNTABLESERVER_H

#include "VRServer.h"

namespace VirtualRoom
{
	//-----------------------------------------------------------------------------
	//! turn table game server
	/*!      
	server class for the turn table application
	*/
	//-----------------------------------------------------------------------------
	class TurnTableServer : public Server
	{
	public:
		TurnTableServer(SLint maxPlayers) : Server(maxPlayers, VR_GAME_TYPE_TURN_TABLE) { }
		~TurnTableServer() { }

	protected:
		virtual void simulate();
		virtual WorldState* getWorldState();

	private:
		SLQuat4f			_rotation;		//!< current rotation of the table
	};

}
//-----------------------------------------------------------------------------
#endif