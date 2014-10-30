//#############################################################################
//  File:      VRConnectFourGame.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRConnectFourWorldState.h"
#include "VRConnectFourGame.h"
#include "VRBoardFieldSelectorContinuous.h"
#include "VRConnectFourFallingSphereSimulation.h"

using namespace VirtualRoom;
using namespace VirtualRoom::Game;

//-----------------------------------------------------------------------------
ConnectFourGame::ConnectFourGame() : BoardGame(2, VR_GAME_TYPE_CONNECT_FOUR, new BoardFieldSelectorContinuous(), CONNECTFOUR_NUMBER_OF_ROWS_COLUMNS_LEVELS, CONNECTFOUR_NUMBER_OF_ROWS_COLUMNS_LEVELS, CONNECTFOUR_NUMBER_OF_ROWS_COLUMNS_LEVELS)
{
	this->_players[0].setName(CONNECTFOUR_PLAYER0_COLOR_NAME);
	this->_players[0].setColor(CONNECTFOUR_PLAYER0_COLOR_VALUE);
	this->_players[0].setSelectionColor(CONNECTFOUR_PLAYER0_COLOR_VALUE);
	this->_players[1].setName(CONNECTFOUR_PLAYER1_COLOR_NAME);
	this->_players[1].setColor(CONNECTFOUR_PLAYER1_COLOR_VALUE);
	this->_players[1].setSelectionColor(CONNECTFOUR_PLAYER1_COLOR_VALUE);

	// Calc all Cylinder Min and Maxes for hit detection

	_cylinderMinX = new SLfloat[CONNECTFOUR_NUMBER_OF_CYLINDERS];
	_cylinderMaxX = new SLfloat[CONNECTFOUR_NUMBER_OF_CYLINDERS];
	_cylinderMinY = new SLfloat[CONNECTFOUR_NUMBER_OF_CYLINDERS];
	_cylinderMaxY = new SLfloat[CONNECTFOUR_NUMBER_OF_CYLINDERS];
	_cylinderMinZ = new SLfloat[CONNECTFOUR_NUMBER_OF_CYLINDERS];
	_cylinderMaxZ = new SLfloat[CONNECTFOUR_NUMBER_OF_CYLINDERS];

	SLfloat xVal = -CONNECTFOUR_CYLINDERS_DISTANCE - (CONNECTFOUR_CYLINDERS_DISTANCE / 2);
	SLfloat yVal = CONNECTFOUR_BOARD_Y_OFFSET + CONNECTFOUR_CYLINDERS_HEIGH - (2 * CONNECTFOUR_SPHERES_RADIUS);
	SLfloat zVal = 0.0f;

	int cnt = 0;
	for(SLuint y = 0; y < CONNECTFOUR_NUMBER_OF_ROWS_COLUMNS_LEVELS; y++)
	{
		zVal = -CONNECTFOUR_CYLINDERS_DISTANCE - (CONNECTFOUR_CYLINDERS_DISTANCE / 2);

		for(SLuint x = 0; x < CONNECTFOUR_NUMBER_OF_ROWS_COLUMNS_LEVELS; x++, cnt++)
		{
			_cylinderMinX[cnt] = xVal;
			_cylinderMinY[cnt] = yVal;
			_cylinderMinZ[cnt] = zVal;
			_cylinderMaxX[cnt] = xVal + (2 * CONNECTFOUR_SPHERES_RADIUS);
			_cylinderMaxY[cnt] = yVal + (2 * CONNECTFOUR_SPHERES_RADIUS);
			_cylinderMaxZ[cnt] = zVal + (2 * CONNECTFOUR_SPHERES_RADIUS);

			zVal += CONNECTFOUR_CYLINDERS_DISTANCE;
		}

		xVal += CONNECTFOUR_CYLINDERS_DISTANCE;
	}

	// Calc all the positions of the board fields
	xVal = 0.0f;
	yVal = CONNECTFOUR_BOARD_Y_OFFSET + CONNECTFOUR_SPHERES_RADIUS - (CONNECTFOUR_Y_SPHERE_OFFSET * 0.5f);
	SLfloat yValAbove = yVal + (CONNECTFOUR_SPHERES_RADIUS*8) - (CONNECTFOUR_Y_SPHERE_OFFSET);
	zVal = 0.0f;

	for(SLuint z = 0; z < CONNECTFOUR_NUMBER_OF_ROWS_COLUMNS_LEVELS; z++)
	{
		xVal = -CONNECTFOUR_CYLINDERS_DISTANCE - (CONNECTFOUR_CYLINDERS_DISTANCE / 2);

		for(SLuint y = 0; y < CONNECTFOUR_NUMBER_OF_ROWS_COLUMNS_LEVELS; y++)
		{
			zVal = -CONNECTFOUR_CYLINDERS_DISTANCE - (CONNECTFOUR_CYLINDERS_DISTANCE / 2);

			for(SLuint x = 0; x < CONNECTFOUR_NUMBER_OF_ROWS_COLUMNS_LEVELS; x++)
			{
				this->_boardPlayFields[z][y][x]._simulationStartPosition.x = xVal;
				this->_boardPlayFields[z][y][x]._simulationStartPosition.y = yValAbove;
				this->_boardPlayFields[z][y][x]._simulationStartPosition.z = zVal;
				this->_boardPlayFields[z][y][x]._simulationEndPosition.x = xVal;
				this->_boardPlayFields[z][y][x]._simulationEndPosition.y = yVal;
				this->_boardPlayFields[z][y][x]._simulationEndPosition.z = zVal;

				this->_boardPlayFields[z][y][x]._actualPosition = this->_boardPlayFields[z][y][x]._simulationEndPosition;

				this->_boardPlayFields[z][y][x]._initColor = SLCol4f::WHITE;
				this->_boardPlayFields[z][y][x]._color = SLCol4f::WHITE;

				zVal += CONNECTFOUR_CYLINDERS_DISTANCE;
			}

			xVal += CONNECTFOUR_CYLINDERS_DISTANCE;
		}

		yVal += ((CONNECTFOUR_SPHERES_RADIUS*2) - (CONNECTFOUR_Y_SPHERE_OFFSET));
	}
}
//-----------------------------------------------------------------------------
ConnectFourGame::~ConnectFourGame()
{
	delete[] _cylinderMinX;
	delete[] _cylinderMaxX;
	delete[] _cylinderMinY;
	delete[] _cylinderMaxY;
	delete[] _cylinderMinZ;
	delete[] _cylinderMaxZ;
}
//-----------------------------------------------------------------------------
/*! 
updates the game state
*/
void ConnectFourGame::updateGameState(int indexLastSelectedX, int indexLastSelectedY, int indexLastSelectedZ)
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
		// Mark field as winner!
		this->_boardPlayFields[indexLastSelectedZ][indexLastSelectedY][indexLastSelectedX]._isWinner = true;
		checkX = (indexLastSelectedX + 1) % _numberOfBoardPlayFieldRows;
		while(checkX != indexLastSelectedX)
		{
			this->_boardPlayFields[indexLastSelectedZ][indexLastSelectedY][checkX]._isWinner = true;

			checkX = (checkX + 1) % _numberOfBoardPlayFieldRows;
		}

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
		// Mark field as winner!
		this->_boardPlayFields[indexLastSelectedZ][indexLastSelectedY][indexLastSelectedX]._isWinner = true;
		checkY = (indexLastSelectedY + 1) % _numberOfBoardPlayFieldColumns;
		while(checkY != indexLastSelectedY)
		{
			this->_boardPlayFields[indexLastSelectedZ][checkY][indexLastSelectedX]._isWinner = true;

			checkY = (checkY + 1) % _numberOfBoardPlayFieldColumns;
		}

		this->finisheGame(VR_BOARDGAME_STATE_WON);
		return;
	}

	// Check level
	oneOther = false;
	int checkZ = (indexLastSelectedZ + 1) % _numberOfBoardPlayFieldLevels;
	while(checkZ != indexLastSelectedZ)
	{
		if (owner != this->_boardPlayFields[checkZ][indexLastSelectedY][indexLastSelectedX].getOwnerId())
		{
			oneOther = true;
			break;
		}

		checkZ = (checkZ + 1) % _numberOfBoardPlayFieldLevels;
	}

	// All identical, so one has won
	if (!oneOther)
	{
		// Mark field as winner!
		this->_boardPlayFields[indexLastSelectedZ][indexLastSelectedY][indexLastSelectedX]._isWinner = true;
		checkZ = (indexLastSelectedZ + 1) % _numberOfBoardPlayFieldLevels;
		while(checkZ != indexLastSelectedZ)
		{
			this->_boardPlayFields[checkZ][indexLastSelectedY][indexLastSelectedX]._isWinner = true;

			checkZ = (checkZ + 1) % _numberOfBoardPlayFieldLevels;
		}

		this->finisheGame(VR_BOARDGAME_STATE_WON);
		return;
	}

	// Check first diagonals in z plane
	if (indexLastSelectedY == indexLastSelectedX)
	{
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
			// Mark field as winner!
			this->_boardPlayFields[indexLastSelectedZ][indexLastSelectedY][indexLastSelectedX]._isWinner = true;
			checkXY = (indexLastSelectedX + 1) % _numberOfBoardPlayFieldRows;
			while(checkXY != indexLastSelectedX)
			{
				this->_boardPlayFields[indexLastSelectedZ][checkXY][checkXY]._isWinner = true;

				checkXY = (checkXY + 1) % _numberOfBoardPlayFieldRows;
			}

			this->finisheGame(VR_BOARDGAME_STATE_WON);
			return;
		}
	}

	// Check second diagonals in z plane
	if (indexLastSelectedY == (_numberOfBoardPlayFieldRows - 1 - indexLastSelectedX))
	{
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
			// Mark field as winner!
			this->_boardPlayFields[indexLastSelectedZ][indexLastSelectedY][indexLastSelectedX]._isWinner = true;
			checkXY = (indexLastSelectedX + 1) % _numberOfBoardPlayFieldRows;
			while(checkXY != indexLastSelectedX)
			{
				this->_boardPlayFields[indexLastSelectedZ][_numberOfBoardPlayFieldRows - 1 - checkXY][checkXY]._isWinner = true;

				checkXY = (checkXY + 1) % _numberOfBoardPlayFieldRows;
			}

			this->finisheGame(VR_BOARDGAME_STATE_WON);
			return;
		}
	}

	// Check first diagonals in x plane
	if (indexLastSelectedY == indexLastSelectedZ)
	{
		oneOther = false;
		int checkYZ = (indexLastSelectedY + 1) % _numberOfBoardPlayFieldColumns;
		while(checkYZ != indexLastSelectedY)
		{
			if (owner != this->_boardPlayFields[checkYZ][checkYZ][indexLastSelectedX].getOwnerId())
			{
				oneOther = true;
				break;
			}

			checkYZ = (checkYZ + 1) % _numberOfBoardPlayFieldColumns;
		}

		// All identical, so one has won
		if (!oneOther)
		{
			// Mark field as winner!
			this->_boardPlayFields[indexLastSelectedZ][indexLastSelectedY][indexLastSelectedX]._isWinner = true;
			checkYZ = (indexLastSelectedY + 1) % _numberOfBoardPlayFieldColumns;
			while(checkYZ != indexLastSelectedY)
			{
				this->_boardPlayFields[checkYZ][checkYZ][indexLastSelectedX]._isWinner = true;

				checkYZ = (checkYZ + 1) % _numberOfBoardPlayFieldColumns;
			}

			this->finisheGame(VR_BOARDGAME_STATE_WON);
			return;
		}
	}

	// Check second diagonals in x plane
	if (indexLastSelectedY == (_numberOfBoardPlayFieldColumns - 1 - indexLastSelectedZ))
	{
		oneOther = false;
		int checkYZ = (indexLastSelectedY + 1) % _numberOfBoardPlayFieldColumns;
		while(checkYZ != indexLastSelectedY)
		{
			if (owner != this->_boardPlayFields[_numberOfBoardPlayFieldColumns - 1 - checkYZ][checkYZ][indexLastSelectedX].getOwnerId())
			{
				oneOther = true;
				break;
			}

			checkYZ = (checkYZ + 1) % _numberOfBoardPlayFieldColumns;
		}

		// All identical, so one has won
		if (!oneOther)
		{
			// Mark field as winner!
			this->_boardPlayFields[indexLastSelectedZ][indexLastSelectedY][indexLastSelectedX]._isWinner = true;
			checkYZ = (indexLastSelectedY + 1) % _numberOfBoardPlayFieldColumns;
			while(checkYZ != indexLastSelectedY)
			{
				this->_boardPlayFields[_numberOfBoardPlayFieldColumns - 1 - checkYZ][checkYZ][indexLastSelectedX]._isWinner = true;

				checkYZ = (checkYZ + 1) % _numberOfBoardPlayFieldColumns;
			}

			this->finisheGame(VR_BOARDGAME_STATE_WON);
			return;
		}
	}

	// Check first diagonals in y plane
	if (indexLastSelectedX == indexLastSelectedZ)
	{
		oneOther = false;
		int checkXZ = (indexLastSelectedX + 1) % _numberOfBoardPlayFieldRows;
		while(checkXZ != indexLastSelectedX)
		{
			if (owner != this->_boardPlayFields[checkXZ][indexLastSelectedY][checkXZ].getOwnerId())
			{
				oneOther = true;
				break;
			}

			checkXZ = (checkXZ + 1) % _numberOfBoardPlayFieldRows;
		}

		// All identical, so one has won
		if (!oneOther)
		{
			// Mark field as winner!
			this->_boardPlayFields[indexLastSelectedZ][indexLastSelectedY][indexLastSelectedX]._isWinner = true;
			checkXZ = (indexLastSelectedX + 1) % _numberOfBoardPlayFieldRows;
			while(checkXZ != indexLastSelectedX)
			{
				this->_boardPlayFields[checkXZ][indexLastSelectedY][checkXZ]._isWinner = true;

				checkXZ = (checkXZ + 1) % _numberOfBoardPlayFieldRows;
			}

			this->finisheGame(VR_BOARDGAME_STATE_WON);
			return;
		}
	}

	// Check second diagonals in y plane
	if (indexLastSelectedX == (_numberOfBoardPlayFieldRows - 1 - indexLastSelectedZ))
	{
		oneOther = false;
		int checkXZ = (indexLastSelectedX + 1) % _numberOfBoardPlayFieldRows;
		while(checkXZ != indexLastSelectedX)
		{
			if (owner != this->_boardPlayFields[_numberOfBoardPlayFieldRows - 1 - checkXZ][indexLastSelectedY][checkXZ].getOwnerId())
			{
				oneOther = true;
				break;
			}

			checkXZ = (checkXZ + 1) % _numberOfBoardPlayFieldRows;
		}

		// All identical, so one has won
		if (!oneOther)
		{
			// Mark field as winner!
			this->_boardPlayFields[indexLastSelectedZ][indexLastSelectedY][indexLastSelectedX]._isWinner = true;
			checkXZ = (indexLastSelectedX + 1) % _numberOfBoardPlayFieldRows;
			while(checkXZ != indexLastSelectedX)
			{
				this->_boardPlayFields[_numberOfBoardPlayFieldRows - 1 - checkXZ][indexLastSelectedY][checkXZ]._isWinner = true;

				checkXZ = (checkXZ + 1) % _numberOfBoardPlayFieldRows;
			}

			this->finisheGame(VR_BOARDGAME_STATE_WON);
			return;
		}
	}

	// Check first diagonal in all three dimensions
	if (indexLastSelectedX == indexLastSelectedZ && indexLastSelectedX == indexLastSelectedY)
	{
		oneOther = false;
		int checkXYZ = (indexLastSelectedX + 1) % _numberOfBoardPlayFieldRows;
		while(checkXYZ != indexLastSelectedX)
		{
			if (owner != this->_boardPlayFields[checkXYZ][checkXYZ][checkXYZ].getOwnerId())
			{
				oneOther = true;
				break;
			}

			checkXYZ = (checkXYZ + 1) % _numberOfBoardPlayFieldRows;
		}

		// All identical, so one has won
		if (!oneOther)
		{
			// Mark field as winner!
			this->_boardPlayFields[indexLastSelectedZ][indexLastSelectedY][indexLastSelectedX]._isWinner = true;
			checkXYZ = (indexLastSelectedX + 1) % _numberOfBoardPlayFieldRows;
			while(checkXYZ != indexLastSelectedX)
			{
				this->_boardPlayFields[checkXYZ][checkXYZ][checkXYZ]._isWinner = true;

				checkXYZ = (checkXYZ + 1) % _numberOfBoardPlayFieldRows;
			}

			this->finisheGame(VR_BOARDGAME_STATE_WON);
			return;
		}
	}

	// Check second diagonal in all three dimensions
	if (indexLastSelectedX == (_numberOfBoardPlayFieldRows - 1 - indexLastSelectedZ) && indexLastSelectedX == (_numberOfBoardPlayFieldRows - 1 - indexLastSelectedY))
	{
		oneOther = false;
		int checkXYZ = (indexLastSelectedX + 1) % _numberOfBoardPlayFieldRows;
		while(checkXYZ != indexLastSelectedX)
		{
			if (owner != this->_boardPlayFields[_numberOfBoardPlayFieldRows - 1 - checkXYZ][_numberOfBoardPlayFieldRows - 1 - checkXYZ][checkXYZ].getOwnerId())
			{
				oneOther = true;
				break;
			}

			checkXYZ = (checkXYZ + 1) % _numberOfBoardPlayFieldRows;
		}

		// All identical, so one has won
		if (!oneOther)
		{
			// Mark field as winner!
			this->_boardPlayFields[indexLastSelectedZ][indexLastSelectedY][indexLastSelectedX]._isWinner = true;
			checkXYZ = (indexLastSelectedX + 1) % _numberOfBoardPlayFieldRows;
			while(checkXYZ != indexLastSelectedX)
			{
				this->_boardPlayFields[_numberOfBoardPlayFieldRows - 1 - checkXYZ][_numberOfBoardPlayFieldRows - 1 - checkXYZ][checkXYZ]._isWinner = true;

				checkXYZ = (checkXYZ + 1) % _numberOfBoardPlayFieldRows;
			}

			this->finisheGame(VR_BOARDGAME_STATE_WON);
			return;
		}
	}

	// Check third diagonal in all three dimensions
	if (indexLastSelectedX == (_numberOfBoardPlayFieldRows - 1 - indexLastSelectedZ) && indexLastSelectedX == indexLastSelectedY)
	{
		oneOther = false;
		int checkXYZ = (indexLastSelectedX + 1) % _numberOfBoardPlayFieldRows;
		while(checkXYZ != indexLastSelectedX)
		{
			if (owner != this->_boardPlayFields[_numberOfBoardPlayFieldRows - 1 - checkXYZ][checkXYZ][checkXYZ].getOwnerId())
			{
				oneOther = true;
				break;
			}

			checkXYZ = (checkXYZ + 1) % _numberOfBoardPlayFieldRows;
		}

		// All identical, so one has won
		if (!oneOther)
		{
			// Mark field as winner!
			this->_boardPlayFields[indexLastSelectedZ][indexLastSelectedY][indexLastSelectedX]._isWinner = true;
			checkXYZ = (indexLastSelectedX + 1) % _numberOfBoardPlayFieldRows;
			while(checkXYZ != indexLastSelectedX)
			{
				this->_boardPlayFields[_numberOfBoardPlayFieldRows - 1 - checkXYZ][checkXYZ][checkXYZ]._isWinner = true;

				checkXYZ = (checkXYZ + 1) % _numberOfBoardPlayFieldRows;
			}

			this->finisheGame(VR_BOARDGAME_STATE_WON);
			return;
		}
	}

	// Check fourth diagonal in all three dimensions
	if (indexLastSelectedX == indexLastSelectedZ && indexLastSelectedX == (_numberOfBoardPlayFieldRows - 1 - indexLastSelectedY))
	{
		oneOther = false;
		int checkXYZ = (indexLastSelectedX + 1) % _numberOfBoardPlayFieldRows;
		while(checkXYZ != indexLastSelectedX)
		{
			if (owner != this->_boardPlayFields[checkXYZ][_numberOfBoardPlayFieldRows - 1 - checkXYZ][checkXYZ].getOwnerId())
			{
				oneOther = true;
				break;
			}

			checkXYZ = (checkXYZ + 1) % _numberOfBoardPlayFieldRows;
		}

		// All identical, so one has won
		if (!oneOther)
		{
			// Mark field as winner!
			this->_boardPlayFields[indexLastSelectedZ][indexLastSelectedY][indexLastSelectedX]._isWinner = true;
			checkXYZ = (indexLastSelectedX + 1) % _numberOfBoardPlayFieldRows;
			while(checkXYZ != indexLastSelectedX)
			{
				this->_boardPlayFields[checkXYZ][_numberOfBoardPlayFieldRows - 1 - checkXYZ][checkXYZ]._isWinner = true;

				checkXYZ = (checkXYZ + 1) % _numberOfBoardPlayFieldRows;
			}

			this->finisheGame(VR_BOARDGAME_STATE_WON);
			return;
		}
	}

	// Check if still is runnning or game ended in a tie
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
WorldState* ConnectFourGame::getBoardState(SLTimer &timer, SLint maxPlayers)
{
	ConnectFourWorldState* worldState = new ConnectFourWorldState(timer.getElapsedTimeInMilliSec(), maxPlayers);

	// Get Cylinder colors

	int cnt = 0;
	for(SLuint y = 0; y < this->_numberOfBoardPlayFieldColumns; y++)
	{
		for(SLuint x = 0; x < this->_numberOfBoardPlayFieldRows; x++, cnt++)
		{
			bool isSelected = false;

			for(SLuint z = 0; z < this->_numberOfBoardPlayFieldLevels; z++)
			{
				if (this->_boardPlayFields[z][y][x]._selected)
				{
					worldState->_cylinderColors[cnt] = this->_boardPlayFields[z][y][x]._color;
					isSelected = true;
					break;
				}
			}

			if (!isSelected)
			{
				// Pick color of one of the fields
				worldState->_cylinderColors[cnt] = this->_boardPlayFields[0][y][x]._color;
			}
		}
	}

	// Get sphere infos
	int numberOfSpheres = 0;
	for(SLuint z = 0; z < this->_numberOfBoardPlayFieldLevels; z++)
	{
		for(SLuint y = 0; y < this->_numberOfBoardPlayFieldColumns; y++)
		{
			for(SLuint x = 0; x < this->_numberOfBoardPlayFieldRows; x++)
			{
				if (this->_boardPlayFields[z][y][x]._owner != NULL)
				{
					numberOfSpheres++;
				}
			}
		}
	}

	worldState->_numberOfSpheres = numberOfSpheres;
	worldState->_sphereColors = new SLCol4f[numberOfSpheres];
	worldState->_spherePositions = new SLVec3f[numberOfSpheres];
	worldState->_sphereIsWinner = new SLbool[numberOfSpheres];

	cnt = 0;
	int test = 0;
	if(cnt != numberOfSpheres)
	{
		for(SLuint z = 0; z < this->_numberOfBoardPlayFieldLevels; z++)
		{
			for(SLuint y = 0; y < this->_numberOfBoardPlayFieldColumns; y++)
			{
				for(SLuint x = 0; x < this->_numberOfBoardPlayFieldRows; x++)
				{
					if (this->_boardPlayFields[z][y][x]._owner != NULL)
					{
						worldState->_sphereColors[cnt] = this->_boardPlayFields[z][y][x]._owner->getColor();
						worldState->_spherePositions[cnt] = this->_boardPlayFields[z][y][x]._actualPosition;
						worldState->_sphereIsWinner[cnt] = this->_boardPlayFields[z][y][x]._isWinner;
						cnt++;
					}
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
return an ConnectFourFallingSphereSimulation
*/
BoardGameSimulation* ConnectFourGame::getMoveSimulation(BoardPlayField* boardPlayField, int indexLastSelectedX, int indexLastSelectedY, int indexLastSelectedZ)
{
	return new ConnectFourFallingSphereSimulation(boardPlayField, indexLastSelectedZ);
}
//-----------------------------------------------------------------------------
/*! 
checks if a field of the game is hitted
*/
BoardPlayField* ConnectFourGame::checkHit(SLVec3f& effectivePosLeftHand, SLVec3f& effectivePosRightHand, int& indexLastSelectedX, int& indexLastSelectedY, int& indexLastSelectedZ)
{
	BoardPlayField* BoardPlayField = NULL;

	bool found = false;
	SLuint y = 0;
	SLuint x = 0;

	int cnt = 0;
	for(y = 0; y < this->_numberOfBoardPlayFieldColumns; y++)
	{
		for(x = 0; x < this->_numberOfBoardPlayFieldRows; x++, cnt++)
		{
			if (this->checkHitWithCylinder(cnt, effectivePosLeftHand) || this->checkHitWithCylinder(cnt, effectivePosRightHand))
			{
				found = true;
				break;
			}
		}

		if (found)
		{
			break;
		}
	}

	if (found)
	{
		for(SLuint z = 0; z < this->_numberOfBoardPlayFieldLevels; z++)
		{
			if (this->_boardPlayFields[z][y][x]._owner == NULL)
			{
				BoardPlayField = &this->_boardPlayFields[z][y][x];

				indexLastSelectedX = x;
				indexLastSelectedY = y;
				indexLastSelectedZ = z;

				break;
			}
		}
	}

	return BoardPlayField;
}
//-----------------------------------------------------------------------------
/*! 
checks the hit with the cylinder
*/
SLbool ConnectFourGame::checkHitWithCylinder(SLuint cylinderIndex, SLVec3f armPosition)
{
	return (armPosition.x > (this->_cylinderMinX[cylinderIndex]-HAND_SELECTION_RADIUS) && armPosition.x < (this->_cylinderMaxX[cylinderIndex]+HAND_SELECTION_RADIUS) &&
		armPosition.y > (this->_cylinderMinY[cylinderIndex]-HAND_SELECTION_RADIUS) && armPosition.y < (this->_cylinderMaxY[cylinderIndex]+HAND_SELECTION_RADIUS) &&
		armPosition.z > (this->_cylinderMinZ[cylinderIndex]-HAND_SELECTION_RADIUS) && armPosition.z < (this->_cylinderMaxZ[cylinderIndex]+HAND_SELECTION_RADIUS));
}
//-----------------------------------------------------------------------------