//#############################################################################
//  File:      VRTurnTableGameScene.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRTurnTableGameScene.h"
#include "SLBox.h"
#include <VRTurnTableWorldState.h>
#include "SLAssImp.h"
#include "SLScene.h"

//-----------------------------------------------------------------------------
TurnTableGameScene::TurnTableGameScene(SkeletonData* defaultSkeletonData,  SLint localPlayerIndex, SLint maxPlayers, SLMat4f* playerPositions, VR_MODEL* models) :
	GameScene(defaultSkeletonData, localPlayerIndex, maxPlayers, playerPositions, models)
{
	_matTable = new SLMaterial[3];
	_matTable[0].name("matTableLeg");
	_matTable[0].ambient(SLVec4f(0.0f, 0.0f, 0.0f, 1.0f));
	_matTable[0].diffuse(SLVec4f(0.0f, 0.0f, 0.0f, 1.0f));

	_matTable[1].name("matTablePlate");
	_matTable[1].ambient(SLVec4f(1.0f, 1.0f, 1.0f, 0.6f));
	_matTable[1].diffuse(SLVec4f(1.0f, 1.0f, 1.0f, 0.6f));

	_matTable[2].name("matTestBox");
	_matTable[2].ambient(SLVec4f(1.0f, 0.5f, 0.0f, 1.0f));
	_matTable[2].diffuse(SLVec4f(1.0f, 0.5f, 0.0f, 1.0f));

	SLCylinder* cylinder = new SLCylinder(0.15f, 0.8f, 1, 32, true, true, "tableLeg", &_matTable[0]);
	SLNode* tableLeg = new SLNode("Table Leg");
	tableLeg->addMesh(cylinder);
	tableLeg->rotate(-90, 1, 0, 0);
	_rootNode->addChild(tableLeg);

	_table = new SLNode("turnTable");
	_table->translate(0, 0.8f, 0);
	
	cylinder = new SLCylinder(0.5f, 0.04f, 1, 32, true, true, "tablePlate", &_matTable[1]);
	SLNode* tablePlate = new SLNode("Table Plate");
	tablePlate->addMesh(cylinder);
	tablePlate->rotate(-90, 1, 0, 0);
	_table->addChild(tablePlate);

	SLNode* tableObjects = new SLNode("tableObjects");
	_table->addChild(tableObjects);
	tableObjects->translate(0, 0.04f, 0);

	// test object
	SLNode* pot = SLAssImp::load("../_data/models/teapot.obj");


	pot->translate(0, 0.138f, 0);
	pot->scale(0.2f);
	tableObjects->addChild(pot);

	_rootNode->addChild(_table);
}
//-----------------------------------------------------------------------------
TurnTableGameScene::~TurnTableGameScene()
{
	//delete[] _matTable; removed to fix error on shutdown
}
//-----------------------------------------------------------------------------
/*! 
update game from world state
*/
void TurnTableGameScene::updateFromWorldState(RakNet::BitStream* bsIn, SkeletonData* defaultData)
{
	TurnTableWorldState* message = new TurnTableWorldState(bsIn);
	
	// Always call this for rebuilding the persons
	this->rebuildPersons(message->_hasSkeletonData, message->_playerData, defaultData);

	_table->rotation(0, SLVec3f(0, 0, 0));
    _table->om(message->_rotation.toMat4() * _table->om());

	delete message;
}
//-----------------------------------------------------------------------------