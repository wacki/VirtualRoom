//#############################################################################
//  File:      VRWorldState.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRWORLDSTATE_H
#define VRWORLDSTATE_H

#include "VRBaseMessage.h"

namespace VirtualRoom
{	
	//-----------------------------------------------------------------------------
	//! world state sent from the server to every client
	/*!      
	a world state is a snapshot of every manipulable object (shape / position / rotation / color / ...)
	within a scene. As the virtual room always contains person in it, the base world state
	contains the number of players, an information about each player if he has skeleton
	data or not and if so, the effective data
	*/
	class WorldState : public Network::BaseMessage 
	{
	public:	
		SLint						_maxPlayers;			//!< number of players in to virtual room
		SLbool*						_hasSkeletonData;		//!< dynamic array indicating for each player if there are data
		SkeletonData*				_playerData;			//!< effective data for each player if any

		WorldState					(double timeStamp, SLint maxPlayers);
		WorldState					(RakNet::BitStream* bsIn);
		virtual ~WorldState			();

		virtual void serialize		(RakNet::BitStream* bsOut);	
	};
}
//-----------------------------------------------------------------------------
#endif