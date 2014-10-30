//#############################################################################
//  File:      VRBoardGameServer.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRBoardGameServer.h"
#include "VRColorSphereWorldState.h"
#include "VRTypes.h"

using namespace VirtualRoom;

//-----------------------------------------------------------------------------
BoardGameServer::BoardGameServer(Game::BoardGame* boardGame) : Server(boardGame->_numberOfPlayers, boardGame->_gameType), _boardGame(boardGame)
{
	this->_boardGame->setPersonMatrices(this->_personMatrices);
};
//-----------------------------------------------------------------------------
BoardGameServer::~BoardGameServer()
{
	delete _boardGame;
}
//-----------------------------------------------------------------------------
/*! 
calculates the board game actions. Only the active player can manipulate the
game. If the game has ended, all player are allowed to reset the game
*/
void BoardGameServer::simulate()
{
	if(this->_boardGame->hasActiveSimulation())
	{
		this->_boardGame->doSimulationStep();
	}
	else
	{
		if (this->_boardGame->isRuning())
		{
			int actPlayerId = this->_boardGame->getActPlayerId();

			if (_hasSkeletonData[actPlayerId])
			{
				SLVec3f posLeftHand = _leftHandPos[actPlayerId];
				SLVec3f posRightHand = _rightHandPos[actPlayerId];

				this->_boardGame->play(posLeftHand, posRightHand);
			}
		}
		else
		{
			for(int actPlayerId = 0; actPlayerId < 2; actPlayerId++)
			{
				if (_hasSkeletonData[actPlayerId])
				{
					SLVec3f posLeftHand = _leftHandPos[actPlayerId];
					SLVec3f posRightHand = _rightHandPos[actPlayerId];

					this->_boardGame->handleReset(actPlayerId, posLeftHand, posRightHand);
				}
			}
		}
	}
}
//-----------------------------------------------------------------------------
/*! 
returns the board game state
*/
WorldState* BoardGameServer::getWorldState()
{
	return this->_boardGame->getBoardState(_timer, _maxPlayers);
}
//-----------------------------------------------------------------------------