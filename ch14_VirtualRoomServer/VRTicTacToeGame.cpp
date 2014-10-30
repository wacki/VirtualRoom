//#############################################################################
//  File:      VRTicTacToeGame.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRTicTacToeWorldState.h"
#include "VRTicTacToeGame.h"
#include "VRBoardFieldSelectorConfirm.h"

using namespace VirtualRoom;
using namespace VirtualRoom::Game;

//-----------------------------------------------------------------------------
TicTacToeGame::TicTacToeGame() : BoardGame(2, VR_GAME_TYPE_TIC_TAC_TOE, new BoardFieldSelectorConfirm(), 1, TICTACTOE_NUMBER_OF_ROWS_COLUMNS, TICTACTOE_NUMBER_OF_ROWS_COLUMNS)
{
	this->_players[0].setName(TICTACTOE_PLAYER0_COLOR_NAME);
	this->_players[0].setColor(TICTACTOE_PLAYER0_COLOR_VALUE);
	this->_players[0].setSelectionColor(SLCol4f(0.0f, 0.0f, 1.0f, BOX_ALPHA_VALUE));
	this->_players[1].setName(TICTACTOE_PLAYER1_COLOR_NAME);
	this->_players[1].setColor(TICTACTOE_PLAYER1_COLOR_VALUE);
	this->_players[1].setSelectionColor(SLCol4f(0.0f, 0.0f, 1.0f, BOX_ALPHA_VALUE));

	for(SLuint z = 0; z < this->_numberOfBoardPlayFieldLevels; z++)
	{
		for(SLuint y = 0; y < this->_numberOfBoardPlayFieldColumns; y++)
		{
			for(SLuint x = 0; x < this->_numberOfBoardPlayFieldRows; x++)
			{
				this->_boardPlayFields[z][y][x]._heigh = TICTACTOE_BOARDGAME_FIELD_HEIGH;
				this->_boardPlayFields[z][y][x]._width = TICTACTOE_BOARDGAME_FIELD_WIDTH;
				this->_boardPlayFields[z][y][x]._depth = TICTACTOE_BOARDGAME_FIELD_DEPTH;
			}
		}
	}

	SLVec3f iniBoardPosition(0.0f, TICTACTOE_BOARDGAME_Y_OFFSET, 0.0f);

	SLfloat xVal = 0.0f;
	SLfloat yVal = - (TICTACTOE_BOARDGAME_FIELD_HEIGH + TICTACTOE_BOARDGAME_FIELD_BOARDER) - (TICTACTOE_BOARDGAME_FIELD_HEIGH / 2);
	SLfloat zVal = -(TICTACTOE_BOARDGAME_FIELD_DEPTH / 2);

	for(SLuint z = 0; z < this->_numberOfBoardPlayFieldLevels; z++)
	{
		for(SLuint y = 0; y < this->_numberOfBoardPlayFieldColumns; y++)
		{
			xVal = - (TICTACTOE_BOARDGAME_FIELD_WIDTH + TICTACTOE_BOARDGAME_FIELD_BOARDER) - (TICTACTOE_BOARDGAME_FIELD_WIDTH / 2);

			for(SLuint x = 0; x < this->_numberOfBoardPlayFieldRows; x++)
			{
				this->_boardPlayFields[z][y][x]._actualPosition.x = xVal;
				this->_boardPlayFields[z][y][x]._actualPosition.y = yVal;
				this->_boardPlayFields[z][y][x]._actualPosition.z = zVal;
				this->_boardPlayFields[z][y][x]._actualPosition.add(iniBoardPosition);

				xVal += (TICTACTOE_BOARDGAME_FIELD_WIDTH + TICTACTOE_BOARDGAME_FIELD_BOARDER);
			}

			yVal += (TICTACTOE_BOARDGAME_FIELD_HEIGH + TICTACTOE_BOARDGAME_FIELD_BOARDER);
		}
	}
}
//-----------------------------------------------------------------------------
/*! 
updates the game state
*/
void TicTacToeGame::updateGameState(int indexLastSelectedX, int indexLastSelectedY, int indexLastSelectedZ)
{
	SLint owner = this->_boardPlayFields[indexLastSelectedZ][indexLastSelectedY][indexLastSelectedX].getOwnerId();
	// Check if even selected
	if (owner == BOARDGAME_NO_OWNER_CODE)
		return;

	// Check row
	bool oneOther = false;
	int checkX = (indexLastSelectedX + 1) % _numberOfBoardPlayFieldRows;
	while(checkX != indexLastSelectedX)
	{
		if (owner != this->_boardPlayFields[indexLastSelectedZ][indexLastSelectedY][checkX].getOwnerId())
		{
			oneOther = true;
			break;
		}

		checkX = (checkX + 1) % _numberOfBoardPlayFieldRows;
	}

	// All identical, so one has won
	if (!oneOther)
	{
		this->finisheGame(VR_BOARDGAME_STATE_WON);
		return;
	}

	// Check column
	oneOther = false;
	int checkY = (indexLastSelectedY + 1) % _numberOfBoardPlayFieldColumns;
	while(checkY != indexLastSelectedY)
	{
		if (owner != this->_boardPlayFields[indexLastSelectedZ][checkY][indexLastSelectedX].getOwnerId())
		{
			oneOther = true;
			break;
		}

		checkY = (checkY + 1) % _numberOfBoardPlayFieldColumns;
	}

	// All identical, so one has won
	if (!oneOther)
	{
		this->finisheGame(VR_BOARDGAME_STATE_WON);
		return;
	}

	// Check first diagonals
	if (indexLastSelectedY == indexLastSelectedX)
	{
		// Check row
		oneOther = false;
		int checkXY = (indexLastSelectedX + 1) % _numberOfBoardPlayFieldRows;
		while(checkXY != indexLastSelectedX)
		{
			if (owner != this->_boardPlayFields[indexLastSelectedZ][checkXY][checkXY].getOwnerId())
			{
				oneOther = true;
				break;
			}

			checkXY = (checkXY + 1) % _numberOfBoardPlayFieldRows;
		}

		// All identical, so one has won
		if (!oneOther)
		{
			this->finisheGame(VR_BOARDGAME_STATE_WON);
			return;
		}
	}

	// Check second diagonals
	if (indexLastSelectedY == (_numberOfBoardPlayFieldRows - 1 - indexLastSelectedX))
	{
		// Check row
		oneOther = false;
		int checkXY = (indexLastSelectedX + 1) % _numberOfBoardPlayFieldRows;
		while(checkXY != indexLastSelectedX)
		{
			if (owner != this->_boardPlayFields[indexLastSelectedZ][_numberOfBoardPlayFieldRows - 1 - checkXY][checkXY].getOwnerId())
			{
				oneOther = true;
				break;
			}

			checkXY = (checkXY + 1) % _numberOfBoardPlayFieldRows;
		}

		// All identical, so one has won
		if (!oneOther)
		{
			this->finisheGame(VR_BOARDGAME_STATE_WON);
			return;
		}
	}

	for(SLuint z = 0; z < this->_numberOfBoardPlayFieldLevels; z++)
	{
		for(SLuint y = 0; y < this->_numberOfBoardPlayFieldColumns; y++)
		{
			for(SLuint x = 0; x < this->_numberOfBoardPlayFieldRows; x++)
			{
				if (this->_boardPlayFields[z][y][x].getOwnerId() == BOARDGAME_NO_OWNER_CODE)
				{
					// Game is still running -> return and do nothing
					return;
				}
			}
		}
	}

	// Not running an no one won
	this->finisheGame(VR_BOARDGAME_STATE_TIE);
}
//-----------------------------------------------------------------------------
/*! 
returns the board game state
*/
WorldState* TicTacToeGame::getBoardState(SLTimer &timer, SLint maxPlayers)
{
	TicTacToeWorldState* worldState = new TicTacToeWorldState(timer.getElapsedTimeInMilliSec(), maxPlayers);

	int cnt = 0;
	for(SLuint z = 0; z < this->_numberOfBoardPlayFieldLevels; z++)
	{
		for(SLuint y = 0; y < this->_numberOfBoardPlayFieldColumns; y++)
		{
			for(SLuint x = 0; x < this->_numberOfBoardPlayFieldRows; x++, cnt++)
			{
				if (this->_boardPlayFields[z][y][x]._owner == NULL)
				{
					worldState->_fieldColors[cnt] = this->_boardPlayFields[z][y][x]._color;
				}
				else
				{
					worldState->_fieldColors[cnt] = this->_boardPlayFields[z][y][x]._owner->getColor();
				}
			}
		}
	}

	worldState->_gameState = this->_gameState;
	worldState->_actPlayerId = this->_players[this->_actPlayerIndex].getId();

	for(SLuint i = 0; i < this->_numberOfPlayers; i++)
	{
		worldState->_resetBoxColors[i] = this->_resetBoxes[i]._color;
	}

	return worldState;
}
//-----------------------------------------------------------------------------
/*! 
checks if a field of the game is hitted
*/
BoardPlayField* TicTacToeGame::checkHit(SLVec3f& effectivePosLeftHand, SLVec3f& effectivePosRightHand, int& indexLastSelectedX, int& indexLastSelectedY, int& indexLastSelectedZ)
{
	BoardPlayField* BoardPlayField = NULL;
	bool found = false;

	for(SLuint z = 0; z < this->_numberOfBoardPlayFieldLevels; z++)
	{
		for(SLuint y = 0; y < this->_numberOfBoardPlayFieldColumns; y++)
		{
			for(SLuint x = 0; x < this->_numberOfBoardPlayFieldRows; x++)
			{
				if (this->_boardPlayFields[z][y][x].checkHit(effectivePosLeftHand) || this->_boardPlayFields[z][y][x].checkHit(effectivePosRightHand))
				{
					BoardPlayField = &this->_boardPlayFields[z][y][x];
					found = true;
					indexLastSelectedX = x;
					break;
				}
			}

			if (found)
			{
				indexLastSelectedY = y;
				break;
			}
		}

		if (found)
		{
			indexLastSelectedZ = z;
			break;
		}
	}

	return BoardPlayField;
}
//-----------------------------------------------------------------------------