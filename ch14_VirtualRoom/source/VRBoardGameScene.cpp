//#############################################################################
//  File:      VRBoardGameScene.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRBoardGameScene.h"
#include "SLRectangle.h"
#include "SLTexFont.h"

#include <SLScene.h>

//-----------------------------------------------------------------------------
BoardGameScene::BoardGameScene(SkeletonData* defaultSkeletonData,  SLint localPlayerIndex, SLint maxPlayers, SLMat4f* playerPositions, VR_MODEL* models) :
	GameScene(defaultSkeletonData, localPlayerIndex, maxPlayers, playerPositions, models)
{
	this->_matResetBoxesForPlayer = new SLMaterial*[_maxPlayers];
	this->_textResetBoxes = new SLGLTexture("reset_text.jpg");
	this->_resetBoxesForPlayer = NULL;
	this->_textesForPlayer = NULL;

	for(int i = 0; i < _maxPlayers; i++)
	{
		this->_matResetBoxesForPlayer[i] = new SLMaterial("matResetBox"+i, this->_textResetBoxes);
	}

	_gameState = VR_BOARDGAME_STATE::VR_BOARDGAME_STATE_RUNNING;
}
//-----------------------------------------------------------------------------
BoardGameScene::~BoardGameScene()
{
	for(int i = 0; i < _maxPlayers; i++)
	{
		//delete _matResetBoxesForPlayer[i]; removed to fix error on shutdown
	}
	//delete[] _matResetBoxesForPlayer; removed to fix error on shutdown

	//delete _textResetBoxes; removed to fix error on shutdown

	if (_resetBoxesForPlayer != NULL)
		delete[] _resetBoxesForPlayer;

	if (_textesForPlayer != NULL)
		delete[] _textesForPlayer;
}
//-----------------------------------------------------------------------------
/*! 
updates the game from the given worldstate
*/
void BoardGameScene::updateBoardGameFromWroldState(BoardGameWorldState* message, SkeletonData* defaultData)
{
	// Always call this for rebuilding the persons
	this->rebuildPersons(message->_hasSkeletonData, message->_playerData, defaultData);

	for(int i = 0; i < _maxPlayers; i++)
	{
		this->_matResetBoxesForPlayer[i]->ambient(message->_resetBoxColors[i]);
		this->_matResetBoxesForPlayer[i]->diffuse(message->_resetBoxColors[i]);
	}
}
//-----------------------------------------------------------------------------
/*! 
changes the game state
*/
void BoardGameScene::changeGameState(SLint activePlayerId, SLstring endText, SLfloat heightOfText)
{
	if (this->_gameState == VR_BOARDGAME_STATE::VR_BOARDGAME_STATE_RUNNING)
	{
		SL_LOG("GAME STARTED AGAIN\n");

		for(int i = 0; i < _maxPlayers; i++)
		{
			this->_rootNode->deleteChild(_textesForPlayer[i]);
			this->_rootNode->deleteChild(_resetBoxesForPlayer[i]);
		}

		delete[] _resetBoxesForPlayer;
		_resetBoxesForPlayer = NULL;

		delete[] _textesForPlayer;
		_textesForPlayer = NULL;

		this->resetSpecificGame();
	}
	else
	{
		_resetBoxesForPlayer = new SLNode*[_maxPlayers];
		_textesForPlayer = new SLText*[_maxPlayers];
		for(int i = 0; i < _maxPlayers; i++)
		{
			_textesForPlayer[i] = new SLText(endText, SLTexFont::font12);

			SLfloat sclaingFactor = 0.005f;
			_textesForPlayer[i]->scale(sclaingFactor, sclaingFactor, sclaingFactor);

			SLMat4f mat = SLMat4f(this->_persons[i]->om());
			mat.translation(0.0f, 0.0f, 0.0f, true);
			mat *= _textesForPlayer[i]->om();
			_textesForPlayer[i]->om(mat);

			_textesForPlayer[i]->translate(-95.0f, heightOfText, 0.0f);

			this->_rootNode->addChild(_textesForPlayer[i]);

			_textesForPlayer[i]->updateAABBRec();

			_resetBoxesForPlayer[i] = new SLNode("ResetBoxPlayer" + i);

			// bottom rectangle
			SLRectangle* b = new SLRectangle(SLVec2f(0.0f, 0.0f), SLVec2f(RESET_BOX_CUBE_SIDE, RESET_BOX_CUBE_SIDE), 6, 6, "ResetBoxBottom" + i, this->_matResetBoxesForPlayer[i]);
			SLNode* nodeB = new SLNode;
			nodeB->addMesh(b);
			nodeB->rotate(90.0f, 1.0f, 0.0f, 0.0f);
			_resetBoxesForPlayer[i]->addChild(nodeB);

			// top rectangle
			SLRectangle* t = new SLRectangle(SLVec2f(0.0f, 0.0f), SLVec2f(RESET_BOX_CUBE_SIDE, RESET_BOX_CUBE_SIDE), 6, 6, "ResetBoxTop" + i, this->_matResetBoxesForPlayer[i]);
			SLNode* nodeT = new SLNode;
			nodeT->addMesh(t);
			nodeT->translate(0.0f, RESET_BOX_CUBE_SIDE, 0.0f);
			nodeT->rotate(-90.0f, 1.0f, 0.0f, 0.0f);
			nodeT->rotate(-90.0f, 0.0f, 0.0f, 1.0f);
			_resetBoxesForPlayer[i]->addChild(nodeT);
	
			// near rectangle
			SLRectangle* n = new SLRectangle(SLVec2f(0.0f, 0.0f), SLVec2f(RESET_BOX_CUBE_SIDE, RESET_BOX_CUBE_SIDE), 6, 6, "ResetBoxNear" + i, this->_matResetBoxesForPlayer[i]);
			SLNode* nodeN = new SLNode;
			nodeN->addMesh(n);
			nodeN->translate(0.0f, 0.0f, RESET_BOX_CUBE_SIDE);
			_resetBoxesForPlayer[i]->addChild(nodeN);

			// far rectangle
			SLRectangle* f = new SLRectangle(SLVec2f(0.0f, 0.0f), SLVec2f(RESET_BOX_CUBE_SIDE, RESET_BOX_CUBE_SIDE), 6, 6, "ResetBoxFar" + i, this->_matResetBoxesForPlayer[i]);
			SLNode* nodeF = new SLNode;
			nodeF->addMesh(f);
			nodeF->translate(RESET_BOX_CUBE_SIDE, 0.0f, 0.0f);
			nodeF->rotate(180.0f, 0.0f, 1.0f, 0.0f);
			_resetBoxesForPlayer[i]->addChild(nodeF);

			// left rectangle
			SLRectangle* l = new SLRectangle(SLVec2f(0.0f, 0.0f), SLVec2f(RESET_BOX_CUBE_SIDE, RESET_BOX_CUBE_SIDE), 6, 6, "ResetBoxLeft" + i, this->_matResetBoxesForPlayer[i]);
			SLNode* nodeL = new SLNode;
			nodeL->addMesh(l);
			nodeL->rotate(-90.0f, 0.0f, 1.0f, 0.0f);
			_resetBoxesForPlayer[i]->addChild(nodeL);

			// right rectangle
			SLRectangle* r = new SLRectangle(SLVec2f(0.0f, 0.0f), SLVec2f(RESET_BOX_CUBE_SIDE, RESET_BOX_CUBE_SIDE), 6, 6, "ResetBoxRight" + i, this->_matResetBoxesForPlayer[i]);
			SLNode* nodeR = new SLNode;
			nodeR->addMesh(r);
			nodeR->translate(RESET_BOX_CUBE_SIDE, 0.0f, RESET_BOX_CUBE_SIDE);
			nodeR->rotate(90.0f, 0.0f, 1.0f, 0.0f);
			_resetBoxesForPlayer[i]->addChild(nodeR);

			// Move whole box to the right position
            _resetBoxesForPlayer[i]->om(_resetBoxesForPlayer[i]->om() * this->_persons[i]->om());
			_resetBoxesForPlayer[i]->translate(RESET_BOX_POS_X, RESET_BOX_POS_Y, 0.0f);

			this->_rootNode->addChild(_resetBoxesForPlayer[i]);

			_resetBoxesForPlayer[i]->updateAABBRec();
		}
	}
}
//-----------------------------------------------------------------------------