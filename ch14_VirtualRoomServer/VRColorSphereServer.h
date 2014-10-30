//#############################################################################
//  File:      VRColorSphereServer.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRCOLORSPHERESERVER_H
#define VRCOLORSPHERESERVER_H

#include "VRServer.h"

namespace VirtualRoom
{
	//-----------------------------------------------------------------------------
	//! the base color of a players hands
	struct PlayerColor
	{
		SLCol4f leftHand;
		SLCol4f rightHand;
	};
	//-----------------------------------------------------------------------------
	//! color sphere server
	/*!      
	defines the color for the hands of each player and calculates the color of the
	sphere
	*/
	//-----------------------------------------------------------------------------
	class ColorSphereServer : public Server
	{
	public:
		ColorSphereServer(SLint maxPlayers);
		~ColorSphereServer();

	protected:
		SLCol4f				_sphereColor;			//!< current sphere color
		PlayerColor*		_playerColor;			//!< the defined colors for the players

		virtual void		simulate		();
		virtual WorldState* getWorldState	();
	};

}
//-----------------------------------------------------------------------------
#endif