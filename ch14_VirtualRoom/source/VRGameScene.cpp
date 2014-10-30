//#############################################################################
//  File:      VRGameScene.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRGameScene.h"
#include "SLCamera.h"
#include "SLScene.h"

//-----------------------------------------------------------------------------
GameScene::GameScene(SkeletonData* defaultSkeletonData,  SLint localPlayerIndex, SLint maxPlayers, SLMat4f* playerPositions, VR_MODEL* models)
	: _localPlayerIndex(localPlayerIndex), _maxPlayers(maxPlayers)
{
	// material
	this->_matLocalPerson = new SLMaterial("mG", SLCol4f::WHITE);
	this->_matExternalPerson = new SLMaterial("mR", SLCol4f::WHITE);

	// generate slPersons
	this->_rootNode = new SLNode();
	this->_persons = new SLPerson*[_maxPlayers];
	for(SLint i = 0; i < _maxPlayers; i++)
	{
		if(i == _localPlayerIndex){
			this->_persons[i] = new SLPerson(this->_matLocalPerson, SL_HIERARCHICAL_BONES, defaultSkeletonData, "local person", models[i]);
		}else{
			this->_persons[i] = new SLPerson(this->_matExternalPerson, SL_HIERARCHICAL_BONES, defaultSkeletonData, "external person ", models[i]);
			
		}
		this->_persons[i]->om(playerPositions[i]);
		this->_rootNode->addChild(this->_persons[i]);
	}

	if(localPlayerIndex >= 0)	
		this->activateCam(localPlayerIndex);
}
//-----------------------------------------------------------------------------
GameScene::~GameScene()
{
	delete[] _persons;
}
//-----------------------------------------------------------------------------
/*! 
update game scene from world state
*/
void GameScene::updateFromWorldState(RakNet::BitStream* bsIn, SkeletonData* defaultData)
{
	WorldState* message = new WorldState(bsIn);
	this->rebuildPersons(message->_hasSkeletonData, message->_playerData, defaultData);
	delete message;
}
//-----------------------------------------------------------------------------
/*! 
rebuilds the local player skeleton
*/
void GameScene::rebuildLocalFigure(SkeletonData* data){
	this->_persons[_localPlayerIndex]->rebuildFigure(data);
}

//-----------------------------------------------------------------------------
/*! 
retrieves the hand position of the local person in world space
this function is needed because certain mesh skeletons can have a different
ws position than the kinect data
*/
void GameScene::getLocalPersonHandPos(SLVec3f& leftHand, SLVec3f& rightHand)
{
	leftHand = this->_persons[_localPlayerIndex]->getLeftHandPos();
	rightHand = this->_persons[_localPlayerIndex]->getRightHandPos();
}

//-----------------------------------------------------------------------------
/*! 
rebuilds all skeletons
*/
void GameScene::rebuildPersons(SLbool* hasSkeletonData, SkeletonData* playerData, SkeletonData* defaultData)
{
	// rebuild persons
	for(SLint i = 0; i < _maxPlayers; i++)
	{
		if(i != _localPlayerIndex)
		{
			if(hasSkeletonData[i])
			{
				this->_persons[i]->rebuildFigure(&playerData[i]);
			}
			else
			{
				this->_persons[i]->rebuildFigure(defaultData);
			}
		}
	}
}
//-----------------------------------------------------------------------------
/*! 
activate the game scene camera
*/
void GameScene::activateCam(SLint playerIndex)
{
	this->_persons[playerIndex]->makeCamActive();
}
//-----------------------------------------------------------------------------