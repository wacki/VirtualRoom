//#############################################################################
//  File:      VRBoardGame.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRBoardGame.h"

using namespace VirtualRoom::Game;
using namespace VirtualRoom;

//-----------------------------------------------------------------------------
BoardGame::BoardGame(SLuint numberOfPlayers, VR_GAME_TYPE gameType, BoardFieldSelector* playFieldSelector, SLuint numberOfLevels, SLuint numberOfColumns, SLuint numberOfRows)
	: _numberOfPlayers(numberOfPlayers), _playFieldSelector(playFieldSelector), _actPlayerIndex(0), _gameState(VR_BOARDGAME_STATE_RUNNING), _gameType(gameType), _numberOfBoardPlayFieldLevels(numberOfLevels), _numberOfBoardPlayFieldColumns(numberOfColumns), _numberOfBoardPlayFieldRows(numberOfRows)
{
	this->_resetBoxesSelectors = new BoardFieldSelectorContinuous[_numberOfPlayers];

	this->_resetBoxes = new BoardField[_numberOfPlayers];

	for(SLuint i = 0; i < _numberOfPlayers; i++)
	{
		this->_resetBoxes[i]._initColor = SLCol4f::WHITE;
		this->_resetBoxes[i]._color = SLCol4f::WHITE;
		this->_resetBoxesSelectors[i].reset();
	}

	this->_boardPlayFields = new BoardPlayField**[_numberOfBoardPlayFieldLevels];
	for(SLuint z = 0; z < this->_numberOfBoardPlayFieldLevels; z++)
	{
		this->_boardPlayFields[z] = new BoardPlayField*[_numberOfBoardPlayFieldColumns];
		for(SLuint y = 0; y < this->_numberOfBoardPlayFieldColumns; y++)
		{
			this->_boardPlayFields[z][y] = new BoardPlayField[_numberOfBoardPlayFieldRows];
		}
	}

	this->_players = new Player[_numberOfPlayers];

	for(SLuint i = 0; i < _numberOfPlayers; i++)
	{
		this->_players[i].setId(i);
	}
}
//-----------------------------------------------------------------------------
BoardGame::~BoardGame()
{
	delete[] this->_players;

	for(SLuint z = 0; z < this->_numberOfBoardPlayFieldLevels; z++)
	{
		for(SLuint y = 0; y < this->_numberOfBoardPlayFieldColumns; y++)
		{
			delete[] this->_boardPlayFields[z][y];
		}
		delete[] this->_boardPlayFields[z];
	}
	delete[] this->_boardPlayFields;

	if(_activeSimulation != NULL)
	{
		delete _activeSimulation;
	}

	delete[] this->_resetBoxes;

	delete _playFieldSelector;
	delete[] this->_resetBoxesSelectors;
}
//-----------------------------------------------------------------------------
/*! 
resets the board game
*/
void BoardGame::reset()
{
	for(SLuint i = 0; i < _numberOfPlayers; i++)
	{
		this->_resetBoxes[i].reset();
		this->_resetBoxesSelectors[i].reset();
	}
	this->_playFieldSelector->reset();

	for(SLuint z = 0; z < this->_numberOfBoardPlayFieldLevels; z++)
	{
		for(SLuint y = 0; y < this->_numberOfBoardPlayFieldColumns; y++)
		{
			for(SLuint x = 0; x < this->_numberOfBoardPlayFieldRows; x++)
			{
				this->_boardPlayFields[z][y][x].reset();
			}
		}
	}

	this->_gameState = VR_BOARDGAME_STATE::VR_BOARDGAME_STATE_RUNNING;
	_actPlayerIndex = 0;
}
//-----------------------------------------------------------------------------
/*! 
disselects all board game fields
*/
void BoardGame::unselectAllFields()
{
	for(SLuint z = 0; z < this->_numberOfBoardPlayFieldLevels; z++)
	{
		for(SLuint y = 0; y < this->_numberOfBoardPlayFieldColumns; y++)
		{
			for(SLuint x = 0; x < this->_numberOfBoardPlayFieldRows; x++)
			{
				this->_boardPlayFields[z][y][x].unselect();
			}
		}
	}
}
//-----------------------------------------------------------------------------
/*! 
handle game end
*/
void BoardGame::finisheGame(VirtualRoom::VR_BOARDGAME_STATE state)
{
	this->_gameState = state;

	switch(this->_gameState)
	{
	case VR_BOARDGAME_STATE_WON:
		SL_LOG("!!! Game was won by Player with index '%u' (%s) !!!\n", this->_players[this->_actPlayerIndex].getId(), this->_players[this->_actPlayerIndex].getName().c_str());
		break;
	case VR_BOARDGAME_STATE_TIE:
		SL_LOG("!!! Game ended in a tie !!!\n");
		break;
	default:
		// DO NOTHING
		break;
	}
}
//-----------------------------------------------------------------------------
/*! 
change active player
*/
void BoardGame::moveToNextPlayer(int indexLastSelectedX, int indexLastSelectedY, int indexLastSelectedZ)
{
	this->unselectAllFields();

	// Check if Game is finished or not!
	this->updateGameState(indexLastSelectedX, indexLastSelectedY, indexLastSelectedZ);

	if (this->_gameState == VR_BOARDGAME_STATE::VR_BOARDGAME_STATE_RUNNING)
	{
		_actPlayerIndex = (_actPlayerIndex+1) % _numberOfPlayers;
	}
}
//-----------------------------------------------------------------------------
/*! 
returns the id of the active player
*/
SLint BoardGame::getActPlayerId()
{
	return this->_players[this->_actPlayerIndex].getId();
}
//-----------------------------------------------------------------------------
/*! 
returns if the game is running
*/
SLbool BoardGame::isRuning()
{
	return this->_gameState == VR_BOARDGAME_STATE::VR_BOARDGAME_STATE_RUNNING;
}
//-----------------------------------------------------------------------------
/*! 
returns if the game has a active simulation
*/
SLbool BoardGame::hasActiveSimulation()
{
	return (this->_activeSimulation != NULL);
}
//-----------------------------------------------------------------------------
/*! 
simulates a step of a simulation
*/
void BoardGame::doSimulationStep()
{
	this->_activeSimulation->doSimulationStep();

	if (this->_activeSimulation->isSimulationFinished())
	{
		delete this->_activeSimulation;
		this->_activeSimulation = NULL;
	}
}
//-----------------------------------------------------------------------------
/*! 
do a move of a player
*/
void BoardGame::play(SLVec3f& effectivePosLeftHand, SLVec3f& effectivePosRightHand)
{
	int indexLastSelectedX;
	int indexLastSelectedY;
	int indexLastSelectedZ;
	BoardPlayField* boardPlayField = this->checkHit(effectivePosLeftHand, effectivePosRightHand, indexLastSelectedX, indexLastSelectedY, indexLastSelectedZ);

	if (boardPlayField == NULL || boardPlayField->_owner == NULL)
	{
		if (this->_playFieldSelector->select(boardPlayField, this->_players[this->_actPlayerIndex].getSelectionColor()))
		{
			boardPlayField->_owner = &this->_players[this->_actPlayerIndex];
			BoardGameSimulation* simulation = this->getMoveSimulation(boardPlayField, indexLastSelectedX, indexLastSelectedY, indexLastSelectedZ);
			this->moveToNextPlayer(indexLastSelectedX, indexLastSelectedY, indexLastSelectedZ);

			if (simulation != NULL)
			{
				this->_activeSimulation = simulation;
			}
		}
	}
}
//-----------------------------------------------------------------------------
/*! 
handle the reset action
*/
void BoardGame::handleReset(int playerIndex, SLVec3f& effectivePosLeftHand, SLVec3f& effectivePosRightHand)
{
	BoardField* resetBox = NULL;
	
	if (this->_resetBoxes[playerIndex].checkHit(effectivePosLeftHand) || this->_resetBoxes[playerIndex].checkHit(effectivePosRightHand))
		resetBox = &this->_resetBoxes[playerIndex];

	if (this->_resetBoxesSelectors[playerIndex].select(resetBox, SLCol4f::BLUE))
	{
		this->reset();
	}
}
//-----------------------------------------------------------------------------
/*! 
sets the player matrices
*/
void BoardGame::setPersonMatrices(SLMat4f* personMatrices)
{
	this->_personMatrices = personMatrices;

	for(SLuint i = 0; i < _numberOfPlayers; i++)
	{
		SLVec4f posMin = this->_personMatrices[i] * SLVec3f(RESET_BOX_POS_X, RESET_BOX_POS_Y, 0.0f);

		this->_resetBoxes[i]._actualPosition = SLVec3f(posMin.x, posMin.y, posMin.z);
		this->_resetBoxes[i]._width = RESET_BOX_CUBE_SIDE;
		this->_resetBoxes[i]._heigh = RESET_BOX_CUBE_SIDE;
		this->_resetBoxes[i]._depth = RESET_BOX_CUBE_SIDE;
	}
}
//-----------------------------------------------------------------------------