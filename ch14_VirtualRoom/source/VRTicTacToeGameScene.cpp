//#############################################################################
//  File:      VRTicTacToeGameScene.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRTicTacToeGameScene.h"
#include "SLTexFont.h"

#include <SLScene.h>

//-----------------------------------------------------------------------------
TicTacToeGameScene::TicTacToeGameScene(SkeletonData* defaultSkeletonData,  SLint localPlayerIndex, SLint maxPlayers, SLMat4f* playerPositions, VR_MODEL* models) :
	BoardGameScene(defaultSkeletonData, localPlayerIndex, maxPlayers, playerPositions, models)
{
	this->_matBoxes = new SLMaterial[TICTACTOE_NUMBER_OF_FIELDS];

	// material
	for(int i = 0; i < TICTACTOE_NUMBER_OF_FIELDS; i++)
	{
		this->_matBoxes[i].name("matBox"+i);
		this->_matBoxes[i].ambient(SLVec4f(1.0f, 1.0f, 1.0f, BOX_ALPHA_VALUE));
		this->_matBoxes[i].diffuse(SLVec4f(1.0f, 1.0f, 1.0f, BOX_ALPHA_VALUE));
	}

	SLNode* board = new SLNode("board");

	SLfloat xVal = 0.0f;
    SLfloat yVal = - (TICTACTOE_BOARDGAME_FIELD_HEIGH + TICTACTOE_BOARDGAME_FIELD_BOARDER) - (TICTACTOE_BOARDGAME_FIELD_HEIGH / 2);
    SLfloat zVal = -(TICTACTOE_BOARDGAME_FIELD_DEPTH/2);

    int cnt = 0;
    for(int row = 0; row < TICTACTOE_NUMBER_OF_ROWS_COLUMNS; row++)
    {
            xVal = - (TICTACTOE_BOARDGAME_FIELD_WIDTH + TICTACTOE_BOARDGAME_FIELD_BOARDER) - (TICTACTOE_BOARDGAME_FIELD_WIDTH / 2);

            for(int column = 0; column < TICTACTOE_NUMBER_OF_ROWS_COLUMNS; column++, cnt++)
            {
                    SLBox* box = new SLBox(0.0f, 0.0f, 0.0f, TICTACTOE_BOARDGAME_FIELD_WIDTH, TICTACTOE_BOARDGAME_FIELD_HEIGH, TICTACTOE_BOARDGAME_FIELD_DEPTH, "Field " + cnt, &this->_matBoxes[cnt]); 
					SLNode* boxNode = new SLNode("fieldNode");
					boxNode->translate(xVal, yVal, zVal);
					board->addChild(boxNode);

                    xVal += (TICTACTOE_BOARDGAME_FIELD_WIDTH + TICTACTOE_BOARDGAME_FIELD_BOARDER);
            }

            yVal += (TICTACTOE_BOARDGAME_FIELD_HEIGH + TICTACTOE_BOARDGAME_FIELD_BOARDER);
    }

	board->translate(0.0f, TICTACTOE_BOARDGAME_Y_OFFSET, 0.0f);

	this->_rootNode->addChild(board);
}
//-----------------------------------------------------------------------------
TicTacToeGameScene::~TicTacToeGameScene()
{
	//delete[] _matBoxes; removed to fix error on shutdown
}
//-----------------------------------------------------------------------------
/*! 
update game from world state
*/
void TicTacToeGameScene::updateFromWorldState(RakNet::BitStream* bsIn, SkeletonData* defaultData)
{
	TicTacToeWorldState* message = new TicTacToeWorldState(bsIn);

	this->updateBoardGameFromWroldState(message, defaultData);

	for(int i = 0; i < TICTACTOE_NUMBER_OF_FIELDS; i++)
	{
		this->_matBoxes[i].ambient(message->_fieldColors[i]);
		this->_matBoxes[i].diffuse(message->_fieldColors[i]);
	}

	if (_gameState != message->_gameState)
	{
		this->_gameState = message->_gameState;

		SLstring endText = "";
		if (this->_gameState == VR_BOARDGAME_STATE::VR_BOARDGAME_STATE_TIE)
		{
			endText = "Game ended in a tie";
			SL_LOG("\nGame ended in a tie\n");
		}
		else
		{
			SLstring colorWon = TICTACTOE_PLAYER0_COLOR_NAME;
			if (message->_actPlayerId == 1)
			{
				colorWon = TICTACTOE_PLAYER1_COLOR_NAME;
			}

			endText = "Game was won by " + colorWon;
			SL_LOG("\nGame was won by %s\n", colorWon.c_str());
		}

		this->changeGameState(message->_actPlayerId, endText, 920.0f * (TICTACTOE_BOARDGAME_FIELD_BOARDER + TICTACTOE_BOARDGAME_FIELD_HEIGH));
	}

	delete message;
}
//-----------------------------------------------------------------------------