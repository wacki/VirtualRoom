//#############################################################################
//  File:      SLScene_onLoad.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include <stdafx.h>           // precompiled headers
#ifdef SL_MEMLEAKDETECT       // set in SL.h for debug config only
#include <debug_new.h>        // memory leak detector
#endif

#include "SLScene.h"
#include "VRSceneView.h"
#include "SLNode.h"
#include "SLCamera.h"
#include "SLLightSphere.h"
#include "SLRectangle.h"
#include "SLPerson.h"

//-----------------------------------------------------------------------------
/*! 
initates the default room for the different games
*/
void SLScene::onLoad(SLSceneView* sv, SLCmd sceneName)
{  
   // Initialize all preloaded stuff from SLScene
   cout << "------------------------------------------------------------------" << endl;
   init();
   _currentID = sceneName;

	name("Virtual Room");
	info(sv, "");

	// ligth
	SLLightSphere* light1 = new SLLightSphere(7,7,0, 0.1f, 15, 50);
	light1->attenuation(0,0,1);
	
	SLLightSphere* light2 = new SLLightSphere(-5,5,-5, 0.1f, 15, 50);
	light2->attenuation(0,0,1);
	
	SLNode* scene  = new SLNode;
	scene->addChild(light1);
	scene->addChild(light2);
	//cam1->projection(stereoSideBySideD);

	//create a virtual room
	
	// Testmap material
	SLGLTexture* roomTex = new SLGLTexture("cloth1024_C.png");
	SLMaterial* roomMat = new SLMaterial("roomMat", roomTex);
	
	SLGLTexture* roomTex2 = new SLGLTexture("parquet1600_C.jpg");
	SLMaterial* roomMat2 = new SLMaterial("roomMat2", roomTex2);

	// Cube dimensions
	SLfloat pL = -2.0f, pR = 2.0f; // left/right
	SLfloat pB = 0.0f, pT = 2.5f; // bottom/top
	SLfloat pN =  2.0f, pF =-2.0f; // near/far
	
	// var to hold the nodes to store the meshes in
	SLNode* meshNode;

	// bottom rectangle
	SLRectangle* b = new SLRectangle(SLVec2f(pL,-pN), SLVec2f(pR,-pF), 6, 6, "floor", roomMat2);
	meshNode = new SLNode;
	meshNode->addMesh(b);
	meshNode->rotate(90, -1, 0, 0); meshNode->translate(0, 0, pB);
	scene->addChild(meshNode);

	// top rectangle
	SLRectangle* t = new SLRectangle(SLVec2f(pL, pF), SLVec2f(pR, pN), 6, 6, "ceiling", roomMat);
	meshNode = new SLNode;
	meshNode->addMesh(t);
	meshNode->rotate(90, 1, 0, 0); meshNode->translate(0, 0, -pT);
	scene->addChild(meshNode);
	
	// near rectangle
	SLRectangle* n = new SLRectangle(SLVec2f(pL, pB), SLVec2f(pR, pT), 6, 6, "mearWall", roomMat);
	meshNode = new SLNode;
	meshNode->addMesh(n);
	meshNode->rotate(180, 0, 1, 0); meshNode->translate(0, 0, -pN);
	scene->addChild(meshNode);

	// far rectangle
	SLRectangle* f = new SLRectangle(SLVec2f(pL, pB), SLVec2f(pR, pT), 6, 6, "farWall", roomMat);
	meshNode = new SLNode;
	meshNode->addMesh(f);
	meshNode->translate(0, 0, pF);
	scene->addChild(meshNode);

	// left rectangle
	SLRectangle* l = new SLRectangle(SLVec2f(-pN, pB), SLVec2f(-pF, pT), 6, 6, "leftWall", roomMat);
	meshNode = new SLNode;
	meshNode->addMesh(l);
	meshNode->rotate(90, 0, 1, 0); meshNode->translate(0, 0, pL);
	scene->addChild(meshNode);

	// right rectangle
	SLRectangle* r = new SLRectangle(SLVec2f(pF, pB), SLVec2f(pN, pT), 6, 6, "rightWall", roomMat);
	meshNode = new SLNode;
	meshNode->addMesh(r);
	meshNode->rotate(90, 0, -1, 0); meshNode->translate(0, 0, -pR);
	scene->addChild(meshNode);
	

	_backColor.set(SLCol4f(0.1f,0.4f,0.8f));
	_root3D = scene;

   sv->onInitialize();
}
//-----------------------------------------------------------------------------
