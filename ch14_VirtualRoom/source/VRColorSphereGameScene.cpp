//#############################################################################
//  File:      VRColorSphereGameScene.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRColorSphereGameScene.h"

//-----------------------------------------------------------------------------
ColorSphereGameScene::ColorSphereGameScene(SkeletonData* defaultSkeletonData,  SLint localPlayerIndex, SLint maxPlayers, SLMat4f* playerPositions, VR_MODEL* models) :
	GameScene(defaultSkeletonData, localPlayerIndex, maxPlayers, playerPositions, models)
{
	// material
	this->_matSphere = new SLMaterial("matSphere", SLCol4f::BLACK);

	this->_sphere = new SLSphere(0.2f, 32, 32, "Color Sphere Game 1", this->_matSphere);
	SLNode* sphereNode = new SLNode;
	sphereNode->translate(0.0f, 1.0f, 0.0f);
	sphereNode->addMesh(_sphere);

	this->_rootNode->addChild(sphereNode);
}
//-----------------------------------------------------------------------------
ColorSphereGameScene::~ColorSphereGameScene()
{
	//delete this->_matSphere; removed to fix error on shutdown
}
//-----------------------------------------------------------------------------
/*! 
update sphere color from world state
*/
void ColorSphereGameScene::updateFromWorldState(RakNet::BitStream* bsIn, SkeletonData* defaultData)
{
	ColorSphereWorldState* message = new ColorSphereWorldState(bsIn);
	
	// Always call this for rebuilding the persons
	this->rebuildPersons(message->_hasSkeletonData, message->_playerData, defaultData);

	this->_matSphere->ambient(message->_sphereColor);
	this->_matSphere->diffuse(message->_sphereColor);

	delete message;
}
//-----------------------------------------------------------------------------