//#############################################################################
//  File:      VRConnectFourGameScene.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRConnectFourGameScene.h"
#include "SLTexFont.h"

#include <SLScene.h>

//-----------------------------------------------------------------------------
ConnectFourGameScene::ConnectFourGameScene(SkeletonData* defaultSkeletonData,  SLint localPlayerIndex, SLint maxPlayers, SLMat4f* playerPositions, VR_MODEL* models) :
	BoardGameScene(defaultSkeletonData, localPlayerIndex, maxPlayers, playerPositions, models)
{
	this->_matCylinders = new SLMaterial*[CONNECTFOUR_NUMBER_OF_CYLINDERS];

	// material
	for(int i = 0; i < CONNECTFOUR_NUMBER_OF_CYLINDERS; i++)
	{
        this->_matCylinders[i] = new SLMaterial;
		this->_matCylinders[i]->name("matCylinder "+ to_string(i));
		this->_matCylinders[i]->ambient(SLCol4f::WHITE);
		this->_matCylinders[i]->ambient(SLCol4f::WHITE);
	}
	this->_matGround = new SLMaterial("matGroundConnectFour", SLCol4f::BLACK);
	this->_matFeet   = new SLMaterial("matFeetConnectFour", SLCol4f(0.6f, 0.6f, 0.6f));

	SLNode* board = new SLNode("board");

	SLCylinder* feet1 = new SLCylinder(CONNECTFOUR_CYLINDERS_RADIUS*2, (CONNECTFOUR_BOARD_Y_OFFSET - CONNECTFOUR_GROUD_HEIGHT + 0.01), 1, 32, true, true, "feet_1", this->_matFeet);
	SLNode* foot1 = new SLNode("foot1");
	foot1->addMesh(feet1); 
	foot1->translate(-CONNECTFOUR_CYLINDERS_DISTANCE - (CONNECTFOUR_CYLINDERS_DISTANCE / 2), 0, -CONNECTFOUR_CYLINDERS_DISTANCE - (CONNECTFOUR_CYLINDERS_DISTANCE / 2));
	foot1->rotate(-90, 1, 0, 0);
	board->addChild(foot1);

	SLCylinder* feet2 = new SLCylinder(CONNECTFOUR_CYLINDERS_RADIUS * 2, (CONNECTFOUR_BOARD_Y_OFFSET - CONNECTFOUR_GROUD_HEIGHT + 0.01), 1, 32, true, true, "feet_2", this->_matFeet);
	SLNode* foot2 = new SLNode("foot1");
    foot2->addMesh(feet2);
	foot2->translate(CONNECTFOUR_CYLINDERS_DISTANCE + (CONNECTFOUR_CYLINDERS_DISTANCE / 2), 0, -CONNECTFOUR_CYLINDERS_DISTANCE - (CONNECTFOUR_CYLINDERS_DISTANCE / 2));
	foot2->rotate(-90, 1, 0, 0);
	board->addChild(foot2);

	SLCylinder* feet3 = new SLCylinder(CONNECTFOUR_CYLINDERS_RADIUS * 2, (CONNECTFOUR_BOARD_Y_OFFSET - CONNECTFOUR_GROUD_HEIGHT + 0.01), 1, 32, true, true, "feet_3", this->_matFeet);
	SLNode* foot3 = new SLNode("foot1");
    foot3->addMesh(feet3);
	foot3->translate(-CONNECTFOUR_CYLINDERS_DISTANCE - (CONNECTFOUR_CYLINDERS_DISTANCE / 2), 0, CONNECTFOUR_CYLINDERS_DISTANCE + (CONNECTFOUR_CYLINDERS_DISTANCE / 2));
	foot3->rotate(-90, 1, 0, 0);
	board->addChild(foot3);

	SLCylinder* feet4 = new SLCylinder(CONNECTFOUR_CYLINDERS_RADIUS * 2, (CONNECTFOUR_BOARD_Y_OFFSET - CONNECTFOUR_GROUD_HEIGHT + 0.01), 1, 32, true, true, "feet_4", this->_matFeet);
	SLNode* foot4 = new SLNode("foot1");
    foot4->addMesh(feet4);
	foot4->translate(CONNECTFOUR_CYLINDERS_DISTANCE + (CONNECTFOUR_CYLINDERS_DISTANCE / 2), 0, CONNECTFOUR_CYLINDERS_DISTANCE + (CONNECTFOUR_CYLINDERS_DISTANCE / 2));
	foot4->rotate(-90, 1, 0, 0);
	board->addChild(foot4);

	SLBox* ground = new SLBox(0.0f, 0.0f, 0.0f, CONNECTFOUR_GROUD_WIDHT_DEPTH, CONNECTFOUR_GROUD_HEIGHT, CONNECTFOUR_GROUD_WIDHT_DEPTH, "Ground", _matGround);
	SLNode* groundNode = new SLNode("ground");
	groundNode->addMesh(ground);
	groundNode->translate(-CONNECTFOUR_GROUD_WIDHT_DEPTH / 2, (CONNECTFOUR_BOARD_Y_OFFSET - CONNECTFOUR_GROUD_HEIGHT), -CONNECTFOUR_GROUD_WIDHT_DEPTH / 2);
	board->addChild(groundNode);

	// Draw cylinders

	SLfloat xVal = -CONNECTFOUR_CYLINDERS_DISTANCE - (CONNECTFOUR_CYLINDERS_DISTANCE / 2);
	SLfloat yVal = CONNECTFOUR_BOARD_Y_OFFSET;
	SLfloat zVal = 0.0f;

	int cnt = 0;
	for(SLuint y = 0; y < CONNECTFOUR_NUMBER_OF_ROWS_COLUMNS_LEVELS; y++)
	{
		zVal = -CONNECTFOUR_CYLINDERS_DISTANCE - (CONNECTFOUR_CYLINDERS_DISTANCE / 2);

		for(SLuint x = 0; x < CONNECTFOUR_NUMBER_OF_ROWS_COLUMNS_LEVELS; x++, cnt++)
		{
			SLCylinder* cylinder = new SLCylinder(CONNECTFOUR_CYLINDERS_RADIUS, CONNECTFOUR_CYLINDERS_HEIGH, 1, 32, true, true, "cylinder_", this->_matCylinders[cnt]);
			SLNode* cylinderNode = new SLNode;
            cylinderNode->addMesh(cylinder);
			cylinderNode->translate(xVal, yVal, zVal);
			cylinderNode->rotate(-90, 1, 0, 0);
			board->addChild(cylinderNode);

			zVal += CONNECTFOUR_CYLINDERS_DISTANCE;
		}

		xVal += CONNECTFOUR_CYLINDERS_DISTANCE;
	}

	// Spheres
	_spheres = new SLSphere*[CONNECTFOUR_NUMBER_OF_CYLINDERS * CONNECTFOUR_NUMBER_OF_ROWS_COLUMNS_LEVELS];
	_sphereNodes = new SLNode*[CONNECTFOUR_NUMBER_OF_CYLINDERS * CONNECTFOUR_NUMBER_OF_ROWS_COLUMNS_LEVELS];
	_matSpheres = new SLMaterial*[CONNECTFOUR_NUMBER_OF_CYLINDERS * CONNECTFOUR_NUMBER_OF_ROWS_COLUMNS_LEVELS];
	_actualSpherePositions = new SLVec3f[CONNECTFOUR_NUMBER_OF_CYLINDERS * CONNECTFOUR_NUMBER_OF_ROWS_COLUMNS_LEVELS];
	_numberOfSpheres = 0;

	for(int i = 0; i < CONNECTFOUR_NUMBER_OF_CYLINDERS * CONNECTFOUR_NUMBER_OF_ROWS_COLUMNS_LEVELS; i++)
	{
        this->_matSpheres[i] = new SLMaterial;
		this->_matSpheres[i]->name("matSphere"+i);
		this->_matSpheres[i]->ambient(SLCol4f::WHITE);
		this->_matSpheres[i]->diffuse(SLCol4f::WHITE);
	}

	this->_rootNode->addChild(board);
}
//-----------------------------------------------------------------------------
ConnectFourGameScene::~ConnectFourGameScene()
{
	delete[] _matCylinders;
	delete[] _actualSpherePositions;
	//delete _matGround; removed to fix error on shutdown
	//delete _matFeet; removed to fix error on shutdown
	delete[] _spheres; 
	delete[] _matSpheres;
}
//-----------------------------------------------------------------------------
/*! 
update game from world state
*/
void ConnectFourGameScene::updateFromWorldState(RakNet::BitStream* bsIn, SkeletonData* defaultData)
{
	ConnectFourWorldState* message = new ConnectFourWorldState(bsIn);

	this->updateBoardGameFromWroldState(message, defaultData);

	for(int i = 0; i < CONNECTFOUR_NUMBER_OF_CYLINDERS; i++)
	{
		this->_matCylinders[i]->ambient(message->_cylinderColors[i]);
		this->_matCylinders[i]->diffuse(message->_cylinderColors[i]);
	}

	// Maybe check for remove (not possible usualy)
	if (_numberOfSpheres <= message->_numberOfSpheres)
	{
		// Update current spheres (maybe leave this out!)
		for(SLint i = 0; i < _numberOfSpheres; i++)
		{
			_matSpheres[i]->ambient(message->_sphereColors[i]);
			_matSpheres[i]->diffuse(message->_sphereColors[i]);
			if (message->_sphereIsWinner[i])
			{
				_matSpheres[i]->emission(message->_sphereColors[i]);
			}
			else
			{
				_matSpheres[i]->emission(SLCol4f::BLACK);
			}

			if (_actualSpherePositions[i] != message->_spherePositions[i])
			{
				_sphereNodes[i]->position(message->_spherePositions[i]);
				_actualSpherePositions[i] = message->_spherePositions[i];
			}
		}

		// Add new spheres
		for(SLint i = _numberOfSpheres; i < message->_numberOfSpheres; i++)
		{
			_spheres[i] = new SLSphere(CONNECTFOUR_SPHERES_RADIUS, 32, 32, "Sphere" + i, _matSpheres[i]);
			_sphereNodes[i] = new SLNode("Sphere");
			_sphereNodes[i]->addMesh(_spheres[i]);
			_matSpheres[i]->ambient(message->_sphereColors[i]);
			_matSpheres[i]->diffuse(message->_sphereColors[i]);
			if (message->_sphereIsWinner[i])
			{
				_matSpheres[i]->emission(message->_sphereColors[i]);
			}
			else
			{
				_matSpheres[i]->emission(SLCol4f::BLACK);
			}

			_sphereNodes[i]->om(SLMat4f());
			_sphereNodes[i]->translate(message->_spherePositions[i]);
			_actualSpherePositions[i] = message->_spherePositions[i];

			this->_rootNode->addChild(_sphereNodes[i]);

			_spheres[i]->init(getRootNode());
			_spheres[i]->buildAABB(*_sphereNodes[i]->aabb(), _sphereNodes[i]->updateAndGetWM());
		}

		_numberOfSpheres = message->_numberOfSpheres;
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
			SLstring colorWon = CONNECTFOUR_PLAYER0_COLOR_NAME;
			if (message->_actPlayerId == 1)
			{
				colorWon = CONNECTFOUR_PLAYER1_COLOR_NAME;
			}

			endText = "Game was won by " + colorWon;
			SL_LOG("\nGame was won by %s\n", colorWon.c_str());
		}

		this->changeGameState(message->_actPlayerId, endText, 600.0f * CONNECTFOUR_CYLINDERS_HEIGH);
	}

	delete message;
}
//-----------------------------------------------------------------------------
/*! 
resets the game to the initial state
*/
void ConnectFourGameScene::resetSpecificGame()
{
	for(int i = 0; i < _numberOfSpheres; i++)
	{
		this->_rootNode->deleteChild(_sphereNodes[i]);
	}
	_numberOfSpheres = 0;
}
//-----------------------------------------------------------------------------