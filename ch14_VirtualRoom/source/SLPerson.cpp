//#############################################################################
//  File:      SLPerson.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "SLPerson.h"
#include "SLSphere.h"
#include "SLBox.h"
#include "SLRectangle.h"
#include "SLCamera.h"
#include "SLScene.h"
#include "SLSceneView.h"
#include "SLAssImp.h"
#include "SLImporter.h"

//-----------------------------------------------------------------------------
SLPerson::SLPerson(SLMaterial* mat, SL_PERSON_TYPE type, SkeletonData* data, SLstring name, VR_MODEL model) : SLNode(name) 
{  
	_mat = mat;
	_type = type;
	_model = model;
	_skeleton = NULL;
	this->BuildFigureGroup(data);
}

SLPerson::~SLPerson()
{
	if(_skeleton)
		delete _skeleton;
}
//-----------------------------------------------------------------------------
/*!
initialize the skeleton
*/
void SLPerson::BuildFigureGroup(SkeletonData* data)
{
   SLBox* cyl;
   SLSphere* sph;

   // init skeleton groups with spheres
   for(int i = 0; i < VR_SKELETON_POSITION_COUNT; i++)
   {
	   // create skeleton groups
	   _skeletonGroups[i]  = new SLNode;
	   
	   sph = new SLSphere(0.05f,32,32,"joint_", _mat);
	   _skeletonGroups[i]->addMesh(sph);
       
		cyl = new SLBox(-0.015f, -0.015f, 0, 0.015f, 0.015f, 1, "Box", _mat);// (0.03f,1,1,32, true, true, "bone_", _mat);
        //cyl = new SLRectangle(SLVec2f(-1, -1), SLVec2f(1, 1), 10, 10, "Box", _mat);

		_bones[i] = new SLNode("BoneBox " + to_string(i));
		if(i != 0)
			_bones[i]->addMesh(cyl);

		_bones[i]->rotate(-90, 1, 0, 0);
		_skeletonGroups[i]->addChild(_bones[i]);

		// HIDE SKELETON HACK: hiding all bones for now, we still need the skeleton for the camera atm
		/*if(_model != VR_STICKMAN) {
			_bones[i]->hide();
			sph->hide();
		}
		if(_type == SL_JOINTS_ONLY || i == 0)
			_bones[i]->hide();		*/	
   }

   // hide feet
   //_bones[VR_SKELETON_POSITION_FOOT_LEFT]->hide();
   //_bones[VR_SKELETON_POSITION_FOOT_RIGHT]->hide();
   
   // build skeleton hierarchy
   this->addChild(_skeletonGroups[VR_SKELETON_POSITION_SPINE_BASE]);
   
   for(int i = 1; i < VR_SKELETON_POSITION_COUNT; i++)
	   _skeletonGroups[data->bones[i].parent]->addChild(_skeletonGroups[i]);

   // cam
   // HIDE SKELETON HACK: hiding all bones for now, we still need the skeleton for the camera atm
   //sph = new SLSphere(0.05f,32,32,"head", _mat);
   //_camGroup->addNode(sph);
   _cam = new SLCamera();
   _cam->focalDist(20);
   _cam->fov(120);
   _cam->name(_name + "camera");

   // set perspective
    if(SLScene::current->oculus()->isConnected())
		_cam->projection(stereoSideBySideD);
	else
		_cam->projection(monoPerspective);
   
   _skeletonGroups[VR_SKELETON_POSITION_HEAD]->addChild(_cam);

   // import our mesh that represents the player
   importModel();

    // Init rebuild of figure
   this->rebuildFigure(data);
}
//-----------------------------------------------------------------------------
/*!
activates the person POV
*/
void SLPerson::makeCamActive()
{
	SLScene* s = SLScene::current;
    // virtual room only has a single scene view, so we work on 0.
	s->sv(0)->camera(_cam);
}
//-----------------------------------------------------------------------------
/*!
rebuilds the skeleton with the given data
*/
void SLPerson::rebuildFigure(SkeletonData* data)
{
	// rebuild figure
	this->rebuildFigureJointsAndBones(data);

	// set cam matrix

	_cam->om(SLMat4f());
    //_cam->rotate(180, 1, 0, 0);

    
	//fix the cam position for the HL meshes
	switch(_model) {
	case VR_HL2_CIV_MALE: _cam->position(0.0f,0.07f, -0.03f); break;
	case VR_HL2_CIV_FEMALE: _cam->position(0.0f,0.03f, -0.03f); break;
	default: _cam->position(0.0f, 0.18f, 0.15f); break;
	}

	//_cam->setMatrix(SLMat4f(0.0f, 0.15f, 0.0f) * data->headRotation.toMat4() * (*_cam)); // <-old code emulated by the two lines below
   // _cam->rotate(data->headRotation);
   // _cam->translate(0.0f, 0.15f, 0.0f);
    _cam->om(SLMat4f(0.0f, 0.15f, 0.0f) * data->headRotation.toMat4() * _cam->om());
	//*_camGroup *= data->headRotation.toMat4();
	//_camGroup->translation(0, data->bones[VR_SKELETON_POSITION_HEAD].length, 0);

}
//-----------------------------------------------------------------------------
/*!
recalculates the bone position and orientation
*/
void SLPerson::rebuildFigureJointsAndBones(SkeletonData* data)
{
	// rebuild skeleton
	for(int i = 0; i < VR_SKELETON_POSITION_COUNT; i++)
	{			
		// translate and scale bones
        _skeletonGroups[i]->om(SLMat4f());
		if(i == VR_SKELETON_POSITION_SPINE_BASE){
			_skeletonGroups[i]->position(data->position.x, data->position.y, data->position.z);
		}else{
			_skeletonGroups[i]->position(0, data->bones[data->bones[i].parent].length, 0);

			// scale bones
            _bones[i]->om(SLMat4f());
			if(data->bones[i].length > 0.1){
				_bones[i]->scale(1, data->bones[i].length, 1);
			}else{
				_bones[i]->scale(1, 0.1f, 1);
			}			
			_bones[i]->rotate(-90, 1, 0, 0);
		}

		// roatet groups
        _skeletonGroups[i]->om(_skeletonGroups[i]->om() * data->bones[i].rotation.toMat4());
	}

	_skeletonKinectBinding.updateSkeleton(data);

	// update hand positions
	calcHandPositions(data);
}
//-----------------------------------------------------------------------------
/*!
Calculate the hand positions of the mesh
*/
void SLPerson::calcHandPositions(SkeletonData* data)
{
	// temp display hand position
	SLMat4f leftMat, rightMat;
	
	if(_model == VR_STICKMAN) {
		leftMat.translate(calculatePositionOfJoint(data, VR_SKELETON_POSITION_HAND_LEFT));
		rightMat.translate(calculatePositionOfJoint(data, VR_SKELETON_POSITION_HAND_RIGHT));

		leftMat = this->om() * leftMat;
		rightMat = this->om() * rightMat;

		_lastLeftHandPos = leftMat.translation();	
		_lastRightHandPos = rightMat.translation();
	}
	else
	{
		SLBoneNode* boneL = _skeleton->root()->findBone("ValveBiped.Bip01_L_Finger21");
		if(boneL) 
			leftMat.setMatrix(this->om() * _modelGroup->om() * boneL->globalTransform());
	
		SLBoneNode* boneR = _skeleton->root()->findBone("ValveBiped.Bip01_R_Finger21");
		if(boneR) {
			rightMat.setMatrix(this->om() * _modelGroup->om() * boneR->globalTransform());
		}

		_lastLeftHandPos = leftMat.translation();	
		_lastRightHandPos = rightMat.translation();
	}
}

//-----------------------------------------------------------------------------
/*!
Import the set model
*/
void SLPerson::importModel()
{
	SLstring modelPath;

	// on stickman do nothing, else load the correct file
	switch(_model) {
	case VR_STICKMAN: return;
	case VR_ASTROBOY: modelPath = "../_data/models/astroboy/astroBoy_walk_Maya.dae"; break;
	case VR_HL2_CIV_MALE: modelPath = "../_data/models/source engine/hl2/male2/finished.dae"; break;
	case VR_HL2_CIV_FEMALE: modelPath = "../_data/models/source engine/hl2/female2/finished.dae"; break;
	default: return;
	}
	
	// hl2 civilian
	_modelGroup = new SLNode;
	_modelGroup->scale(0.022f);
	_modelGroup->rotate(180, 1, 0, 0);
    addChild(_modelGroup);

	SLImporter importer;
	importer.import(modelPath);
	importer.loadIntoScene(SLScene::current, _modelGroup);
		
	_skeleton = new SLSkeleton(importer.root());
	_skeletonKinectBinding.setSkeleton(_skeleton);

	// overwrite the materials for the skinned mesh to use the right shaders
	// TODO: Fix this, the shader should pick the skinning version automatically for a mesh like this
	for(auto mesh : importer.meshes())
		mesh->material()->shaderProg(new SLGLShaderProgGeneric("PerPixBlinn_Skinned.vert", "PerPixBlinnTex.frag"));
	
	_skeletonKinectBinding.useOculus(SLScene::current->oculus()->isConnected());
	_skeletonKinectBinding.setOculusBone("ValveBiped.Bip01_Head1");		
	_skeletonKinectBinding.setOculusToSkeleton(SLQuat4f(180, SLVec3f(0, 1, 0)) * SLQuat4f(180, SLVec3f(1, 0, 0)) * SLQuat4f(-90, SLVec3f(0, 1, 0)) * SLQuat4f(-90, SLVec3f(0, 0, 1)));
	_skeletonKinectBinding.setOculusOffset(SLQuat4f(180, SLVec3f(0, 1, 0)));

	// skeleton to kienct bindings are relative to the mesh again
	switch(_model) {
	case VR_ASTROBOY:
	{
		// create bindings
		_skeletonKinectBinding.addBinding(VR_SKELETON_POSITION_HEAD, "head");

		// left arm
		_skeletonKinectBinding.addBinding(VR_SKELETON_POSITION_ELBOW_LEFT, "L_shoulder");
		_skeletonKinectBinding.setRotationOffset(VR_SKELETON_POSITION_ELBOW_LEFT, SLQuat4f(-25, SLVec3f(0, 0, 1)));

		_skeletonKinectBinding.addBinding(VR_SKELETON_POSITION_WRIST_LEFT, "L_elbow");

		_skeletonKinectBinding.addBinding(VR_SKELETON_POSITION_HAND_LEFT, "L_wrist");


		// right arm
		_skeletonKinectBinding.addBinding(VR_SKELETON_POSITION_ELBOW_RIGHT, "R_shoulder");
		_skeletonKinectBinding.setKinectToSkeleton(VR_SKELETON_POSITION_ELBOW_RIGHT, SLQuat4f(180, SLVec3f(1, 0, 0)));
		_skeletonKinectBinding.setRotationOffset(VR_SKELETON_POSITION_ELBOW_RIGHT, SLQuat4f(25, SLVec3f(0, 0, 1)));
		
		_skeletonKinectBinding.addBinding(VR_SKELETON_POSITION_WRIST_RIGHT, "R_elbow");
		_skeletonKinectBinding.setKinectToSkeleton(VR_SKELETON_POSITION_WRIST_RIGHT, SLQuat4f(180, SLVec3f(1, 0, 0)));

		_skeletonKinectBinding.addBinding(VR_SKELETON_POSITION_HAND_RIGHT, "R_wrist");
		_skeletonKinectBinding.setKinectToSkeleton(VR_SKELETON_POSITION_HAND_RIGHT, SLQuat4f(180, SLVec3f(1, 0, 0)));
		
		// left leg
		_skeletonKinectBinding.addBinding(VR_SKELETON_POSITION_KNEE_LEFT, "L_hip");
		_skeletonKinectBinding.setRotationOffset(VR_SKELETON_POSITION_KNEE_LEFT, SLQuat4f(45, SLVec3f(0, 0, 1)));
		
		_skeletonKinectBinding.addBinding(VR_SKELETON_POSITION_ANKLE_LEFT, "L_knee_01");
		_skeletonKinectBinding.setKinectToSkeleton(VR_SKELETON_POSITION_ANKLE_LEFT, SLQuat4f(180, SLVec3f(0, 1, 0)));

		// right leg
		_skeletonKinectBinding.addBinding(VR_SKELETON_POSITION_KNEE_RIGHT, "R_hip");
		_skeletonKinectBinding.setKinectToSkeleton(VR_SKELETON_POSITION_KNEE_RIGHT, SLQuat4f(180, SLVec3f(1, 0, 0)));
		_skeletonKinectBinding.setRotationOffset(VR_SKELETON_POSITION_KNEE_RIGHT, SLQuat4f(-45, SLVec3f(0, 0, 1)));
		
		_skeletonKinectBinding.addBinding(VR_SKELETON_POSITION_ANKLE_RIGHT, "R_knee_01");
		_skeletonKinectBinding.setKinectToSkeleton(VR_SKELETON_POSITION_ANKLE_RIGHT, SLQuat4f(180, SLVec3f(0, 1, 0)));
		
		// torso
		_skeletonKinectBinding.addBinding(VR_SKELETON_POSITION_SPINE_BASE, "root");
		_skeletonKinectBinding.setKinectToSkeleton(VR_SKELETON_POSITION_SPINE_BASE, SLQuat4f(90, SLVec3f(0, 1, 0)));
		
		_skeletonKinectBinding.addBinding(VR_SKELETON_POSITION_SPINE_MID, "spine01");
	}
	break;
	case VR_HL2_CIV_FEMALE:
	case VR_HL2_CIV_MALE:
	{

		// torso
		_skeletonKinectBinding.addBinding(VR_SKELETON_POSITION_SPINE_BASE, "ValveBiped.Bip01_Pelvis");
		
		// create bindings
		_skeletonKinectBinding.addBinding(VR_SKELETON_POSITION_HEAD, "ValveBiped.Bip01_Head1");
		_skeletonKinectBinding.setKinectToSkeleton(VR_SKELETON_POSITION_HEAD, SLQuat4f(180, SLVec3f(1, 0, 0)) * SLQuat4f(-90, SLVec3f(0, 1, 0)) * SLQuat4f(-90, SLVec3f(0, 0, 1)));
	
	
		// left arm
		_skeletonKinectBinding.addBinding(VR_SKELETON_POSITION_ELBOW_LEFT, "ValveBiped.Bip01_L_UpperArm");
		_skeletonKinectBinding.setKinectToSkeleton(VR_SKELETON_POSITION_ELBOW_LEFT, SLQuat4f(-90, SLVec3f(0, 1, 0)) * SLQuat4f(90, SLVec3f(0, 0, 1)));
		_skeletonKinectBinding.setRotationOffset(VR_SKELETON_POSITION_ELBOW_LEFT, SLQuat4f(20, SLVec3f(0, 0, 1)));
	
	
		_skeletonKinectBinding.addBinding(VR_SKELETON_POSITION_WRIST_LEFT, "ValveBiped.Bip01_L_Forearm");
		_skeletonKinectBinding.setKinectToSkeleton(VR_SKELETON_POSITION_WRIST_LEFT, SLQuat4f(-90, SLVec3f(0, 1, 0)) * SLQuat4f(90, SLVec3f(0, 0, 1)));
	
		_skeletonKinectBinding.addBinding(VR_SKELETON_POSITION_HAND_LEFT, "ValveBiped.Bip01_L_Hand");
		_skeletonKinectBinding.setKinectToSkeleton(VR_SKELETON_POSITION_HAND_LEFT, SLQuat4f(90, SLVec3f(0, 0, 1)));
	

		// right arm
		_skeletonKinectBinding.addBinding(VR_SKELETON_POSITION_ELBOW_RIGHT, "ValveBiped.Bip01_R_UpperArm");	
		_skeletonKinectBinding.setKinectToSkeleton(VR_SKELETON_POSITION_ELBOW_RIGHT, SLQuat4f(-90, SLVec3f(0, 1, 0)) * SLQuat4f(90, SLVec3f(0, 0, 1)));
		_skeletonKinectBinding.setRotationOffset(VR_SKELETON_POSITION_ELBOW_RIGHT, SLQuat4f(-20, SLVec3f(0, 0, 1)));

		_skeletonKinectBinding.addBinding(VR_SKELETON_POSITION_WRIST_RIGHT, "ValveBiped.Bip01_R_Forearm");
		_skeletonKinectBinding.setKinectToSkeleton(VR_SKELETON_POSITION_WRIST_RIGHT, SLQuat4f(-90, SLVec3f(0, 1, 0)) * SLQuat4f(90, SLVec3f(0, 0, 1)));

		_skeletonKinectBinding.addBinding(VR_SKELETON_POSITION_HAND_RIGHT, "ValveBiped.Bip01_R_Hand");
		_skeletonKinectBinding.setKinectToSkeleton(VR_SKELETON_POSITION_HAND_RIGHT, SLQuat4f(180, SLVec3f(0, 1, 0)) * SLQuat4f(90, SLVec3f(0, 0, 1)));
		
		// left leg	
		_skeletonKinectBinding.addBinding(VR_SKELETON_POSITION_KNEE_LEFT, "ValveBiped.Bip01_L_Thigh");
		_skeletonKinectBinding.setRotationOffset(VR_SKELETON_POSITION_KNEE_LEFT, SLQuat4f(45, SLVec3f(0, 0, 1)));
		_skeletonKinectBinding.setKinectToSkeleton(VR_SKELETON_POSITION_KNEE_LEFT, SLQuat4f(-90, SLVec3f(0, 1, 0)) * SLQuat4f(90, SLVec3f(0, 0, 1)));
	
		_skeletonKinectBinding.addBinding(VR_SKELETON_POSITION_ANKLE_LEFT, "ValveBiped.Bip01_L_Calf");
		_skeletonKinectBinding.setKinectToSkeleton(VR_SKELETON_POSITION_ANKLE_LEFT, SLQuat4f(-90, SLVec3f(0, 1, 0)) * SLQuat4f(90, SLVec3f(0, 0, 1)));

		// right leg
		_skeletonKinectBinding.addBinding(VR_SKELETON_POSITION_KNEE_RIGHT, "ValveBiped.Bip01_R_Thigh");
		_skeletonKinectBinding.setRotationOffset(VR_SKELETON_POSITION_KNEE_RIGHT, SLQuat4f(-45, SLVec3f(0, 0, 1)));
		_skeletonKinectBinding.setKinectToSkeleton(VR_SKELETON_POSITION_KNEE_RIGHT, SLQuat4f(-90, SLVec3f(0, 1, 0)) * SLQuat4f(90, SLVec3f(0, 0, 1)));
	
		_skeletonKinectBinding.addBinding(VR_SKELETON_POSITION_ANKLE_RIGHT, "ValveBiped.Bip01_R_Calf");
		_skeletonKinectBinding.setKinectToSkeleton(VR_SKELETON_POSITION_ANKLE_RIGHT, SLQuat4f(-90, SLVec3f(0, 1, 0)) * SLQuat4f(90, SLVec3f(0, 0, 1)));
	}
	break;
	}
}